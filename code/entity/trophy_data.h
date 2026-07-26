#ifndef ENTITY_TROPHY_DATA_H
#define ENTITY_TROPHY_DATA_H

#include "fwd.h"

//
// One scalp on a Horde trophy belt: the name of a character the owner trophied.
// Owns its `victname` string (palloc_string / free_pstring). A player holds
// these by value in a std::list<TROPHY_DATA> (pcdata->trophy).
//
// std::list matches the intrusive list it replaces; the trophy list is tiny
// (bounded by belt scalps) and nothing caches an element pointer across a
// mutation, so a vector would have been safe too -- list is chosen for
// consistency with the other intrusive-list conversions. The rule-of-5 below
// keeps copies deep and moves cheap; bodies live in recycle.c by the memory
// helpers they call.
//
struct trophy_data
{
	char *victname = nullptr;			// Name of the trophied character.

	trophy_data() = default;
	explicit trophy_data(const char *name);
	~trophy_data();

	trophy_data(const trophy_data &other);
	trophy_data &operator=(const trophy_data &other);
	trophy_data(trophy_data &&other) noexcept;
	trophy_data &operator=(trophy_data &&other) noexcept;
};

#endif /* ENTITY_TROPHY_DATA_H */
