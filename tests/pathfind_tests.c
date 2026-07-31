#include "catch.hpp"
#include "../code/merc.h"
#include "../code/act_move.h"
#include "../code/direction.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/exit_data.h"
#include "../code/entity/area_data.h"
#include "../code/db.h"
#include "../code/recycle.h"

//
// PATHFIND_DATA -- the mob tracking search.
//
// The search is a recursive walk over an area's room graph. Each node it visits
// is also hung off room->path, which is what stops it re-expanding a room it has
// already reached by a shorter route. So room->path is a *visited marker for one
// search*, and it has to be gone before the next search starts.
//
// That is the property these scenarios are about. The nodes themselves are a
// plain tree -- every one is stored into exactly one dir_to[] slot -- so
// ownership was never the interesting part; lifetime and leftover state were.
//

namespace
{
	std::vector<ROOM_INDEX_DATA *> builtRooms;
	AREA_DATA *testArea = nullptr;

	ROOM_INDEX_DATA *MakeRoom(int vnum)
	{
		auto room = new room_index_data();

		room->vnum = vnum;
		room->area = testArea;
		room->path = nullptr;

		room->next_room = room_list;
		room_list = room;

		builtRooms.push_back(room);

		return room;
	}

	void Link(ROOM_INDEX_DATA *from, ROOM_INDEX_DATA *to, int dir, int back)
	{
		auto out = new exit_data();
		out->u1.to_room = to;
		from->exit[dir] = out;

		auto in = new exit_data();
		in->u1.to_room = from;
		to->exit[back] = in;
	}

	void ClearRooms()
	{
		for (auto room : builtRooms)
		{
			for (auto i = 0; i < MAX_EXIT; i++)
			{
				if (room->exit[i] != nullptr)
				{
					delete room->exit[i];
					room->exit[i] = nullptr;
				}
			}
		}

		for (auto room : builtRooms)
			delete room;

		builtRooms.clear();
		room_list = nullptr;

		delete testArea;
		testArea = nullptr;
	}

	void StartWorld()
	{
		room_list = nullptr;
		testArea = new area_data();
	}
}

SCENARIO("a search finds the first step towards its goal", "[pathfind]")
{
	GIVEN("three rooms in a line, west to east")
	{
		StartWorld();

		auto west = MakeRoom(100);
		auto middle = MakeRoom(101);
		auto east = MakeRoom(102);

		Link(west, middle, Directions::East, Directions::West);
		Link(middle, east, Directions::East, Directions::West);

		WHEN("a search runs from the west end to the east end")
		{
			auto dir = find_first_step(west, east);

			THEN("the first step is east")
			{
				REQUIRE(dir == Directions::East);
			}
		}

		ClearRooms();
	}
}

SCENARIO("a search leaves no visited markers behind it", "[pathfind]")
{
	GIVEN("three rooms in a line")
	{
		StartWorld();

		auto west = MakeRoom(100);
		auto middle = MakeRoom(101);
		auto east = MakeRoom(102);

		Link(west, middle, Directions::East, Directions::West);
		Link(middle, east, Directions::East, Directions::West);

		WHEN("a search has run to completion")
		{
			find_first_step(west, east);

			THEN("no room still names a node from it")
			{
				// The markers point into a tree that has been freed by now, so
				// leaving one set is not merely untidy.
				REQUIRE(west->path == nullptr);
				REQUIRE(middle->path == nullptr);
				REQUIRE(east->path == nullptr);
			}
		}

		ClearRooms();
	}
}

SCENARIO("a search that finds nothing does not spoil the next one", "[pathfind]")
{
	GIVEN("two connected rooms and one unreachable room in the same area")
	{
		StartWorld();

		auto west = MakeRoom(100);
		auto middle = MakeRoom(101);
		auto marooned = MakeRoom(200);

		Link(west, middle, Directions::East, Directions::West);

		WHEN("a search for the unreachable room fails, and then a reachable one runs")
		{
			// The failing search still walks west and middle, and marks both.
			REQUIRE(find_first_step(west, marooned) == -1);

			auto dir = find_first_step(west, middle);

			THEN("the second search still works")
			{
				// It did not, before the search started owning its own state.
				// The markers left behind by the failed search say "already
				// reached in one step or fewer, and already evaluated", which
				// is exactly the test used to decide a room is not worth
				// expanding -- so the second search pruned the only route it
				// had and reported no path.
				REQUIRE(dir == Directions::East);
			}
		}

		ClearRooms();
	}
}
