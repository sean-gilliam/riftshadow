#include "catch.hpp"
#include "../code/merc.h"
#include "../code/db.h"
#include "../code/comm.h"
#include "../code/recycle.h"
#include "../code/handler.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/handles.h"
#include "../code/entity/list_cursor.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

//
// Storage for the three global entity lists: char_list, object_list and
// descriptor_list.
//
// These lists are what decides an entity's lifetime. The containment chains
// (room->people, ch->carrying, obj->contains) are a game rule about where
// things are, not about when they die. All three are owning containers now, so
// an entity's node is what holds it and erasing that node is what destroys it.
//
// This file characterised the storage behaviour the game depended on before
// that conversion, so that the same assertions could be run after it. Every
// scenario is written against the shim below. Anything *outside* the shim that
// had to be edited to keep this file compiling and green was a behaviour
// change, and finding those was the whole point.
//
// It found three, all recorded where they happened: Unlink collapsing into the
// erase (the shim), section 3 losing its premise with the last free list, and
// section 5 becoming the only place the successor hazard is still observable.
//

namespace
{
	// ─── the storage shim ──────────────────────────────────────────────────
	//
	// One list accessor and four operations. These are the only bodies in this
	// file that know how the lists are built.
	//
	// All three lists are owning containers now, so what used to be a generic
	// raw-list implementation plus three specializations is one generic body
	// again. Note what fell out of it: Unlink is gone. On a raw list "take it
	// off the list" and "free it" were two steps and a test could do one
	// without the other. Here erasing the node *is* the destruction, so there
	// is no "off the list but still alive" state left to construct. That
	// collapse is the behaviour change this phase made, and this is where the
	// file records it.
	//

	template <class T> std::list<std::unique_ptr<T>> &GlobalList();

	template <> CharacterList &GlobalList<CHAR_DATA>() { return char_list; }
	template <> ObjectList &GlobalList<OBJ_DATA>() { return object_list; }
	template <> DescriptorList &GlobalList<DESCRIPTOR_DATA>() { return descriptor_list; }

	/// Puts an entity on its global list, the way db.c/comm.c do it: push front.
	/// Ownership moves to the list here, which is what the real link sites do.
	template <class T>
	void Link(T *entity)
	{
		GlobalList<T>().push_front(std::unique_ptr<T>(entity));
		entity->globalNode = GlobalList<T>().begin();
	}

	/// The list as a vector, head first.
	template <class T>
	std::vector<T *> Collect()
	{
		std::vector<T *> out;

		for (auto &owned : GlobalList<T>())
			out.push_back(owned.get());

		return out;
	}

	/// The unprotected walk. It is what a hand-written loop does, and section 2
	/// needs it to show that extracting the current element is the safe case.
	/// It reads its successor before running the body, exactly as the old
	/// intrusive loops did.
	template <class T>
	void Walk(const std::function<void(T *)> &body)
	{
		auto &list = GlobalList<T>();

		for (auto it = list.begin(); it != list.end(); )
		{
			T *entity = it->get();
			++it;

			body(entity);
		}
	}

