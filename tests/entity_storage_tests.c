#include "catch.hpp"
#include "../code/merc.h"
#include "../code/db.h"
#include "../code/comm.h"
#include "../code/recycle.h"
#include "../code/handler.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/handles.h"

#include <algorithm>
#include <functional>
#include <vector>

//
// Storage for the three global entity lists: char_list, object_list and
// descriptor_list.
//
// These lists are what decides an entity's lifetime. The containment chains
// (room->people, ch->carrying, obj->contains) are a game rule about where
// things are, not about when they die. Today each global list is an intrusive
// singly-linked chain through an entity's `next` field, and `free_char` and
// friends push the entity onto a free list rather than returning its memory,
// so an address is handed straight back out to the next allocation.
//
// This file characterises the storage behaviour the game currently depends on,
// so that the same assertions can be run after the lists become owning
// containers. Every scenario is written against the five-function shim below;
// converting the containers means rewriting those five bodies. Anything
// *outside* the shim that has to be edited to keep this file compiling and
// green is a behaviour change, and finding those is the whole point.
//
// The interesting case is the last two scenarios. A walk that unlinks the
// element it is looking at is safe by construction, because every walk in the
// game captures the successor before running its body. A walk that unlinks the
// *successor* is not, and the damage is bigger than reading stale bytes: the
// free lists are threaded through the very same `next` field, so freeing an
// entity overwrites the successor the walk had already saved. The walk stops
// following the live list and starts following the free list. Section 3 pins
// both halves of that.
//

namespace
{
	// ─── the storage shim ──────────────────────────────────────────────────
	//
	// One head accessor and four operations. These are the only bodies in this
	// file that know how the lists are built.
	//

	template <class T> T *&ListHead();

	template <> CHAR_DATA *&ListHead<CHAR_DATA>() { return char_list; }
	template <> OBJ_DATA *&ListHead<OBJ_DATA>() { return object_list; }
	template <> DESCRIPTOR_DATA *&ListHead<DESCRIPTOR_DATA>() { return descriptor_list; }

	/// Puts an entity on its global list, the way db.c/comm.c do it: push front.
	template <class T>
	void Link(T *entity)
	{
		entity->next = ListHead<T>();
		ListHead<T>() = entity;
	}

	/// Takes an entity off its global list. Mirrors extract_char/extract_obj/
	/// close_socket, including the linear scan for the predecessor.
	template <class T>
	void Unlink(T *entity)
	{
		if (ListHead<T>() == entity)
		{
			ListHead<T>() = entity->next;
			return;
		}

		for (T *prev = ListHead<T>(); prev != nullptr; prev = prev->next)
		{
			if (prev->next == entity)
			{
				prev->next = entity->next;
				return;
			}
		}
	}

	/// The list as a vector, head first.
	template <class T>
	std::vector<T *> Collect()
	{
		std::vector<T *> out;

		for (T *entity = ListHead<T>(); entity != nullptr; entity = entity->next)
			out.push_back(entity);

		return out;
	}

	/// Walks the live list the way the game does in 164 places: the successor is
	/// read before the body runs, so the body is free to unlink the element it
	/// was handed. `body` sees each entity the cursor lands on, including any it
	/// should not have.
	template <class T>
	void Walk(const std::function<void(T *)> &body)
	{
		T *next;

		for (T *entity = ListHead<T>(); entity != nullptr; entity = next)
		{
			next = entity->next;
			body(entity);
		}
	}

	// ─── end of shim ───────────────────────────────────────────────────────

	//
	// Entities come from the real allocators so that they are registered in the
	// slot map and their liveness is something a test can assert on. Extract*
	// pairs the list unlink with the free, which is what extract_char and
	// extract_obj do for storage purposes -- without dragging in rooms, carried
	// objects or the event queue, none of which this file is about.
	//

	CHAR_DATA *MakeChar()
	{
		CHAR_DATA *ch = new_char();
		Link(ch);
		return ch;
	}

	OBJ_DATA *MakeObj()
	{
		OBJ_DATA *obj = new_obj();
		Link(obj);
		return obj;
	}

	DESCRIPTOR_DATA *MakeDescriptor()
	{
		DESCRIPTOR_DATA *d = new_descriptor();
		Link(d);
		return d;
	}

