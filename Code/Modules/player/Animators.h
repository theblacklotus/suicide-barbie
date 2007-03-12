#ifndef NEWAGE_ANIMATORS_H_
#define NEWAGE_ANIMATORS_H_

#include "cfg.h"

#include <memory>
#include <vector>
#include <mutant/bundle.h>
#include <mutant/clip.h>
#include <mutant/hierarchy.h>
#include <mutant/type_names.h>
#include <mutant/time_algo.h>

#include "Transform.h"

#define SEPARATE_SPEED 0

////////////////////////////////////////////////
class CAnimConsts
{
public:
	static float animK() { return ANIM_K; }

private:
	static const float ANIM_K;
};

////////////////////////////////////////////////
class CTransformAnimator
{
	////////////////////////////////////////////////
	class AnimatorImpl
	{
	public:
		virtual ~AnimatorImpl() /*= 0*/ {}
		virtual CTransform value( float t ) = 0;
	};

	typedef std::auto_ptr<AnimatorImpl> t_anim_ptr;

	////////////////////////////////////////////////
	template<class InterpolatorT>
	class InterpolatorEmbed : public AnimatorImpl
	{
	public:
		InterpolatorEmbed( InterpolatorT const& i )
		:	mInterpolator( i )
		{
		}

		virtual CTransform value( float t )
		{
			return mInterpolator.value( t );
		}

	private:
		InterpolatorT	mInterpolator;
	};

public:
	enum eInterpolateMethod
	{
		iLINEAR,
		iHERMITE // (not supported yet)
	};

	enum eTimeType
	{
		tCONSTANT,
		tLOOP
	};

	enum eDataType
	{
		dGUESS,

		// anim curve must be supplied with 10 floats per component:
		//	position(translation) vector; 3 x float
		//	rotation quaternion; 4 x float
		//	scale vector; 3 x float
		dPOS_ROT_SCL,

		// anim curve must be supplied with 7 floats per component:
		//	position(translation) vector; 3 x float
		//	rotation quaternion; 4 x float
		dPOS_ROT,

		// 9 curves, each separate.
		//  position 3 x float anim curve
		//  rotation 3 x float anim curve (yaw/pitch/roll)
		//  scale 3 x float anim curve
		dPOS_ROT_SCL_SEP,

		// 6 curves, each separate.
		//  position 3 x float anim curve
		//  rotation 3 x float anim curve
		dPOS_ROT_SEP,
	};

	CTransformAnimator();
	CTransformAnimator( eTimeType timeType, eDataType dataType = dGUESS, eInterpolateMethod interpolateMethod = iLINEAR );
	CTransformAnimator( mutant::anim_bundle const& bundle, eTimeType timeType = tLOOP, eDataType dataType = dGUESS, eInterpolateMethod interpolateMethod = iLINEAR );
	~CTransformAnimator();
	
	void setTimeType( eTimeType timeType );
	void setType( eInterpolateMethod interpolateMethod );
	void setSource( eDataType dataType, mutant::anim_bundle const& bundle, eInterpolateMethod interpolateMethod );
	void setSource( eDataType dataType, mutant::anim_bundle const& bundle );
#if SEPARATE_SPEED
	void setSpeed( float speed ) { mSpeed = speed; }
#endif

	CTransform value( float t ) const {
		if( mAnimatorImpl.get() )
#if SEPARATE_SPEED
			return mAnimatorImpl->value( t * mSpeed );
#else
			return mAnimatorImpl->value( t );
#endif
		else
			return CTransform::identity();
	}

	CTransformAnimator(	CTransformAnimator const& c );
	CTransformAnimator& operator=( CTransformAnimator const& rhs );


protected:
	eDataType guessDataType();
	void createInterpolator();

private:
	t_anim_ptr					mAnimatorImpl;
	mutant::anim_bundle const*	mBundle;
	eDataType					mDataType;
	eInterpolateMethod			mInterpolateMethod;
	eTimeType					mTimeType;
#if SEPARATE_SPEED
	float						mSpeed;
#endif
};

////////////////////////////////////////////////
class CTransformArrayAnimator
{
public:
	typedef std::vector<CTransformAnimator> t_animators;

	enum eCreationPolicy
	{
		WHOLE_CLIP,				// creates animators for all nodes that exist in clip
		FILTER_HIERARCHY		// creates animators only for those bundles, that exist in hierarchy
	};

	CTransformArrayAnimator();

	void addAnimator( CTransformAnimator const& animator )
	{
		mAnimators.push_back( animator );
	}

	void clear() { mAnimators.clear(); }
	size_t size() const { return mAnimators.size(); }
	void setSpeed( float speed ) { timeController().set_speed( speed ); }

	mutant::time_controller<>& timeController() { return mTimeController; }
	mutant::time_controller<> const& timeController() const { return mTimeController; }

	//
	unsigned createFromClip(
		mutant::anim_clip const& clip,
		mutant::anim_hierarchy const& hier,
		bool create_looping_animators = true,
		eCreationPolicy creationPolicy = FILTER_HIERARCHY,
		bool identity_animator_for_non_existen_bundle = true
		);

	// Updates matrices begining at supplied iterator 'dest'
	// Number of updated matrices equals to minimum of animator count and supplied array size
	template<typename ItT>
	void updateTransforms( float t, ItT dest, ItT destEnd )
	{
		t_animators::iterator aIt = mAnimators.begin();
		t_animators::iterator aEnd = mAnimators.end();

		mTimeController.set_time( t );

		while( aIt != aEnd )
		{
			if( !(dest < destEnd) )
				break;

			*dest = aIt->value( mTimeController.processed_time() );

			++dest;
			++aIt;
		}
	}

private:
	t_animators					mAnimators;
	mutant::time_controller<>	mTimeController;
};

////////////////////////////////////////////////
/*class CGenericAnimator
{
public:
	CGenericAnimator();

	mutant::time_controller<>& timeController() { return mTimeController; }
	mutant::time_controller<> const& timeController() const { return mTimeController; }

	//
	unsigned createFromClip(
		mutant::anim_clip const& clip,
		bool create_looping_animators = true
		);

	// Updates matrices begining at supplied iterator 'dest'
	// Number of updated matrices equals to minimum of animator count and supplied array size
	template<typename ItT>
	void value( float t, ItT dest, ItT destEnd )
	{
		t_animators::iterator aIt = mAnimators.begin();
		t_animators::iterator aEnd = mAnimators.end();

		mTimeController.set_time( t );

		while( aIt != aEnd )
		{
			if( !(dest < destEnd) )
				break;

			*dest = aIt->value( mTimeController.processed_time() );

			++dest;
			++aIt;
		}
	}

private:
	t_animators					mAnimators;
	mutant::time_controller<>	mTimeController;
};*/

#endif // NEWAGE_ANIMATORS_H_