	/// One operation now: advancing the walks has to come first, while the node
	/// still links to its successor, and the erase runs the destructor.
	template <class T>
	void Extract(T *entity)
	{
		OwningCursorRegistry<T>::Advance(entity->globalNode);
		GlobalList<T>().erase(entity->globalNode);
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

	// Mirrors the order the real extract_char/extract_obj/close_socket use:
	// advance any walk in flight first, because the erase destroys the link
	// that answer comes from. A walk that registered no cursor is unaffected,
	// which is what makes section 3 and section 5 differ.
	void ExtractChar(CHAR_DATA *ch)        { Extract(ch); }
	void ExtractObj(OBJ_DATA *obj)         { Extract(obj); }
	void ExtractDescriptor(DESCRIPTOR_DATA *d) { Extract(d); }

	/// Leaves the list empty however a scenario ended.
	void DrainChars()       { while (!char_list.empty()) ExtractChar(char_list.front().get()); }
	void DrainObjs()        { while (!object_list.empty()) ExtractObj(object_list.front().get()); }
	void DrainDescriptors() { while (!descriptor_list.empty()) ExtractDescriptor(descriptor_list.front().get()); }
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
// RETIRED. This section held two scenarios that asserted the pre-conversion
// behaviour on purpose, because that is what "no behaviour change" would have
// meant:
//
//   - "a walk that extracts its successor abandons the rest of the list", and
//   - "a walk that extracts its successor continues into the free list".
//
// Both rested on the same mechanism. The free lists were threaded through the
// very `next` field the global lists used, so freeing an entity overwrote the
// successor a walk had already saved, and the walk left the live list at that
// point. With char_list converted there is no free list left in the tree and no
// `next` field to overwrite, so neither scenario has a premise to assert. They
// were removed rather than rewritten because the rewrite is not a variation on
// them: an unprotected walk over an owning list holds an iterator to an erased
// node, which is undefined rather than merely wrong, and there is nothing a
// test may legally observe about it.
//
// What replaced them is section 5. The hazard is not gone, it is answered, and
// answered somewhere a test can watch: extraction advances the cursor before it
// destroys what the cursor names. Section 5 asserts that directly, which is the
// assertion these two were standing in for.
//
// The box that used to sit here predicted tombstone-and-sweep as the
// replacement. That design was costed during execution and rejected in favour
// of cursor patching. The prediction being wrong is why it is worth recording:
// the section named a mechanism rather than the property it wanted, and the
// property ("keep the pointer the cursor is about to follow valid") is what
// survived into section 5.
//

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

//
// 5. The cursor-aware walk.
//
// Section 3 pins what a hand-rolled walk does when its successor is extracted:
// it leaves the live list. A registered walk hands extraction somewhere to move
// that successor to instead. These are section 3's setups with the walk swapped,
// and they are what the walks in the game become as each list converts.
//
// All four global lists are owning containers, so every walk here is an
// OwningListWalk and every successor is a list iterator. The hazard is sharper
// than it was on the raw lists rather than milder: erasing the node really frees
// the entity, so a walk that carried on regardless would be reading freed memory
// rather than an intact corpse. What makes these pass is extraction advancing
// the cursor before it destroys what the cursor names.
//
// The raw-list form of this helper is gone with the last raw list. ListWalk and
// CursorRegistry survive in list_cursor.h with no caller left in the tree.
//

namespace
{
	/// The walk under test. Same contract as the unprotected one in the shim:
	/// the body sees each entity the walk lands on.
	void CursorWalk(ObjectList &list, const std::function<void(OBJ_DATA *)> &body)
	{
		for (OwningListWalk<OBJ_DATA> walk(list); !walk.Done(); walk.Step())
			body(walk.Current());
	}
}

SCENARIO("a cursor-aware walk survives having its successor extracted", "[entity_storage]")
{
	GIVEN("three objects on the list")
	{
		OBJ_DATA *oldest = MakeObj();
		OBJ_DATA *middle = MakeObj();
		OBJ_DATA *newest = MakeObj();

		// No free list to seed any more: object_list owns its objects, so
		// extraction returns the memory to the allocator. Section 3's char_list
		// scenarios still seed one, because char_free is still there.

		WHEN("the first body call extracts the entity the walk was about to reach")
		{
			std::vector<OBJ_DATA *> visited;

			CursorWalk(object_list, [&](OBJ_DATA *obj)
			{
				visited.push_back(obj);

				if (obj == newest)
					ExtractObj(middle);
			});

			THEN("the walk skips the extracted entity and finishes the live list")
			{
				// The comparison that matters: section 3's equivalent setup on
				// char_list, walked by hand, visits four entities of which three
				// are dead and never reaches the last live one.
				REQUIRE(visited == std::vector<OBJ_DATA *>({newest, oldest}));
			}

			THEN("everything the body was handed was alive when it got it")
			{
				for (OBJ_DATA *seen : visited)
					REQUIRE(Deref(seen->self) == seen);
			}
		}

		WHEN("the body extracts the entity it was handed")
		{
			std::vector<OBJ_DATA *> visited;

			CursorWalk(object_list, [&](OBJ_DATA *obj)
			{
				visited.push_back(obj);
				ExtractObj(obj);
			});

			THEN("the walk still visits all three and empties the list")
			{
				REQUIRE(visited == std::vector<OBJ_DATA *>({newest, middle, oldest}));
				REQUIRE(Collect<OBJ_DATA>().empty());
			}
		}

		WHEN("the body extracts an entity the walk has not reached yet")
		{
			std::vector<OBJ_DATA *> visited;

			CursorWalk(object_list, [&](OBJ_DATA *obj)
			{
				visited.push_back(obj);

				if (obj == newest)
					ExtractObj(oldest);
			});

			THEN("the walk simply never sees it")
			{
				REQUIRE(visited == std::vector<OBJ_DATA *>({newest, middle}));
			}
		}

		DrainObjs();
	}
}

SCENARIO("a walk deregisters its cursor however it ends", "[entity_storage]")
{
	GIVEN("no walks in flight")
	{
		REQUIRE(OwningCursorRegistry<OBJ_DATA>::ActiveCount() == 0);

		MakeObj();		// something for the walks to land on

		WHEN("a walk runs to completion")
		{
			CursorWalk(object_list, [](OBJ_DATA *) {});

			THEN("the registry is empty again")
			{
				REQUIRE(OwningCursorRegistry<OBJ_DATA>::ActiveCount() == 0);
			}
		}

		WHEN("walks are nested over the same list")
		{
			std::size_t innerDepth = 0;

			CursorWalk(object_list, [&](OBJ_DATA *)
			{
				CursorWalk(object_list, [&](OBJ_DATA *)
				{
					innerDepth = OwningCursorRegistry<OBJ_DATA>::ActiveCount();
				});
			});

			THEN("both were registered at once, and both cleaned up")
			{
				REQUIRE(innerDepth == 2);
				REQUIRE(OwningCursorRegistry<OBJ_DATA>::ActiveCount() == 0);
			}
		}

		WHEN("a walk is abandoned by an exception")
		{
			// The destructor is what deregisters, so an early exit out of a walk
			// must not strand a dangling T** in the registry -- it would be
			// written through by the next extraction.
			try
			{
				CursorWalk(object_list, [](OBJ_DATA *)
				{
					throw std::runtime_error("abandon the walk");
				});
			}
			catch (const std::runtime_error &)
			{
			}

			THEN("the registry is empty again")
			{
				REQUIRE(OwningCursorRegistry<OBJ_DATA>::ActiveCount() == 0);
			}
		}

		DrainObjs();
	}
}
