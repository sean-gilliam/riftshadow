#include "catch.hpp"
#include "../code/merc.h"
#include "../code/handler.h"
#include "../code/characterClasses/chrono.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/exit_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/handles.h"
#include "../code/enums.h"
#include "../code/db.h"
#include "../code/recycle.h"

//
// RUNE_DATA, and the two lists every rune is on at once.
//
// apply_rune puts a rune on the global `rune_list` (via `next`) AND on a
// per-container chain hanging off obj->rune / exit->rune / room->rune (via
// `next_content`). update.c ticks runes off the global list and calls
// extract_rune when one expires; find_rune walks the per-container chain to
// decide whether entering a room or opening a door should trigger anything.
//
// So the global list decides *when* a rune fires and the container chain
// decides *whether*, and extract_rune has to unlink from both. These scenarios
// are about the container half, because that is the half it gets wrong.
//

namespace
{
	// apply_rune copies its argument into a fresh new_rune(), so the caller's
	// struct is a template rather than the thing that ends up on the lists.
	RUNE_DATA *PlaceRuneOnObj(OBJ_DATA *obj, int type, int trigger)
	{
		RUNE_DATA temp;

		temp.function = nullptr;
		temp.placed_on = obj;
		temp.owner = nullptr;
		temp.target_type = RUNE_TO_WEAPON;
		temp.trigger_type = trigger;
		temp.level = 30;
		temp.duration = 5;
		temp.type = type;
		temp.extra = 0;
		temp.drawn_in = 0;
		temp.next = nullptr;
		temp.next_content = nullptr;
		temp.end_fun = nullptr;

		apply_rune(&temp);

		// apply_rune pushes onto both heads, so the rune it just made is the
		// head of the global list.
		return rune_list;
	}

	// The container chain as a list of rune `type`s, read the way find_rune
	// reads it. A rune that has been unlinked correctly does not appear here.
	std::vector<int> ChainTypes(OBJ_DATA *obj)
	{
		std::vector<int> types;

		for (RUNE_DATA *rune = obj->rune; rune != nullptr; rune = rune->next_content)
			types.push_back(rune->type);

		return types;
	}

	int GlobalListLength()
	{
		int count = 0;

		for (RUNE_DATA *rune = rune_list; rune != nullptr; rune = rune->next)
			count++;

		return count;
	}

	void ClearRunes(OBJ_DATA *obj)
	{
		while (rune_list != nullptr)
			extract_rune(rune_list);

		obj->rune = nullptr;
		obj->has_rune = false;
	}
}

SCENARIO("a rune is reachable from both of the lists it is placed on", "[rune]")
{
	GIVEN("an object with one rune on it")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		THEN("it is on the global tick list and on the object's own chain")
		{
			REQUIRE(GlobalListLength() == 1);
			REQUIRE(ChainTypes(obj) == std::vector<int>{11});
			REQUIRE(obj->has_rune);
		}

		THEN("find_rune matches it by trigger")
		{
			REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_EXIT, nullptr) != nullptr);
			REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_ENTRY, nullptr) == nullptr);
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the newest rune leaves the older ones in place", "[rune]")
{
	GIVEN("an object with three runes, newest first")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);
		PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);
		RUNE_DATA *newest = PlaceRuneOnObj(obj, 33, RUNE_TRIGGER_EXIT);

		REQUIRE(ChainTypes(obj) == std::vector<int>({33, 22, 11}));

		WHEN("the newest expires")
		{
			// The head case, and the only one extract_rune has ever handled.
			extract_rune(newest);

			THEN("the other two are still on the chain, in order")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>({22, 11}));
				REQUIRE(GlobalListLength() == 2);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the oldest rune does not hide the newer ones", "[rune]")
{
	GIVEN("an object with two runes on it")
	{
		OBJ_DATA *obj = new_obj();

		RUNE_DATA *oldest = PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);

		WHEN("the oldest expires")
		{
			// It is the tail, so its next_content is null -- and that is the
			// branch that clears has_rune outright regardless of what else is
			// still on the chain.
			extract_rune(oldest);

			THEN("the newer rune is still there and still visible")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>{22});
				REQUIRE(obj->has_rune);
				REQUIRE(find_rune(obj, RUNE_TO_WEAPON, RUNE_TRIGGER_EXIT, nullptr) != nullptr);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting a rune from the middle of the chain keeps the rest", "[rune]")
{
	GIVEN("an object with three runes on it")
	{
		OBJ_DATA *obj = new_obj();

		PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		RUNE_DATA *middle = PlaceRuneOnObj(obj, 22, RUNE_TRIGGER_EXIT);

		PlaceRuneOnObj(obj, 33, RUNE_TRIGGER_EXIT);

		WHEN("the middle one expires")
		{
			extract_rune(middle);

			THEN("the newest is still the head and the oldest still follows it")
			{
				REQUIRE(ChainTypes(obj) == std::vector<int>({33, 11}));
				REQUIRE(GlobalListLength() == 2);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}

SCENARIO("extracting the last rune leaves the container naming nothing", "[rune]")
{
	GIVEN("an object with a single rune on it")
	{
		OBJ_DATA *obj = new_obj();

		RUNE_DATA *only = PlaceRuneOnObj(obj, 11, RUNE_TRIGGER_EXIT);

		WHEN("it expires")
		{
			extract_rune(only);

			THEN("the object names no rune at all")
			{
				// free_rune pushes onto the rune free list, so a container left
				// pointing at an extracted rune is pointing at something
				// new_rune can hand straight back out.
				REQUIRE(obj->rune == nullptr);
				REQUIRE(!obj->has_rune);
				REQUIRE(GlobalListLength() == 0);
			}
		}

		ClearRunes(obj);
		free_obj(obj);
	}
}
