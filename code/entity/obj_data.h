#ifndef ENTITY_OBJ_DATA_H
#define ENTITY_OBJ_DATA_H

#include <list>

#include "fwd.h"
#include "limits.h"
#include "extra_descr.h"
#include "affect_data.h"		// OBJ_APPLY_DATA held by value
#include "../stdlibs/handle.h"	// self, and the handle-typed cross-references

//
// One object.
//

struct obj_data
{
	// A handle naming this object, issued by new_obj and retired by free_obj.
	// It is how another entity refers to this one without risking a pointer
	// into a recycled slot; null for an object that never came from new_obj.
	Handle<OBJ_DATA> self;
	OBJ_DATA *next;
	OBJ_DATA *next_content;
	OBJ_DATA *contains;
	OBJ_DATA *in_obj;
	OBJ_DATA *on;
	CHAR_DATA *carried_by;
	std::list<EXTRA_DESCR_DATA> extra_descr;
	std::list<OBJ_APPLY_DATA> apply;
	std::list<OBJ_AFFECT_DATA> affected;
	std::list<AFFECT_DATA> charaffs;
	OBJ_INDEX_DATA *pIndexData;
	ROOM_INDEX_DATA *in_room;
	RUNE_DATA *rune;
	bool has_rune;
	bool valid;
	char *talked;
	char *owner;
	char *name;
	char *short_descr;
	char *description;
	short item_type;
	long extra_flags[MAX_BITVECTOR];
	long wear_flags[MAX_BITVECTOR];
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	long affected_by[MAX_BITVECTOR];
	short wear_loc;
	short weight;
	int cost;
	short level;
	short condition;
	char *material;
	short timer;
	int value[5];
	long progtypes[MAX_BITVECTOR];
	int ohp;
	int mob_vnum;
	bool moved;
	char *wear_loc_name;
};

#endif /* ENTITY_OBJ_DATA_H */
