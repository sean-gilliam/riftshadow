#ifndef ENTITY_GEN_DATA_H
#define ENTITY_GEN_DATA_H

#include "fwd.h"
#include "limits.h"

//
// Scratch state for the character-generation skill/group picker: which skills
// and groups have been chosen so far, and what they have cost in points.
//
// A character owns at most one of these (ch->gen_data), and only while it is
// being generated -- hence a nullable unique_ptr on the char rather than an
// inline member. The two arrays are roughly 850 bytes that a fully generated
// character has no use for.
//
// The picker that drives this (parse_gen_groups, skills.c) is commented out
// today; the type is kept for it.
//
struct gen_data
{
	bool skill_chosen[MAX_SKILL] = {};
	bool group_chosen[MAX_GROUP] = {};
	int points_chosen = 0;
};

#endif /* ENTITY_GEN_DATA_H */