	void ExtractChar(CHAR_DATA *ch)   { Unlink(ch); free_char(ch); }
	void ExtractObj(OBJ_DATA *obj)    { Unlink(obj); free_obj(obj); }
	void ExtractDescriptor(DESCRIPTOR_DATA *d) { Unlink(d); free_descriptor(d); }

	/// Leaves the list empty however a scenario ended.
	void DrainChars()       { while (char_list != nullptr) ExtractChar(char_list); }
	void DrainObjs()        { while (object_list != nullptr) ExtractObj(object_list); }
	void DrainDescriptors() { while (descriptor_list != nullptr) ExtractDescriptor(descriptor_list); }
}

//
// 1. Link, walk, unlink -- order and count, for each of the three lists.
//

SCENARIO("a global list hands entities back in the order they were linked", "[entity_storage]")
{
	GIVEN("three characters linked one after another")
	{
		CHAR_DATA *first = MakeChar();
		CHAR_DATA *second = MakeChar();
		CHAR_DATA *third = MakeChar();

		THEN("the list reads newest first")
		{
			REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({third, second, first}));
		}

		DrainChars();
	}

	GIVEN("three objects linked one after another")
	{
		OBJ_DATA *first = MakeObj();
		OBJ_DATA *second = MakeObj();
		OBJ_DATA *third = MakeObj();

		THEN("the list reads newest first")
		{
			REQUIRE(Collect<OBJ_DATA>() == std::vector<OBJ_DATA *>({third, second, first}));
		}

		DrainObjs();
	}

	GIVEN("three descriptors linked one after another")
	{
		DESCRIPTOR_DATA *first = MakeDescriptor();
		DESCRIPTOR_DATA *second = MakeDescriptor();
		DESCRIPTOR_DATA *third = MakeDescriptor();

		THEN("the list reads newest first")
		{
			REQUIRE(Collect<DESCRIPTOR_DATA>() == std::vector<DESCRIPTOR_DATA *>({third, second, first}));
		}

		DrainDescriptors();
	}
}

SCENARIO("unlinking leaves the rest of the list intact wherever it happened", "[entity_storage]")
{
	GIVEN("three characters on the list, newest first")
	{
		CHAR_DATA *oldest = MakeChar();
		CHAR_DATA *middle = MakeChar();
		CHAR_DATA *newest = MakeChar();

		WHEN("the head is unlinked")
		{
			ExtractChar(newest);

			THEN("the other two remain in order")
			{
				REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({middle, oldest}));
			}
		}

		WHEN("the middle is unlinked")
		{
			ExtractChar(middle);

			THEN("the head and tail close up")
			{
				REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({newest, oldest}));
			}
		}

		WHEN("the tail is unlinked")
		{
			ExtractChar(oldest);

			THEN("the other two remain in order")
			{
				REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({newest, middle}));
			}
		}

		WHEN("the only remaining entity is unlinked")
		{
			ExtractChar(newest);
			ExtractChar(middle);
			ExtractChar(oldest);

			THEN("the list is empty")
			{
				REQUIRE(Collect<CHAR_DATA>().empty());
			}
		}

		DrainChars();
	}
}

//
// 2. The safe case. Extracting the element the walk is looking at.
//
// Safe because the walk reads the successor before running the body. This has
// to keep behaving identically: it is what every death, logout and object
// destruction in the game does.
//

SCENARIO("a walk may extract the entity it was handed", "[entity_storage]")
{
	GIVEN("three characters on the list")
	{
		CHAR_DATA *oldest = MakeChar();
		CHAR_DATA *middle = MakeChar();
		CHAR_DATA *newest = MakeChar();

		WHEN("the walk extracts the middle one as it reaches it")
		{
			std::vector<CHAR_DATA *> visited;

			Walk<CHAR_DATA>([&](CHAR_DATA *ch)
			{
				visited.push_back(ch);

				if (ch == middle)
					ExtractChar(ch);
			});

			THEN("every entity was visited exactly once, in order")
			{
				REQUIRE(visited == std::vector<CHAR_DATA *>({newest, middle, oldest}));
			}

			THEN("the extracted one is off the list and the others are not")
			{
				REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({newest, oldest}));
			}
		}

		WHEN("the walk extracts every entity it is handed")
		{
			int visits = 0;

			Walk<CHAR_DATA>([&](CHAR_DATA *ch)
			{
				visits++;
				ExtractChar(ch);
			});

			THEN("the walk completed and the list is empty")
			{
				REQUIRE(visits == 3);
				REQUIRE(Collect<CHAR_DATA>().empty());
			}
		}

		DrainChars();
	}

	GIVEN("three objects on the list")
	{
		OBJ_DATA *oldest = MakeObj();
		OBJ_DATA *middle = MakeObj();
		OBJ_DATA *newest = MakeObj();

		WHEN("the walk extracts the middle one as it reaches it")
		{
			std::vector<OBJ_DATA *> visited;

			Walk<OBJ_DATA>([&](OBJ_DATA *obj)
			{
				visited.push_back(obj);

				if (obj == middle)
					ExtractObj(obj);
			});

			THEN("every entity was visited exactly once, in order")
			{
				REQUIRE(visited == std::vector<OBJ_DATA *>({newest, middle, oldest}));
			}

			THEN("the extracted one is off the list and the others are not")
			{
				REQUIRE(Collect<OBJ_DATA>() == std::vector<OBJ_DATA *>({newest, oldest}));
			}
		}

		DrainObjs();
	}
}

