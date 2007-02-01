#ifndef NEWAGE_ANIMATOR_ALGOS_H_
#define NEWAGE_ANIMATOR_ALGOS_H_

////////////////////////////////////////////////
struct CAnimatorAlgos
{
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
				*outIt = *inIt * *( outRootIt + it->parent );
			else
				*outIt = *inIt * rootTransform;

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
//				*outIt = *inIt * *( outRootIt + it->parent );
				*outIt = *( outRootIt + it->parent ) * *inIt;
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
};

#endif // NEWAGE_ANIMATOR_ALGOS_H_
