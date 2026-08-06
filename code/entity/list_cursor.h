#ifndef ENTITY_LIST_CURSOR_H
#define ENTITY_LIST_CURSOR_H

#include <algorithm>
#include <vector>

//
// Walking a global entity list while things are being extracted from it.
//
// The hazard: a loop body that extracts something other than the element it was
// handed. Every walk in the game saves its successor before running the body:
//
//     for (ch = char_list; ch != nullptr; ch = ch_next)
//     {
//         ch_next = ch->next;
//         ...                     // may extract ch      -- fine
//         ...                     // may extract ch_next -- not fine
//     }
//
// so extracting the successor leaves the loop holding a pointer to an entity
// that has just been freed. That is worse than reading a stale entity, because
// the free lists are threaded through the very same `next` field the global
// lists use: free_char ends with `ch->next = char_free`, so the saved successor
// stops naming anything on the live list. With an empty free list the walk ends
// early and silently skips every remaining entity; with a non-empty one it
// walks off into the free list and hands the loop body one corpse after
// another. Neither is visible at runtime today, because the memory stays mapped.
//
// close_socket has coped with this by hand since forever, for one of the three
// lists:
//
//     if (d_next == dclose)
//         d_next = d_next->next;
//
// That is the whole idea, and it is correct. It just only protects the one
// loop that happens to use the `d_next` global. This generalizes it: a walk
// registers where it keeps its successor, and extraction advances any
// registered successor that names the entity going away. The loop then carries
// on with the next live entity, which is what every one of those loops meant.
//
// Two rules for anything that removes an entity from a global list:
//
//   1. Call CursorRegistry<T>::Advance(entity) BEFORE unlinking it and before
//      freeing it. Both of those destroy the `next` the advance needs to read.
//   2. Do it for every removal path, or the loops go back to being on their own.
//

/// Where the walks in flight are keeping their successors, per entity type.
///
/// One vector per T, holding the address of each active walk's successor
/// variable rather than its value. The walk keeps updating that variable as it
/// steps, and registration has to survive it doing so. Walks nest a few deep at
/// most, so a linear scan is the right shape.
template <class T>
class CursorRegistry
{
public:
	static void Add(T **cursor) { cursors.push_back(cursor); }

	static void Remove(T **cursor)
	{
		cursors.erase(std::remove(cursors.begin(), cursors.end(), cursor), cursors.end());
	}

	/// Moves any walk that was about to visit `removed` past it. Must be called
	/// while `removed` is still linked, since that is where the answer comes from.
	static void Advance(T *removed)
	{
		if (removed == nullptr)
			return;

		for (T **cursor : cursors)
		{
			if (*cursor == removed)
				*cursor = removed->next;
		}
	}

	/// Walks in flight. For tests and for asserting a walk cleaned up after
	/// itself.
	static std::size_t ActiveCount() { return cursors.size(); }

private:
	inline static std::vector<T **> cursors;
};

/// A walk over a global entity list that extraction knows about.
///
/// Replaces the hand-rolled save-the-successor loop:
///
///     for (ListWalk<CHAR_DATA> walk(char_list); !walk.Done(); walk.Step())
///     {
///         CHAR_DATA *ch = walk.Current();
///         ...
///     }
///
/// The body may extract the current entity, the next one, or any other, and the
/// walk still visits exactly the entities that are still on the list when it
/// reaches them.
template <class T>
class ListWalk
{
public:
	explicit ListWalk(T *head)
		: current(head)
		, next(head != nullptr ? head->next : nullptr)
	{
		CursorRegistry<T>::Add(&next);
	}

	~ListWalk() { CursorRegistry<T>::Remove(&next); }

	// The registry holds the address of `next`, so this object has to stay put.
	ListWalk(const ListWalk &) = delete;
	ListWalk &operator=(const ListWalk &) = delete;
	ListWalk(ListWalk &&) = delete;
	ListWalk &operator=(ListWalk &&) = delete;

	bool Done() const { return current == nullptr; }
	T *Current() const { return current; }

	void Step()
	{
		current = next;
		next = current != nullptr ? current->next : nullptr;
	}

private:
	T *current;
	T *next;		// registered; extraction may rewrite this mid-body
};

#endif /* ENTITY_LIST_CURSOR_H */
