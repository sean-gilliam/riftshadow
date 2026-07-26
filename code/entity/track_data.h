#ifndef ENTITY_TRACK_DATA_H
#define ENTITY_TRACK_DATA_H

#include "fwd.h"
#include "time_info_data.h"

//
// A single recorded footprint in a room: who passed through, when, and which
// way they went. A room owns a fixed array of these by value (room->tracks;
// only the first MAX_TRACKS slots are used) -- there is no heap allocation and
// no free list; an empty slot is one whose `prey` is null. The member
// initializers below give a default-constructed track the same empty state
// new_track_data() used to hand out (`prey == nullptr`, `direction == -1`).
//
// `prey` is a non-owning back-pointer to a character owned elsewhere, so it
// stays a raw pointer.
//
struct track_data
{
	CHAR_DATA *prey = nullptr;			// Who passed through.
	TIME_INFO_DATA time = {};			// When they passed through.
	short direction = -1;				// Which way they went.
	bool flying = false;				// Are they?
	bool sneaking = false;				// Are they?
	int legs = 0;						// How many?
	bool bleeding = false;				// Are they?
};

#endif /* ENTITY_TRACK_DATA_H */
