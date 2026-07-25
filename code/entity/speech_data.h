#ifndef ENTITY_SPEECH_DATA_H
#define ENTITY_SPEECH_DATA_H

#include <list>

#include "fwd.h"
#include "line_data.h"

//
// A named mob speech: an ordered list of lines the mob plays out, plus a cursor
// (current_line) into that list marking where playback is. A mob prototype owns
// these by value in a std::list<SPEECH_DATA> (mob->speech). Owns its `name`.
//
// std::list at both levels is deliberate: speech playback is driven through the
// event queue, which holds a SPEECH_DATA* and advances current_line across
// queued callbacks (act_comm.c speech_handler). std::list gives the node/address
// stability those held references need; a vector would dangle them.
//
// Immovable/uncopyable: mob->speech only emplaces/erases/iterates, so no
// rule-of-5 is needed -- and immovability keeps line addresses and the
// current_line iterator stable. current_line sits at first_line.end() when
// playback is unstarted/reset (the old raw-pointer null state).
//
struct speech_data
{
	char *name = nullptr;					// Speech keyword (owned).
	std::list<LINE_DATA> first_line;		// The lines, in order.
	std::list<LINE_DATA>::iterator current_line;	// Playback cursor; end() == reset.

	speech_data() : current_line(first_line.end()) {}
	~speech_data();

	speech_data(const speech_data &) = delete;
	speech_data &operator=(const speech_data &) = delete;
	speech_data(speech_data &&) = delete;
	speech_data &operator=(speech_data &&) = delete;
};

#endif /* ENTITY_SPEECH_DATA_H */
