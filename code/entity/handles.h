#ifndef ENTITY_HANDLES_H
#define ENTITY_HANDLES_H

#include "fwd.h"
#include "../stdlibs/handle.h"

//
// The two entity slot maps, and the shorthand for reading through a handle.
//
// These sit beside char_list and object_list -- the same entities, indexed a
// second way. Registration happens in new_char/new_obj and retirement in
// free_char/free_obj, so an entity is registered for exactly the span during
// which it is not on a free list. Nothing else should Add or Remove.
//
// An entity that was never handed out by new_char/new_obj -- one on the stack
// in a test, say -- is simply not registered, and its `self` handle stays
// null. Deref of a null handle is null, so unregistered entities are safe by
// default rather than dangerous by default.
//
// CACHING A Deref RESULT
//
// Deref returns a raw pointer, and that pointer is only good until something
// can free the entity. Reading the same handle several times in a row looks
// redundant and is a standing temptation to hoist into one local -- but in
// combat and prog code it is load-bearing, because damage_new, multi_hit,
// one_hit, track_attack and anything that moves a character can end a fight or
// kill outright, partway through the very block doing the reads. A cached
// CHAR_DATA * survives that; the handle does not, which is the entire point of
// it being a handle.
//
// The rule, and it is worth stating because the two cases look identical:
//
//   * Read once into a local ONLY when nothing between the read and its last
//     use can invalidate the entity -- display code, predicate code, pure
//     field assignment.
//   * Otherwise re-read through the handle at each use. A Deref is an array
//     index and an integer compare; a stale pointer is a use-after-free.
//
// Sites that follow the second rule are commented where they sit. Do not
// "simplify" them without checking what the calls in between can do.
//

extern SlotMap<CHAR_DATA> charHandles;
extern SlotMap<OBJ_DATA> objectHandles;
extern SlotMap<DESCRIPTOR_DATA> descriptorHandles;

/// The character this handle names, or null if it has been extracted.
inline CHAR_DATA *Deref(Handle<CHAR_DATA> handle)
{
	return charHandles.Deref(handle);
}

/// The object this handle names, or null if it has been extracted.
inline OBJ_DATA *Deref(Handle<OBJ_DATA> handle)
{
	return objectHandles.Deref(handle);
}

/// The connection this handle names, or null if the socket has been closed.
inline DESCRIPTOR_DATA *Deref(Handle<DESCRIPTOR_DATA> handle)
{
	return descriptorHandles.Deref(handle);
}

#endif /* ENTITY_HANDLES_H */
