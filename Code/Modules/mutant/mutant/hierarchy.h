#ifndef MUTANT_HIERARCHY_H_
#define MUTANT_HIERARCHY_H_

namespace mutant
{
	//
	template<class NodeT>
	struct hierarchy
	{
		enum remove_policy
		{
			child_discard = 0,
			child_reparent
		};

		//
		struct node
		{
			typedef mutant::iterator<std::vector<int> > children_it_t;
			// ReJ: added const iterator
			typedef mutant::const_iterator<std::vector<int> > children_const_it_t;

			enum { nparent = -1 };

			node() {}
			node( std::string const& n ) : name( n ) {}

			children_it_t iterate() { return children_it_t( children.begin(), children.end() ); }
			children_const_it_t iterate() const { return children_const_it_t( children.begin(), children.end() ); }
			unsigned child_count() const { return (unsigned)children.size(); }
			int operator[]( unsigned index ) const { return children[index]; }
			void add_child( int index ) { children.push_back( index ); }

			template<class OutputStreamT, class HierarchyT>
			void info( OutputStreamT& os, HierarchyT& hier, std::string const& pre, int verbose ) {
				os	<< pre << name << ", "
					<< (int)children.size() << " child(s)"
					<< std::endl;
			}

			std::string			name;
			std::vector<int>	children;
		};

		typedef NodeT node_t;
		typedef std::vector<node_t> nodes_t;

		size_t size() const { return mNodes.size(); }

		void push_back( node_t const& node ) {
			mNodes.push_back( node );
		}

		void remove_node( std::string const& node_name, remove_policy rp = child_discard ) {
			int nnode = index_by_name( node_name );
			if( nnode == -1 )
				return;

			remove_node( nnode, rp );
		}

		void remove_node( int nodeId, remove_policy rp = child_discard )
		{
			assert( nodeId >= 0 && nodeId < (int)mNodes.size() );
			assert( rp == child_discard );

			int cnt = countChildren( nodeId ) + 1;
			mNodes.erase( mNodes.begin() + nodeId, mNodes.begin() + nodeId + cnt );

			int sub = cnt;
			for( int in = 0; in<(int)mNodes.size(); ++in ) {
				std::vector<int>& cc = mNodes[ in ].children;
				for( int ic = 0; ic<(int)cc.size(); ) {
					if( cc[ ic ] == nodeId ) {
						cc.erase( cc.begin() + ic );
						continue;
					}

					if( cc[ ic ] >= nodeId )
						cc[ ic ] -= sub;

					++ic;
				}
			}
		}

		int countChildren( int nodeId ) {
			node_t& node = mNodes[ nodeId ];
			int cnt = (int)node.children.size();
			for( unsigned in = 0; in<node.children.size(); ++in )
				cnt += countChildren( node.children[ in ] );
			return cnt;
		}

		node_t& root() { return mNodes[0]; }
		node_t const& root() const { return mNodes[0]; }

		node_t& operator[]( int n ) { return mNodes[ n ]; }
		node_t const& operator[]( int n ) const { return mNodes[ n ]; }
		node_t& operator[]( std::string const& str ) {
			int in = index_by_name( str );
			if( in == node_t::nparent )
				mutant_throw( std::string("Node `") + str + "' does not exist" );

			return mNodes[ in ];
		}
		node_t const& operator[]( std::string const& str ) const {
			int in = index_by_name( str );
			if( in == node_t::nparent )
				mutant_throw( std::string("Node `") + str + "' does not exist" );

			return mNodes[ in ];
		}

		nodes_t& nodes() { return mNodes; }
		nodes_t const& nodes() const { return mNodes; }

		std::string const& name() const { return mName; }
		void setName( std::string const& n ) { mName = n; }

		typedef mutant::iterator<nodes_t> node_it_t;
		typedef mutant::const_iterator<nodes_t> node_cit_t;
		node_it_t iterate() { return node_it_t( mNodes.begin(), mNodes.end() ); }
		node_cit_t iterate() const { return node_cit_t( mNodes.begin(), mNodes.end() ); }

