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

extern SlotMap<CHAR_DATA> charHandles;
extern SlotMap<OBJ_DATA> objectHandles;

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

#endif /* ENTITY_HANDLES_H */
