#include "Animators.h"

#include <mutant/mutant.h>
#include <mutant/quaternion_evaluator.h>
#include <mutant/meta.h>

using namespace mutant;
using namespace std;

const float CAnimConsts::ANIM_K = 1.25f;

//
CTransformAnimator::CTransformAnimator()
:	mBundle( 0 )
,	mDataType( dGUESS )
,	mInterpolateMethod( iLINEAR )
,	mTimeType( tLOOP )
#if SEPARATE_SPEED
,	mSpeed( 1.0f )
#endif
{
}

CTransformAnimator::CTransformAnimator( eTimeType timeType, eDataType dataType, eInterpolateMethod interpolateMethod )
:	mBundle( 0 )
,	mDataType( dataType )
,	mInterpolateMethod( interpolateMethod )
,	mTimeType( timeType )
#if SEPARATE_SPEED
,	mSpeed( 1.0f )
#endif
{
}

CTransformAnimator::CTransformAnimator( mutant::anim_bundle const& bundle, eTimeType timeType, eDataType dataType, eInterpolateMethod interpolateMethod )
:	mBundle( &bundle )
,	mDataType( dataType )
,	mInterpolateMethod( interpolateMethod )
,	mTimeType( timeType )
#if SEPARATE_SPEED
,	mSpeed( 1.0f )
#endif
{
	if( dataType == dGUESS )
		mDataType = guessDataType();

	createInterpolator();
}

CTransformAnimator::~CTransformAnimator()
{
}

CTransformAnimator::CTransformAnimator(	CTransformAnimator const& c )
{
	*this = c;
}

CTransformAnimator& CTransformAnimator::operator= ( CTransformAnimator const& rhs )
{
	if( &rhs == this )
		return *this;

	mBundle = rhs.mBundle;
	mDataType = rhs.mDataType;
	mInterpolateMethod = rhs.mInterpolateMethod;
	mTimeType = rhs.mTimeType;
#if SEPARATE_SPEED
	mSpeed = rhs.mSpeed;
#endif

	createInterpolator();
	return *this;
}