		int index_by_name( std::string const& node_name ) const {
			for( unsigned i = 0; i < (unsigned)size(); ++i )
			{
				if( mNodes[ i ].name == node_name )
					return i;
			}

			return node_t::nparent;
		}

		unsigned calc_children( unsigned nodeToStartAt ) const
		{
			assert( nodeToStartAt < mNodes.size() );

			node_t const& nod = mNodes[ nodeToStartAt ];

			unsigned childCount = nod.child_count();
			for( unsigned i = 0; i<nod.child_count(); ++i )
			{
				childCount += calc_children( nod.children[i] );
			}

			return childCount;
		}

		template<class OutputStreamT>
		void info( OutputStreamT& os, std::string const& pre, int verbose )
		{
			os << pre << "Hierarchy `" << mName << "' has " << (int)mNodes.size() << " node(s)" << std::endl;

			if( verbose >= 5 )
			{
//				info_tree( os, root(), pre, verbose );

				// hack: assumes node_t has `parent' index
				for( unsigned i = 0; i<mNodes.size(); ++i )
				{
					if( mNodes[ i ].parent == node_t::nparent )
						info_tree( os, mNodes[ i ], pre, verbose );
				}
			}
		}

		template<class OutputStreamT>
		void info_tree( OutputStreamT& os, node_t& thisNode, std::string const& pre, int verbose )
		{
			thisNode.info( os, *this, pre, verbose );
//			os << pre << thisNode.name << ", " << (int)thisNode.children.size() << " child(s)" << std::endl;
			for( unsigned i=0; i<thisNode.children.size(); ++i )
			{
				info_tree( os, mNodes[ thisNode.children[i] ], pre + "-", verbose );
			}
		}

		template<class OutputStreamT>
		void info_plain( OutputStreamT& os, std::string const& pre, int verbose )
		{
			os << pre << "Hierarchy `" << mName << "' has " << (int)mNodes.size() << " node(s)" << std::endl;

			if( verbose >= 5 )
			{
				for( unsigned i=0; i<mNodes.size(); ++i )
					mNodes[ i ].info( os, *this, pre, verbose );
			}
		}
		
	private:
		std::string	mName;
		nodes_t		mNodes;
	};

	//
	struct anim_node : public hierarchy<anim_node>::node
	{
		typedef hierarchy<anim_node>::node base_t;
		typedef hierarchy<anim_node> anim_hierarchy;

		anim_node() {}
		anim_node( std::string const& n ) : base_t( n ), parent( base_t::nparent ) {}
		anim_node( std::string const& n, int p ) : base_t( n ), parent( p ) {}

		static void update_parent_ids( anim_hierarchy& hier, int thisNodeId, int parent = nparent )
		{
			anim_node& thisNode = hier[ thisNodeId ];
			thisNode.parent = parent;

			for( anim_node::children_it_t it = thisNode.iterate(); it; ++it )
				update_parent_ids( hier, *it, thisNodeId );
		}

		static void update_parent_all( anim_hierarchy& hier )
		{
			unsigned ii = 0;
			while( ii < hier.size() )
			{
				update_parent_ids( hier, ii, anim_node::nparent );
				unsigned processed = hier.calc_children( 0 ) + 1;
				ii += processed;
			}
		}

		template<class OutputStreamT, class HierarchyT>
		void info( OutputStreamT& os, HierarchyT& hier, std::string const& pre, int verbose ) {
			os	<< pre << name << ", "
				<< (int)children.size() << " child(s), parent: "
				<< parent << " `" << (parent == -1 ? "none" : hier[ parent ].name) << "'"
				<< std::endl;
		}

		int		parent;
	};

	typedef hierarchy<anim_node> anim_hierarchy;
}

#endif // MUTANT_HIERARCHY_H_
