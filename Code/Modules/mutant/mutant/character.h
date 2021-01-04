#ifndef MUTANT_CHARACTER_H_
#define MUTANT_CHARACTER_H_

#include "hierarchy.h"

#include <set>
#include <memory>

namespace mutant
{
	// holds all the clips loaded from file
	struct anim_character
	{
		typedef std::pair<std::string,anim_clip*> clip_pair_t;
		typedef std::vector<clip_pair_t> clip_vector_t;
		typedef mutant::iterator<clip_vector_t> clips_it_t;
		typedef mutant::const_iterator<clip_vector_t> clips_cit_t;

		//
		struct pair_first_less : public std::binary_function<clip_pair_t,clip_pair_t,bool> {
			bool operator()( clip_pair_t const& a, clip_pair_t const& b ) const {
				return a.first < b.first;
			}
		};

		//
		struct hierarchy_compare {
			bool operator()( anim_hierarchy* a, anim_hierarchy* b ) const {
				return a->name() < b->name();
			}
		};
		typedef std::set<anim_hierarchy*,hierarchy_compare> hierarchy_set_t;

		//
		struct hierarchy_find {
			hierarchy_find( std::string const& n ) : name( n ) {}
			bool operator()( anim_hierarchy* a ) const {
				return a->name() == name;
			}
		private:
			std::string name;
		};

		anim_character() {
		}

		~anim_character()
		{
			std::for_each(
				mAnimClips.begin(),
				mAnimClips.end(),
				mutant::compose_f_gx(
					mutant::deleter<anim_clip*>(),
					mutant::sel2nd<clip_pair_t>()
				)
			);
		}

		// clips
		void insertClip( std::string const& clip_name, std::auto_ptr<anim_clip> clip ) {
			mAnimClips.push_back(
				std::make_pair( clip_name, clip.release() )
			);
		}

		anim_clip& operator[]( std::string const& name ) const {
			clip_vector_t::const_iterator it = findClip( name );
			if( it == mAnimClips.end() )
				mutant_throw( std::string("Cannot find '") + name + "' clip" );

			return *it->second;
		}

		anim_clip& operator[]( unsigned int index ) const {
			assert( index < mAnimClips.size() );
			return *mAnimClips[ index ].second;
		}

		clip_pair_t const& clipPair( unsigned int index ) {
			assert( index < mAnimClips.size() );
			return mAnimClips[ index ];
		}

		std::auto_ptr<anim_clip> remove( std::string const& name ) {
			clip_vector_t::iterator it = findClip( name );
			anim_clip* clip = 0;
			if( it != mAnimClips.end() ) {
				clip = it->second;
				mAnimClips.erase( it );
			}
			return std::auto_ptr<anim_clip>( clip );
		}

		size_t size() const { return mAnimClips.size(); }

		bool has( std::string const& clip_name ) const {
			return findClip( clip_name ) != mAnimClips.end();
		}

		void sort_clips_by_name()
		{
			std::sort( mAnimClips.begin(), mAnimClips.end(), pair_first_less() );
		}

		clips_it_t iterate() { return clips_it_t( mAnimClips.begin(), mAnimClips.end() ); }
		clips_cit_t iterate() const { return clips_cit_t( mAnimClips.begin(), mAnimClips.end() ); }

		// hierarchies
		void insert_hierarchy( std::auto_ptr<anim_hierarchy> hier ) {
			mHierarchies.insert( hier.release() );
		}

		anim_hierarchy& hierarchy( std::string const& name ) const {
			hierarchy_set_t::const_iterator it = findHierarchy( name );
			if( it == mHierarchies.end() )
				mutant_throw( std::string("Cannot find '") + name + "' hierarchy" );
 
			return **it;
		}

		std::auto_ptr<anim_hierarchy> remove_hierarchy( std::string const& hier_name ) {
			hierarchy_set_t::iterator it = findHierarchy( hier_name );
			anim_hierarchy* hier = 0;
			if( it != mHierarchies.end() ) {
				hier = *it;
				mHierarchies.erase( it );
			}
			return std::auto_ptr<anim_hierarchy>( hier );
		}

		size_t size_hierarchy() const { return mHierarchies.size(); }

		bool has_hierarchy( std::string const& hier_name ) const {
			return findHierarchy( hier_name ) != mHierarchies.end();
		}

		// info
		typedef mutant::iterator<hierarchy_set_t> hierarchy_it_t;
		hierarchy_it_t iterate_hierarchies() { return hierarchy_it_t( mHierarchies.begin(), mHierarchies.end() ); }

		void info( std::ostream& os, std::string const& pre, int verbose ) {
			os << (int)mHierarchies.size() << " hierarchies" << std::endl;
			if( verbose >= 5 ) {
				for( hierarchy_it_t it = iterate_hierarchies(); it; ++it )
				{
					os << "Hierarchy `" << (*it)->name() << "'" << std::endl;
					(*it)->info( os, pre, verbose );
				}
			}

			os << (int)mAnimClips.size() << " animation clips" << std::endl;
			if( verbose >= 5 ) {
				for( clips_it_t it = iterate(); it; ++it )
				{
					os << "Clip `" << it->first << "'" << std::endl;
					it->second->info( os, pre, verbose );
				}
			}
		}

	protected:
		clip_vector_t::iterator findClip( std::string const& name ) {
			return std::find_if(
				mAnimClips.begin(),
				mAnimClips.end(),
				mutant::compose_f_gx(
					std::bind2nd( std::equal_to<std::string>(), name ),
					sel1st<clip_pair_t>()
				)
			);
		}

		clip_vector_t::const_iterator findClip( std::string const& name ) const {
			return std::find_if(
				mAnimClips.begin(),
				mAnimClips.end(),
				mutant::compose_f_gx(
					std::bind2nd( std::equal_to<std::string>(), name ),
					sel1st<clip_pair_t>()
				)
			);
		}

		hierarchy_set_t::iterator findHierarchy( std::string const& hier_name ) {
			return std::find_if(
				mHierarchies.begin(),
				mHierarchies.end(),
				hierarchy_find( hier_name )
			);
		}

		hierarchy_set_t::const_iterator findHierarchy( std::string const& hier_name ) const {
			return std::find_if(
				mHierarchies.begin(),
				mHierarchies.end(),
				hierarchy_find( hier_name )
			);
		}

	private:
		anim_character( anim_character const& );
		anim_character const& operator=( anim_character const& );

	private:
		clip_vector_t	mAnimClips;
		hierarchy_set_t	mHierarchies;
	};
}

#endif // MUTANT_CHARACTER_H_
