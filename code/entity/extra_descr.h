#ifndef ENTITY_EXTRA_DESCR_H
#define ENTITY_EXTRA_DESCR_H

#include "fwd.h"

//
// Extra description data for a room or object.
//
// Owns its two strings (palloc_string / free_pstring). Parents hold these by
// value in a std::list<EXTRA_DESCR_DATA>. std::list (not vector) is deliberate:
// OLC's `ed add`/`ed edit` hand string_append() a `char**` INTO an element
// (&ed->description) that must stay valid across a deferred string-editing
// session, and a vector realloc (e.g. a concurrent `ed add` on the same room)
// would dangle it. list keeps node/reference stability, exactly as the intrusive
// list it replaces did (dbsession.h precedent). The rule-of-5 below keeps copies
// deep and moves cheap; bodies live in recycle.c by the memory helpers they call.
//
struct extra_descr_data
{
	char *keyword = nullptr;			// Keyword in look/examine
	char *description = nullptr;		// What to see

	extra_descr_data() = default;
	~extra_descr_data();

	extra_descr_data(const extra_descr_data &other);
	extra_descr_data &operator=(const extra_descr_data &other);
	extra_descr_data(extra_descr_data &&other) noexcept;
	extra_descr_data &operator=(extra_descr_data &&other) noexcept;
};

#endif /* ENTITY_EXTRA_DESCR_H */
