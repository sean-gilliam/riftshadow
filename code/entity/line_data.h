#ifndef ENTITY_LINE_DATA_H
#define ENTITY_LINE_DATA_H

#include "fwd.h"

//
// One line of a mob speech: what to say (text), how (type), after how long
// (delay), and its ordering number. Owns its `text` string. A speech holds
// these by value in a std::list<LINE_DATA> (speech->first_line).
//
// Immovable/uncopyable: the lists that hold these only ever emplace, erase,
// iterate, and sort (all of which relink nodes rather than copy/move the
// elements), so a plain owning dtor is all that's needed -- no rule-of-5.
// Keeping it immovable also guarantees element addresses are stable, which the
// runtime speech cursor (an iterator into first_line) relies on.
//
struct line_data
{
	int number = -1;					// Ordering index within the speech.
	int delay = -1;						// Ticks to wait before this line.
	short type = -1;					// SPEECH_SAY / TELL / ... (speech_table).
	char *text = nullptr;				// What to say (owned).

	line_data() = default;
	~line_data();

	line_data(const line_data &) = delete;
	line_data &operator=(const line_data &) = delete;
	line_data(line_data &&) = delete;
	line_data &operator=(line_data &&) = delete;
};

#endif /* ENTITY_LINE_DATA_H */