//
// 3. The hazard, and this file's oracle. Extracting the successor mid-walk.
//
// Nothing in the game documents that a walk must not extract anything but its
// current element, and nothing enforces it. When it happens, the cursor is left
// holding an entity that has already been freed. The damage is not that
// the walk reads stale bytes. It is that `free_char` *overwrites* `ch->next`
// with the free-list head on its way out, so the walk's saved successor is a
// pointer into the free list.
//
// The consequences, both pinned below:
//
//   - the walk leaves the live list at that point and never comes back, so
//     every entity after the extracted one is silently skipped;
//   - if the free list is not empty, the walk continues *through it*, handing
//     the loop body one dead entity after another until the free list runs out.
//
// So a single misplaced extraction turns one pass over the living into a
// partial pass plus a full pass over the dead. That is invisible today: the
// memory is mapped, the fields are readable, and nothing crashes.
//
// ┌─ WHEN THE CONTAINERS CONVERT, THIS IS WHERE IT SHOWS ─────────────────────┐
// │ These two scenarios assert *today's* broken behaviour on purpose, because │
// │ that is what "no behaviour change" would mean. They are the only          │
// │ assertions in this file expected to need editing, and editing them is the │
// │ record of the semantic change. After conversion the same code cannot      │
// │ silently continue. A saved iterator to an erased node is a hard failure   │
// │ and a sanitizer report, which is the improvement being bought.            │
// │                                                                           │
// │ Note this is NOT fixed by keeping the entity alive past the extract: the  │
// │ entity's lifetime is not what the walk is following, the list node is.    │
// │ Preserving these semantics needs the *unlink* deferred, not just the      │
// │ free, ie. the node tombstoned in place and erased later.                  │
// └───────────────────────────────────────────────────────────────────────────┘
//

SCENARIO("a walk that extracts its successor abandons the rest of the list", "[entity_storage]")
{
	GIVEN("three characters on the list and an empty free list")
	{
		CHAR_DATA *oldest = MakeChar();
		CHAR_DATA *middle = MakeChar();
		CHAR_DATA *newest = MakeChar();

		Handle<CHAR_DATA> middleHandle = middle->self;

		REQUIRE(Deref(middleHandle) == middle);

		WHEN("the first body call extracts the entity the cursor is about to reach")
		{
			std::vector<CHAR_DATA *> visited;
			bool middleWasLiveWhenVisited = true;

			Walk<CHAR_DATA>([&](CHAR_DATA *ch)
			{
				visited.push_back(ch);

				if (ch == newest)
					ExtractChar(middle);

				if (ch == middle)
					middleWasLiveWhenVisited = (Deref(middleHandle) != nullptr);
			});

			THEN("the cursor still lands on the freed entity")
			{
				REQUIRE(visited.size() >= 2);
				REQUIRE(visited[0] == newest);
				REQUIRE(visited[1] == middle);
			}

			THEN("its handle was already dead when the walk got there")
			{
				// Extraction expires the handle, so no supported reference can
				// follow the raw cursor here. This is the one guarantee that
				// does hold, and it is Phase 5's doing rather than the list's.
				REQUIRE(middleWasLiveWhenVisited == false);
				REQUIRE(Deref(middleHandle) == nullptr);
			}

			THEN("and the rest of the live list is never visited at all")
			{
				// free_char set middle->next to the free-list head, which was
				// null, so the walk ended there. `oldest` is still linked and
				// still alive -- it was simply skipped.
				REQUIRE(visited.size() == 2);
				REQUIRE(std::find(visited.begin(), visited.end(), oldest) == visited.end());
				REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({newest, oldest}));
				REQUIRE(Deref(oldest->self) == oldest);
			}
		}

		DrainChars();
	}
}

