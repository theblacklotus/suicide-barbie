#ifndef __ANIMATOR_ALGOS_H_
#define __ANIMATOR_ALGOS_H_

////////////////////////////////////////////////
class CAnimatorAlgos
{
public:
	// multiplies matrices by hierarchy from node-space to skeleton-space, postmultiplying with rootTransform
	template<typename OutputIteratorT, typename RandomAccessItT>
	static unsigned transformHierarchy( OutputIteratorT outIt, OutputIteratorT outItEnd, RandomAccessItT rootToTransformIt, mutant::anim_hierarchy const& hier, CTransform const& rootTransform )
	{
		typedef mutant::anim_hierarchy::node_cit_t it_t;

		RandomAccessItT inIt = rootToTransformIt;
		OutputIteratorT outRootIt = outIt;

		for( it_t it = hier.iterate(); it; ++it )
		{
			if( !(outIt < outItEnd) )
				break;
			
			if( it->parent != mutant::anim_node::nparent )
//				*outIt = *inIt * *( outRootIt + it->parent );
				*outIt = *( outRootIt + it->parent ) * *inIt;
			else
				*outIt = rootTransform * *inIt;
//				*outIt = *inIt * rootTransform;

			++outIt;
			++inIt;
		}

		return unsigned(outIt - outRootIt);
	}

	// multiplies matrices by hierarchy from node-space to skeleton-space
	template<typename OutputIteratorT, typename RandomAccessItT>
	static unsigned transformHierarchy( OutputIteratorT outIt, OutputIteratorT outItEnd, RandomAccessItT rootToTransformIt, mutant::anim_hierarchy const& hier )
	{
		typedef mutant::anim_hierarchy::node_cit_t it_t;

		RandomAccessItT inIt = rootToTransformIt;
		OutputIteratorT outRootIt = outIt;

		for( it_t it = hier.iterate(); it; ++it )
		{
			if( !(outIt < outItEnd) )
				break;
			
			if( it->parent != mutant::anim_node::nparent )
				*outIt = *inIt * *( outRootIt + it->parent );
			else
				*outIt = *inIt;

			++outIt;
			++inIt;
		}

		return unsigned(outIt - outRootIt);
	}

	// multiplies every transform in [rootBeginIt...rootEndIt) by rootTransform
	template<typename OutputIteratorT, typename InputIteratorT>
	static void transformFlattened(
		OutputIteratorT outIt,
		InputIteratorT beginIt,
		InputIteratorT endIt,
		CTransform const& rootTransform )
	{
		for( ; beginIt != endIt; ++beginIt )
		{
			*outIt = *beginIt * rootTransform;
			++outIt;
		}
	}

	// just copies objects (with assignment operator) [beginIt,endIt) -> [outIt, outIt++)
	template<typename OutputIteratorT, typename InputIteratorT>
	static void assign( OutputIteratorT outIt, InputIteratorT beginIt, InputIteratorT endIt )
	{
		for( ; beginIt != endIt; ++beginIt )
		{
			*outIt = *beginIt;
			++outIt;
		}
	}

/*	///
	struct FTransformToMatrix : public std::unary_function<CTransform::t_matrix,CTransform const&>
	{
		CTransform::t_matrix operator()( CTransform const& arg ) {
			return arg.matrix();
		}
	};

	///
	struct FTransformWorldToMatrix : public std::unary_function<CTransform::t_matrix,CTransform const&>
	{
		FTransformWorldToMatrix( CTransform const& t )
			:	mWorldTransform( t ) { }

		CTransform::t_matrix operator()( CTransform const& arg ) {
			return (arg * mWorldTransform).matrix();
		}

		CTransform	mWorldTransform;
	};

	///
	struct FTransformWorldByBoneToMatrix : public std::unary_function<CTransform::t_matrix,CTransform const&>
	{
		FTransformWorldByBoneToMatrix( CTransform const& t )
			:	mWorldTransform( t ) { }

		CTransform::t_matrix operator()( CTransform const& localTransform, CTransform const& boneTransform ) {
			return (boneTransform * localTransform * mWorldTransform).matrix();
		}

		CTransform	mWorldTransform;
	};*/
};

#endif