void CTransformAnimator::createInterpolator()
{
	typedef CTransform::t_vector t_vec;
	typedef CTransform::t_quaternion t_quat;

	typedef knot_data<float,float> t_float_knots;

	typedef compose_access_policy<t_vec,3> t_access_vector;
	typedef compose_access_policy<t_quat,4> t_access_quaternion;

	typedef comp_3to1<CTransform,t_vec,t_quat,t_vec> t_comp_vqv_to_transform;

	typedef comp_3to1<t_vec,float,float,float> t_comp_3_floats_to_vector;
	typedef comp_quaternion_from_euler<t_quat,float,float,float> t_comp_3_floats_to_quaternion;
	typedef comp_3to1<CTransform,t_vec,t_quat,t_vec> t_comp_vec_quat_vec_to_transform;

	if( !mBundle )
		return;

	switch( mInterpolateMethod )
	{
	default:
	case iLINEAR:
		{
			if( mDataType == dPOS_ROT_SCL )
			{
				typedef linear_evaluator<t_vec,t_access_vector> t_vec_eval;
				typedef linear_quaternion_evaluator<t_quat,t_access_quaternion> t_quat_eval;
				typedef composite_evaluator<CTransform,t_comp_vqv_to_transform,t_vec_eval, t_quat_eval, t_vec_eval> t_compose_evaluator;

				ASSERT( mBundle->floatFloat( sTypeNames::VEC_QUAT_VEC ).componentSize() == 10 );

				t_compose_evaluator eval( t_comp_vqv_to_transform(), t_vec_eval(), 0, t_quat_eval(), 3, t_vec_eval(), 7 );

				switch( mTimeType )
				{
				default:
				case tLOOP:
					{
						typedef interpolator1<t_float_knots, t_compose_evaluator, time_algo_cycle> t_interpolator;
						t_interpolator ipol( mBundle->floatFloat( sTypeNames::VEC_QUAT_VEC ), eval );
						mAnimatorImpl.reset( new InterpolatorEmbed<t_interpolator>( ipol ) );
						break;
					}
				case tCONSTANT:
					{
						typedef interpolator1<t_float_knots, t_compose_evaluator, time_algo_constant> t_interpolator;
						t_interpolator ipol( mBundle->floatFloat( sTypeNames::VEC_QUAT_VEC ), eval );
						mAnimatorImpl.reset( new InterpolatorEmbed<t_interpolator>( ipol ) );
						break;
					}
				}
			}

			// it's hermite actually
			if( mDataType == dPOS_ROT_SCL_SEP )
			{
				switch( mTimeType )
				{
				default:
				case tLOOP:
					{
						typedef mutant::meta::hermite_separate_interpolator
						<
							CTransform,
							CTransform::t_vector, CTransform::t_quaternion, CTransform::t_vector,
							float,
							mutant::time_algo_cycle
						>	t_interpolator_type;

						t_interpolator_type::type transfInterp(
							t_interpolator_type::t_position_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Z ) )
							),
							t_interpolator_type::t_rotation_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Z ) )
							),
							t_interpolator_type::t_scale_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_Z ) )
							)
						);

						mAnimatorImpl.reset(
							new InterpolatorEmbed<t_interpolator_type::type>( transfInterp )
						);
						break;
					}
				case tCONSTANT:
					{
						typedef mutant::meta::hermite_separate_interpolator
						<
							CTransform,
							CTransform::t_vector, CTransform::t_quaternion, CTransform::t_vector,
							float,
							mutant::time_algo_constant
						>	t_interpolator_type;

						t_interpolator_type::type transfInterp(
							t_interpolator_type::t_position_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Z ) )
							),
							t_interpolator_type::t_rotation_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Z ) )
							),
							t_interpolator_type::t_scale_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::SCALE_Z ) )
							)
						);

						mAnimatorImpl.reset(
							new InterpolatorEmbed<t_interpolator_type::type>( transfInterp )
						);
						break;
					}
				}
			}

			// it's hermite actually
			if( mDataType == dPOS_ROT_SEP )
			{
				switch( mTimeType )
				{
				default:
				case tLOOP:
					{
						typedef mutant::meta::hermite_separate_interpolator
						<
							CTransform,
							CTransform::t_vector, CTransform::t_quaternion, CTransform::t_vector,
							float,
							mutant::time_algo_cycle
						>	t_interpolator_type;

						typedef mutant::constant_interpolator<CTransform::t_vector>
							t_constant_scale_interpolator;

						typedef t_interpolator_type::custom_composites
							<
								t_interpolator_type::t_position_interpolator,
								t_interpolator_type::t_rotation_interpolator,
								t_constant_scale_interpolator
							>::type t_transform_interpolator;

						t_transform_interpolator transfInterp(
							t_interpolator_type::t_position_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Z ) )
							),
							t_interpolator_type::t_rotation_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Z ) )
							),
							t_constant_scale_interpolator( CTransform::identityScale() )
						);

						mAnimatorImpl.reset(
							new InterpolatorEmbed<t_transform_interpolator>( transfInterp )
						);
						break;
					}
				case tCONSTANT:
					{
						typedef mutant::meta::hermite_separate_interpolator
						<
							CTransform,
							CTransform::t_vector, CTransform::t_quaternion, CTransform::t_vector,
							float,
							mutant::time_algo_constant
						>	t_interpolator_type;

						typedef mutant::constant_interpolator<CTransform::t_vector>
							t_constant_scale_interpolator;

						typedef t_interpolator_type::custom_composites
							<
								t_interpolator_type::t_position_interpolator,
								t_interpolator_type::t_rotation_interpolator,
								t_constant_scale_interpolator
							>::type t_transform_interpolator;

						t_transform_interpolator transfInterp(
							t_interpolator_type::t_position_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::TRANSLATE_Z ) )
							),
							t_interpolator_type::t_rotation_interpolator(
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_X ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Y ) ),
								t_interpolator_type::t_hermite_interpolator( mBundle->floatFloat( mutant::sTypeNames::ROTATE_Z ) )
							),
							t_constant_scale_interpolator( CTransform::identityScale() )
						);

						mAnimatorImpl.reset(
							new InterpolatorEmbed<t_transform_interpolator>( transfInterp )
						);
						break;
					}
				}
			}

			break;
		}
	case iHERMITE:
		{
			ASSERT( "Hermite interpolator not implemented yet!" );
			break;
		}

	}
}

