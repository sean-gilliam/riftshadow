#ifndef ENTITY_LIST_CURSOR_H
#define ENTITY_LIST_CURSOR_H

#include <algorithm>
#include <list>
#include <memory>
#include <vector>

//
// Walking a global entity list while things are being extracted from it.
//
// The hazard: a loop body that extracts something other than the element it was
// handed. Every walk in the game saves its successor before running the body:
//
//     for (OwningListWalk<OBJ_DATA> walk(object_list); !walk.Done(); walk.Step())
//     {
//         OBJ_DATA *obj = walk.Current();
//         ...                     // may extract obj, fine
//         ...                     // may extract the successor, not fine
//     }
//
// A global list owns what it holds, so extracting an entity erases its node and
// destroys it. Any saved successor naming that node is left pointing into a
// destroyed element, and stepping onto it reads freed memory.
//
// close_socket has coped with this by hand since forever, for one of the lists:
//
//     if (d_next == dclose)
//         d_next = d_next->next;
//
// That is the whole idea, and it is correct. It just only protected the one loop
// that happened to use the `d_next` global. This generalizes it: a walk
// registers where it keeps its successor, and extraction advances any
// registered successor that names the entity going away. The loop then carries
// on with the next live entity, which is what every one of those loops meant.
//
// Two rules for anything that removes an entity from a global list:
//
//   1. Call OwningCursorRegistry<T>::Advance(node) BEFORE erasing that node.
//      The erase is what destroys the link the advance needs to read.
//   2. Do it for every removal path, or the loops go back to being on their own.
//

/// Where the walks in flight are keeping their successors, per entity type.
///
/// One vector per T, holding the address of each active walk's successor
/// iterator rather than its value. The walk keeps updating that iterator as it
/// steps, and registration has to survive it doing so. Walks nest a few deep at
/// most, so a linear scan is the right shape.
template <class T>
class OwningCursorRegistry
{
public:
	using ListType = std::list<std::unique_ptr<T>>;
	using Iterator = typename ListType::iterator;

	static void Add(Iterator *cursor) { cursors.push_back(cursor); }

	static void Remove(Iterator *cursor)
	{
		cursors.erase(std::remove(cursors.begin(), cursors.end(), cursor), cursors.end());
	}

	/// Moves any walk that was about to visit `erased` past it. Must be called
	/// before the erase, while the node still links to its successor.
	static void Advance(Iterator erased)
	{
		for (Iterator *cursor : cursors)
		{
			if (*cursor == erased)
				*cursor = std::next(erased);
		}
	}

	/// Walks in flight. For tests and for asserting a walk cleaned up after
	/// itself.
	static std::size_t ActiveCount() { return cursors.size(); }

private:
	inline static std::vector<Iterator *> cursors;
};

/// A walk over a global entity list that extraction knows about.
///
/// Replaces the hand-rolled save-the-successor loop:
///
///     for (OwningListWalk<OBJ_DATA> walk(object_list); !walk.Done(); walk.Step())
///     {
///         OBJ_DATA *obj = walk.Current();
///         ...
///     }
///
/// The body may extract the current entity, the next one, or any other, and the
/// walk still visits exactly the entities that are still on the list when it
/// reaches them.
///
/// `Done` is only ever asked about the *new* current, because the loop runs
/// Step before it, so the body is free to extract the entity it was handed.
template <class T>
class OwningListWalk
{
public:
	using ListType = std::list<std::unique_ptr<T>>;
	using Iterator = typename ListType::iterator;

	explicit OwningListWalk(ListType &list)
		: list(list)
		, current(list.begin())
		, next(current == list.end() ? current : std::next(current))
	{
		OwningCursorRegistry<T>::Add(&next);
	}

	~OwningListWalk() { OwningCursorRegistry<T>::Remove(&next); }

	// The registry holds the address of `next`, so this object has to stay put.
	OwningListWalk(const OwningListWalk &) = delete;
	OwningListWalk &operator=(const OwningListWalk &) = delete;
	OwningListWalk(OwningListWalk &&) = delete;
	OwningListWalk &operator=(OwningListWalk &&) = delete;

	bool Done() const { return current == list.end(); }
	T *Current() const { return current->get(); }

	void Step()
	{
		current = next;
		next = current == list.end() ? current : std::next(current);
	}

private:
	ListType &list;
	Iterator current;
	Iterator next;		// registered; extraction may rewrite this mid-body
};

#endif /* ENTITY_LIST_CURSOR_H */
