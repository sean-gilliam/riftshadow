#ifndef ENTITY_AFFECT_DATA_H
#define ENTITY_AFFECT_DATA_H

#include "fwd.h"
#include "limits.h"

//
// An affect.
//

struct affect_data
{
	AFFECT_DATA *next;
	CHAR_DATA *owner;
	char *name;
	bool valid;
	short where;
	short type;
	short level;
	short duration;
	short location;
	short modifier;
	short mod_name;
	long bitvector[MAX_BITVECTOR];
	int aftype;
	AFF_FUN *tick_fun;		// goes off every tick that char is affected
	AFF_FUN *pulse_fun;
	AFF_FUN *end_fun;		// when the affect wears off this is called
	short init_duration;
	AFF_FUN *beat_fun;		// goes off every beat
};

//
// A room affect.
//

// A room affect. Plain value type; a room owns these by value in a
// std::list<ROOM_AFFECT_DATA> (room->affected). `owner` is a non-owning
// CHAR_DATA back-reference and stays a raw pointer.
struct room_affect_data
{
	CHAR_DATA *owner = nullptr;
	short where = 0;
	short type = 0;
	short level = 0;
	short duration = 0;
	short location = 0;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	RAFF_FUN *pulse_fun = nullptr;
	RAFF_FUN *tick_fun = nullptr;	// goes off every tick
	RAFF_FUN *end_fun = nullptr;	// when the affect wears off this is called
};

//
// An area affect.
//

// An area affect. Plain value type; an area owns these by value in a
// std::list<AREA_AFFECT_DATA> (area->affected). `owner` is a non-owning
// CHAR_DATA back-reference and stays a raw pointer.
struct area_affect_data
{
	CHAR_DATA *owner = nullptr;
	short where = 0;
	short type = 0;
	short level = 0;
	short duration = 0;
	short location = 0;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	AAFF_FUN *pulse_fun = nullptr;
	AAFF_FUN *tick_fun = nullptr;
	AAFF_FUN *end_fun = nullptr;
};

// An object affect. Plain value type; an object owns these by value in a
// std::list<OBJ_AFFECT_DATA> (obj->affected). `owner` is a non-owning
// CHAR_DATA back-reference and stays a raw pointer.
struct obj_affect_data
{
	CHAR_DATA *owner = nullptr;
	short where = 0;
	short type = 0;
	short level = 0;
	short duration = 0;
	short location = 0;
	short modifier = 0;
	long bitvector[MAX_BITVECTOR] = {};
	int aftype = 0;
	OAFF_FUN *pulse_fun = nullptr;
	OAFF_FUN *tick_fun = nullptr;
	OAFF_FUN *end_fun = nullptr;
};

// A stat apply on an object. Plain value type; parents own these by value in a
// std::list<OBJ_APPLY_DATA> (list, not vector: spell_enchant_weapon holds element
// pointers across a second prepend, so nodes must not move).
struct obj_apply_data
{
	short location = 0;
	short modifier = 0;
	short type = 0;						// For gsns, if relevant.
};

#endif /* ENTITY_AFFECT_DATA_H */