CTransformAnimator::eDataType CTransformAnimator::guessDataType()
{
	if( mBundle )
	{
		if( mBundle->has_ff( sTypeNames::VEC_QUAT_VEC ) )
			return dPOS_ROT_SCL;
		else if( mBundle->has_ff( sTypeNames::VEC_QUAT ) )
			return dPOS_ROT;
		else if(
				mBundle->size_ff() >= 9 &&
				mBundle->has_ff( sTypeNames::TRANSLATE_X ) &&
				mBundle->has_ff( sTypeNames::TRANSLATE_Y ) &&
				mBundle->has_ff( sTypeNames::TRANSLATE_Z ) &&
				mBundle->has_ff( sTypeNames::ROTATE_X ) &&
				mBundle->has_ff( sTypeNames::ROTATE_Y ) &&
				mBundle->has_ff( sTypeNames::ROTATE_Z ) &&
				mBundle->has_ff( sTypeNames::SCALE_X ) &&
				mBundle->has_ff( sTypeNames::SCALE_Y ) &&
				mBundle->has_ff( sTypeNames::SCALE_Z ) )
			return dPOS_ROT_SCL_SEP;
		else if(
				mBundle->size_ff() >= 6 &&
				mBundle->has_ff( sTypeNames::TRANSLATE_X ) &&
				mBundle->has_ff( sTypeNames::TRANSLATE_Y ) &&
				mBundle->has_ff( sTypeNames::TRANSLATE_Z ) &&
				mBundle->has_ff( sTypeNames::ROTATE_X ) &&
				mBundle->has_ff( sTypeNames::ROTATE_Y ) &&
				mBundle->has_ff( sTypeNames::ROTATE_Z ) )
			return dPOS_ROT_SEP;
	}

	return dGUESS;
}

void CTransformAnimator::setType( eInterpolateMethod interpolateMethod )
{
	mInterpolateMethod = interpolateMethod;
	createInterpolator();
}

void CTransformAnimator::setTimeType( eTimeType timeType )
{
	mTimeType = timeType;
	createInterpolator();
}

void CTransformAnimator::setSource( eDataType dataType, mutant::anim_bundle const& bundle, eInterpolateMethod interpolateMethod )
{
	mBundle = &bundle;
	mDataType = dataType;
	mInterpolateMethod = interpolateMethod;
	createInterpolator();
}

void CTransformAnimator::setSource( eDataType dataType, mutant::anim_bundle const& bundle )
{
	mBundle = &bundle;
	mDataType = dataType;
	createInterpolator();
}

///
CTransformArrayAnimator::CTransformArrayAnimator()
{
}

unsigned CTransformArrayAnimator::createFromClip(
	mutant::anim_clip const& clip,
	mutant::anim_hierarchy const& hier,
	bool create_looping_animators,
	eCreationPolicy creationPolicy,
	bool identity_animator_for_non_existen_bundle
	)
{
	typedef mutant::anim_hierarchy::node_cit_t t_hit;
	typedef mutant::anim_clip::iterator_t t_cit;

	unsigned created = 0;

	timeController().set_length( clip.clip_length() );
	timeController().set_mode( create_looping_animators ? time_controller<>::time_loop : time_controller<>::time_constant );

//	CTransformAnimator::eTimeType timeType = create_looping_animators ? CTransformAnimator::tLOOP : CTransformAnimator::tCONSTANT;
	CTransformAnimator::eTimeType timeType = CTransformAnimator::tCONSTANT;

	switch( creationPolicy )
	{
	default:
	case WHOLE_CLIP: {
		for( t_cit it = clip.iterate(); it; ++it )
		{
			mutant::anim_bundle const& bundle = *it->second;
			addAnimator( CTransformAnimator( bundle ) );
			created++;
		}
		break;
		}
	case FILTER_HIERARCHY: {
		for( t_hit it = hier.iterate(); it; ++it )
		{
			if( clip.has( it->name ) ) {
				addAnimator( CTransformAnimator( clip[ it->name ], timeType ) );
				created++;
			} else if( identity_animator_for_non_existen_bundle ) {
				addAnimator( CTransformAnimator( timeType ) );
				created++;
			}
		}
		break;
		}
	}

	return created;
}
