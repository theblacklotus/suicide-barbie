#ifndef MUTANT_FWD_H_
#define MUTANT_FWD_H_

namespace mutant
{

template<typename ValueT>
struct property;

struct property_sheet;

struct anim_character_set;

struct anim_character;

struct anim_clip;

template<class NodeT> struct hierarchy;
//template<class NodeT> struct hierarchy<NodeT>::node;
struct anim_node;
typedef hierarchy<anim_node> anim_hierarchy;

struct anim_clip;

struct anim_bundle;

template<typename KeyT, typename ValueT>
struct knot_data;

// io
struct mutant_reader;
struct mutant_writer;

}

#endif // MUTANT_FWD_H_