SCENARIO("a walk that extracts its successor continues into the free list", "[entity_storage]")
{
	GIVEN("three characters on the list and two corpses on the free list")
	{
		CHAR_DATA *oldest = MakeChar();
		CHAR_DATA *middle = MakeChar();
		CHAR_DATA *newest = MakeChar();

		// Seeded *after* the live list on purpose. Freeing first does not work:
		// the MakeChar calls above would pop the corpses straight back off and
		// reuse their addresses, which is the recycling this phase is removing.
		CHAR_DATA *corpseA = new_char();
		CHAR_DATA *corpseB = new_char();

		free_char(corpseA);
		free_char(corpseB);

		WHEN("the first body call extracts the entity the cursor is about to reach")
		{
			std::vector<CHAR_DATA *> visited;

			// Bounded, because the thing being characterised is a walk that has
			// left the container it thought it was walking.
			int guard = 0;

			CHAR_DATA *next;

			for (CHAR_DATA *ch = char_list; ch != nullptr && guard < 20; ch = next, guard++)
			{
				next = ch->next;
				visited.push_back(ch);

				if (ch == newest)
					ExtractChar(middle);
			}

			THEN("the loop body is handed the free list's contents as if they were live")
			{
				// newest (live), middle (just freed), then both corpses, in
				// free-list order. Four entities for a three-entity list.
				REQUIRE(visited.size() == 4);
				REQUIRE(visited[0] == newest);
				REQUIRE(visited[1] == middle);
				REQUIRE(visited[2] == corpseB);
				REQUIRE(visited[3] == corpseA);
			}

			THEN("three of the four are dead, and the one live straggler was skipped")
			{
				REQUIRE(Deref(visited[1]->self) == nullptr);
				REQUIRE(Deref(visited[2]->self) == nullptr);
				REQUIRE(Deref(visited[3]->self) == nullptr);
				REQUIRE(std::find(visited.begin(), visited.end(), oldest) == visited.end());
				REQUIRE(Deref(oldest->self) == oldest);
			}
		}

		DrainChars();
	}
}

//
// 4. Handles across list mutation.
//
// The slot map stores plain pointers to entities allocated elsewhere and never
// moves them, so linking and unlinking other entities must not disturb a
// handle. Expiry happens at exactly one point: the free.
//

SCENARIO("a handle survives list mutation and expires exactly at extract", "[entity_storage]")
{
	GIVEN("a character with a handle taken to it")
	{
		CHAR_DATA *watched = MakeChar();
		Handle<CHAR_DATA> handle = watched->self;

		REQUIRE(Deref(handle) == watched);

		WHEN("other entities are linked in front of it")
		{
			CHAR_DATA *other = MakeChar();
			CHAR_DATA *another = MakeChar();

			THEN("the handle still resolves to the same character")
			{
				REQUIRE(Deref(handle) == watched);
			}

			WHEN("those others are extracted again")
			{
				ExtractChar(another);
				ExtractChar(other);

				THEN("the handle is untouched")
				{
					REQUIRE(Deref(handle) == watched);
					REQUIRE(Collect<CHAR_DATA>() == std::vector<CHAR_DATA *>({watched}));
				}
			}
		}

		WHEN("the character itself is extracted")
		{
			ExtractChar(watched);

			THEN("the handle is expired")
			{
				REQUIRE(Deref(handle) == nullptr);
			}
		}

		DrainChars();
	}

	GIVEN("an object with a handle taken to it")
	{
		OBJ_DATA *watched = MakeObj();
		Handle<OBJ_DATA> handle = watched->self;

		REQUIRE(Deref(handle) == watched);

		WHEN("another object is linked and extracted")
		{
			OBJ_DATA *other = MakeObj();

			REQUIRE(Deref(handle) == watched);

			ExtractObj(other);

			THEN("the handle still resolves")
			{
				REQUIRE(Deref(handle) == watched);
			}
		}

		WHEN("the object itself is extracted")
		{
			ExtractObj(watched);

			THEN("the handle is expired")
			{
				REQUIRE(Deref(handle) == nullptr);
			}
		}

		DrainObjs();
	}
}
