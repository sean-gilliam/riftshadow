#ifndef ENTITY_MEM_DATA_H
#define ENTITY_MEM_DATA_H

#include <ctime>

#include "fwd.h"

//
// A mob's memory of a character it has encountered (who, how it feels about
// them, and when). Plain value type; a char owns these by value in a
// std::list<MEM_DATA> (ch->memory).
//
// The mob-memory mechanic is not wired up yet, but the type is a real part of
// the intended game and is kept for it. std::list matches the other
// intrusive-list conversions; there is no usage to audit for pointer stability,
// and a memory list that ages entries out is exactly the shape that tends to
// hold element references across mutations in this codebase, so list is the
// safe default.
//
struct mem_data
{
	int id = 0;					// pc/mob id of the remembered character
	int reaction = 0;			// how the mob feels about them
	time_t when = 0;			// when it last saw them
};

#endif /* ENTITY_MEM_DATA_H */
