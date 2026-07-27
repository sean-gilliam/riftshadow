#include "catch.hpp"
#include "../code/handler.h"
#include "../code/act_comm.h"
#include "../code/entity/char_data.h"
#include "../code/entity/obj_data.h"
#include "../code/entity/room_index_data.h"
#include "../code/entity/area_data.h"
#include "../code/entity/mob_index_data.h"
#include "../code/macros.h"
#include "../code/entity/handles.h"
#include "../code/enums.h"
#include "../code/db.h"
#include "../code/recycle.h"

// TEST_CASE("Test capitalization", "[string]" )
// {
// 	const char* sample = "this string is lower case.";
// 	char* test = capitalize(sample);
// 	char* shouldBe = "This string is lower case.";
// 	REQUIRE(strcmp(test, shouldBe) == 0);
// }

SCENARIO("Testing race lookup", "[race_lookup]")
{
	GIVEN("a valid race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup("elf");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 4);
			}
		}

		WHEN("race_lookup function is called with the race in uppercase")
		{
			int test = race_lookup("ELF");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 4);
			}
		}
	}

	GIVEN("an invalid race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("race_lookup function is called with a valid race but with extra characters")
		{
			int test = race_lookup("elf  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("race_lookup function is called with special characters")
		{
			int test = race_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized race")
	{
		WHEN("race_lookup function is called")
		{
			int test = race_lookup(nullptr);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing liq lookup", "[liq_lookup]")
{
	GIVEN("a valid liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup("coffee");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 12);
			}
		}

		WHEN("liq_lookup function is called with the liquid in uppercase")
		{
			int test = liq_lookup("COFFEE");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 12);
			}
		}
	}

	GIVEN("an invalid liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup("foo");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("liq_lookup function is called with a valid liquid but with extra characters")
		{
			int test = liq_lookup("coffee  ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("liq_lookup function is called with special characters")
		{
			int test = liq_lookup("ÿàÞ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}

	GIVEN("an uninitialized liquid")
	{
		WHEN("liq_lookup function is called")
		{
			int test = liq_lookup(nullptr);

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}
}

SCENARIO("Testing weapon lookup", "[weapon_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup("polearm");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 7);
			}
		}

		WHEN("weapon_lookup function is called with the weapon in uppercase")
		{
			int test = weapon_lookup("POLEARM");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 7);
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("weapon_lookup function is called with a valid weapon but with extra characters")
		{
			int test = weapon_lookup("polearm  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("weapon_lookup function is called with special characters")
		{
			int test = weapon_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized weapon")
	{
		WHEN("weapon_lookup function is called")
		{
			int test = weapon_lookup(nullptr);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing weapon type lookup", "[weapon_type_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup("sword");

			THEN("then it should return the correct type")
			{
				REQUIRE(test == WEAPON_SWORD);
			}
		}

		WHEN("weapon_type_lookup function is called with the weapon in uppercase")
		{
			int test = weapon_type_lookup("SWORD");

			THEN("then it should return the correct type")
			{
				REQUIRE(test == WEAPON_SWORD);
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup("foo");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}

		WHEN("weapon_type_lookup function is called with a valid weapon but with extra characters")
		{
			int test = weapon_type_lookup("sword  ");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}

		WHEN("weapon_type_lookup function is called with special characters")
		{
			int test = weapon_type_lookup("ÿàÞ");

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}
	}

	GIVEN("an uninitialized weapon")
	{
		WHEN("weapon_type_lookup function is called")
		{
			int test = weapon_type_lookup(nullptr);

			THEN("then it should return the WEAPON_EXOTIC type")
			{
				REQUIRE(test == WEAPON_EXOTIC);
			}
		}
	}
}

SCENARIO("Testing weapon name lookup", "[weapon_name_lookup]")
{
	GIVEN("a valid weapon")
	{
		WHEN("weapon_name_lookup function is called")
		{
			char* test = weapon_name_lookup(WEAPON_SPEAR);

			THEN("then it should return the correct weapon name")
			{
				REQUIRE(!str_prefix(test, "spear"));
			}
		}
	}

	GIVEN("an invalid weapon")
	{
		WHEN("weapon_name_lookup function is called")
		{
			char* test = weapon_name_lookup(-3);

			THEN("then it should return \"weapon\" as the weapon name")
			{
				REQUIRE(!str_prefix(test, "weapon"));
			}
		}

		WHEN("weapon_name_lookup function is called with a passed in default name")
		{
			char* test = weapon_name_lookup(-3, "exotic");

			THEN("then it should return the default name as the weapon name")
			{
				REQUIRE(!str_prefix(test, "exotic"));
			}
		}
	}
}

SCENARIO("Testing item lookup", "[item_lookup]")
{
	GIVEN("a valid item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup("food");

			THEN("then it should return the correct item type")
			{
				REQUIRE(test == ITEM_FOOD);
			}
		}

		WHEN("item_lookup function is called with the item in uppercase")
		{
			int test = item_lookup("FOOD");

			THEN("then it should return the correct item type")
			{
				REQUIRE(test == ITEM_FOOD);
			}
		}
	}

	GIVEN("an invalid item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup("bar");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("item_lookup function is called with a valid item but with extra characters")
		{
			int test = item_lookup("food  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("item_lookup function is called with special characters")
		{
			int test = item_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized item")
	{
		WHEN("item_lookup function is called")
		{
			int test = item_lookup(nullptr);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing item name lookup", "[item_name_lookup]")
{
	GIVEN("a valid item")
	{
		WHEN("item_name_lookup function is called")
		{
			char* test = item_name_lookup(ITEM_KEY);

			THEN("then it should return the correct weapon name")
			{
				REQUIRE(!str_prefix(test, "key"));
			}
		}
	}

	GIVEN("an invalid item")
	{
		WHEN("item_name_lookup function is called")
		{
			char* test = item_name_lookup(-18);

			THEN("then it should return \"none\" as the weapon name")
			{
				REQUIRE(!str_prefix(test, "none"));
			}
		}
	}
}

SCENARIO("Testing attack lookup", "[attack_lookup]")
{
	GIVEN("a valid attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup("punch");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 17);
			}
		}

		WHEN("attack_lookup function is called with the attack in uppercase")
		{
			int test = attack_lookup("PUNCH");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 17);
			}
		}
	}

	GIVEN("an invalid attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup("foo");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("attack_lookup function is called with a valid attack but with extra characters")
		{
			int test = attack_lookup("punch  ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}

		WHEN("attack_lookup function is called with special characters")
		{
			int test = attack_lookup("ÿàÞ");

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}

	GIVEN("an uninitialized attack")
	{
		WHEN("attack_lookup function is called")
		{
			int test = attack_lookup(nullptr);

			THEN("then it should return an index of 0")
			{
				REQUIRE(test == 0);
			}
		}
	}
}

SCENARIO("Testing wiznet lookup", "[wiznet_lookup]")
{
	GIVEN("a valid wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup("cabal");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 20);
			}
		}

		WHEN("wiznet_lookup function is called with the wiznet flag in uppercase")
		{
			long test = wiznet_lookup("CABAL");

			THEN("then it should return the correct index")
			{
				REQUIRE(test == 20);
			}
		}
	}

	GIVEN("an invalid wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup("foo");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("wiznet_lookup function is called with a valid wiznet but with extra characters")
		{
			long test = wiznet_lookup("cabal  ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}

		WHEN("wiznet_lookup function is called with special characters")
		{
			long test = wiznet_lookup("ÿàÞ");

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}

	GIVEN("an uninitialized wiznet flag")
	{
		WHEN("wiznet_lookup function is called")
		{
			long test = wiznet_lookup(nullptr);

			THEN("then it should return an index of -1")
			{
				REQUIRE(test == -1);
			}
		}
	}
}

SCENARIO("Testing deduct gold cost from character","[deduct_cost]")
{
	GIVEN("a character with zero gold")
	{
		auto player = new char_data();
		player->gold = 0;

		WHEN("gold is taken from that character")
		{
			deduct_cost(player, 1000);
			THEN("their gold should be set to zero")
			{
				REQUIRE(player->gold == 0);
			}
		}
	}
}

// Characterization tests for the char-affect list handlers (affect_to_char,
// affect_remove, affect_strip). These pin the observable add/find/remove/strip
// contract via the stable API (is_affected / affect_find) so it is provably
// preserved when ch->affected changes container.
SCENARIO("affect_to_char adds a char affect", "[affect_to_char]")
{
	GIVEN("a fresh character")
	{
		auto ch = new char_data();
		ch->name = "tester";

		WHEN("an affect is given to the character")
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = 100;
			af.where = TO_AFFECTS;
			af.level = 7;
			af.duration = 5;
			af.modifier = 3;
			affect_to_char(ch, &af);

			THEN("the character reports being affected by it")
			{
				REQUIRE(is_affected(ch, 100) == true);
				REQUIRE(is_affected(ch, 999) == false);
			}

			THEN("the stored affect carries the same fields")
			{
				AFFECT_DATA *found = affect_find(ch->affected, 100);
				REQUIRE(found != nullptr);
				REQUIRE(found->level == 7);
				REQUIRE(found->duration == 5);
				REQUIRE(found->modifier == 3);
			}

			THEN("init_duration is seeded from duration")
			{
				AFFECT_DATA *found = affect_find(ch->affected, 100);
				REQUIRE(found != nullptr);
				REQUIRE(found->init_duration == 5);
			}
		}
	}
}

SCENARIO("affect_remove removes a char affect", "[affect_remove]")
{
	GIVEN("a character with a single affect")
	{
		auto ch = new char_data();
		ch->name = "tester";

		AFFECT_DATA af;
		init_affect(&af);
		af.type = 100;
		af.where = TO_AFFECTS;
		af.duration = 5;
		affect_to_char(ch, &af);

		WHEN("that affect is removed")
		{
			affect_remove(ch, affect_find(ch->affected, 100));

			THEN("the character is no longer affected")
			{
				REQUIRE(is_affected(ch, 100) == false);
				REQUIRE(affect_find(ch->affected, 100) == nullptr);
			}
		}
	}
}

SCENARIO("removing one of several char affects preserves the rest", "[affect_remove]")
{
	GIVEN("a character with three distinct affects")
	{
		auto ch = new char_data();
		ch->name = "tester";

		for (int type : {100, 101, 102})
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = type;
			af.where = TO_AFFECTS;
			af.duration = 5;
			affect_to_char(ch, &af);
		}

		WHEN("the middle-added affect is removed")
		{
			affect_remove(ch, affect_find(ch->affected, 101));

			THEN("only that affect is gone")
			{
				REQUIRE(is_affected(ch, 100) == true);
				REQUIRE(is_affected(ch, 101) == false);
				REQUIRE(is_affected(ch, 102) == true);
			}
		}
	}
}

SCENARIO("affect_strip removes all affects of a type but leaves others", "[affect_strip]")
{
	GIVEN("a character affected twice by one type and once by another")
	{
		auto ch = new char_data();
		ch->name = "tester";

		for (int type : {100, 100, 101})
		{
			AFFECT_DATA af;
			init_affect(&af);
			af.type = type;
			af.where = TO_AFFECTS;
			af.duration = 5;
			affect_to_char(ch, &af);
		}

		WHEN("that type is stripped")
		{
			affect_strip(ch, 100);

			THEN("every instance of it is gone and the other type remains")
			{
				REQUIRE(is_affected(ch, 100) == false);
				REQUIRE(affect_find(ch->affected, 100) == nullptr);
				REQUIRE(is_affected(ch, 101) == true);
			}
		}
	}
}

//
// The entity slot maps. These exercise the real new_char/free_char and
// new_obj/free_obj rather than a stand-in, because the property that matters
// is the interaction with the free lists: those functions hand the same
// address back out, and the whole point of a handle is that reuse of an
// address must not resurrect a reference to the thing that used to live there.
//
// (Handle and SlotMap are covered on their own terms in handle_tests.c; these
// are about the wiring.)
//

SCENARIO("new_char registers a character and free_char expires it", "[handles]")
{
	GIVEN("a character from new_char")
	{
		CHAR_DATA *ch = new_char();

		THEN("it has a live self handle that derefs back to itself")
		{
			REQUIRE(!ch->self.IsNull());
			REQUIRE(Deref(ch->self) == ch);
		}

		WHEN("it is freed")
		{
			Handle<CHAR_DATA> stale = ch->self;
			free_char(ch);

			THEN("handles taken before the free no longer resolve")
			{
				REQUIRE(Deref(stale) == nullptr);
			}
		}
	}
}

SCENARIO("a recycled character address does not resurrect an old handle", "[handles]")
{
	GIVEN("a character that has been freed back onto the free list")
	{
		CHAR_DATA *first = new_char();
		Handle<CHAR_DATA> stale = first->self;
		free_char(first);

		WHEN("a new character is allocated, reusing that address")
		{
			CHAR_DATA *second = new_char();

			THEN("the old handle stays expired even though the address matches")
			{
				// The free list really did hand back the same memory -- if it
				// ever stops doing so this test is still correct, just less
				// pointed, so it is asserted rather than assumed.
				REQUIRE(second == first);

				REQUIRE(Deref(stale) == nullptr);
				REQUIRE(stale != second->self);
				REQUIRE(Deref(second->self) == second);
			}

			free_char(second);
		}
	}
}

SCENARIO("new_obj registers an object and free_obj expires it", "[handles]")
{
	GIVEN("an object from new_obj")
	{
		OBJ_DATA *obj = new_obj();

		THEN("it has a live self handle that derefs back to itself")
		{
			REQUIRE(!obj->self.IsNull());
			REQUIRE(Deref(obj->self) == obj);
		}

		WHEN("it is freed")
		{
			Handle<OBJ_DATA> stale = obj->self;
			free_obj(obj);

			THEN("handles taken before the free no longer resolve")
			{
				REQUIRE(Deref(stale) == nullptr);
			}
		}
	}
}

SCENARIO("an entity that never came from new_char is safe by default", "[handles]")
{
	GIVEN("a bare character built without new_char, as the tests do")
	{
		auto ch = new char_data();

		THEN("its self handle is null and derefs to nothing rather than to junk")
		{
			REQUIRE(ch->self.IsNull());
			REQUIRE(Deref(ch->self) == nullptr);
		}
	}
}

//
// ch->on -- what it means for the reference to go away.
//
// These pin the behavior of the two places that clear it today, because that
// behavior is the specification against which any change is judged.
//
// Worth stating plainly, because it is easy to get backwards: the clear inside
// obj_from_room is NOT lifetime bookkeeping. Nine of that function's ten
// callers leave the object entirely alive -- a player picking it up, wind
// blowing it to the next room, a portal being relocated. Only extract_obj is
// about death. So the rule it encodes is "the furniture left the room", which
// is a question about position, not about whether the object still exists.
//
// SittingOn is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	OBJ_DATA *SittingOn(CHAR_DATA *ch)
	{
		return Deref(ch->on);
	}

	ROOM_INDEX_DATA *MakeRoomForSitting()
	{
		auto room = new room_index_data();
		room->area = new area_data();	// char_from_room decrements area->nplayer

		return room;
	}

	void PutCharInRoomForSitting(CHAR_DATA *ch, ROOM_INDEX_DATA *room)
	{
		ch->in_room = room;
		ch->next_in_room = room->people;
		room->people = ch;
	}

	void PutObjInRoomForSitting(OBJ_DATA *obj, ROOM_INDEX_DATA *room)
	{
		obj->in_room = room;
		obj->next_content = room->contents;
		room->contents = obj;
	}
}

SCENARIO("a character stops sitting on furniture that leaves the room", "[on]")
{
	GIVEN("two characters each sitting on a different object in one room")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		OBJ_DATA *bench = new_obj();
		auto sitter = new char_data();
		auto bystander = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutObjInRoomForSitting(bench, room);
		PutCharInRoomForSitting(sitter, room);
		PutCharInRoomForSitting(bystander, room);

		sitter->on = chair->self;
		bystander->on = bench->self;

		WHEN("the chair is taken out of the room but not destroyed")
		{
			obj_from_room(chair);

			THEN("whoever was on it no longer is")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}

			THEN("someone on a different object is left alone")
			{
				REQUIRE(SittingOn(bystander) == bench);
			}

			THEN("the chair itself is still a perfectly live object")
			{
				// This is the whole reason the clear above cannot be replaced
				// by a liveness check: the object did not die, it moved.
				REQUIRE(chair->valid);
				REQUIRE(Deref(chair->self) == chair);
			}
		}
	}
}

SCENARIO("the on-clear reaches only the room the object left", "[on]")
{
	GIVEN("a character in another room referring to the object")
	{
		auto room = MakeRoomForSitting();
		auto elsewhere = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();
		auto distant = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		PutCharInRoomForSitting(distant, elsewhere);

		sitter->on = chair->self;
		distant->on = chair->self;

		WHEN("the chair leaves the room")
		{
			obj_from_room(chair);

			THEN("only the character in that room is cleared")
			{
				// Documents the mechanism's reach rather than a reachable game
				// state: the walk is over in_room->people, so it is scoped to
				// one room and is not a global sweep.
				REQUIRE(SittingOn(sitter) == nullptr);
				REQUIRE(SittingOn(distant) == chair);
			}
		}
	}
}

SCENARIO("a character stops sitting on anything when it leaves the room", "[on]")
{
	GIVEN("a character sitting on an object")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		sitter->on = chair->self;

		WHEN("the character leaves")
		{
			char_from_room(sitter);

			THEN("it is no longer on anything")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}
		}
	}
}

SCENARIO("a character is not left pointing at furniture that was destroyed", "[on]")
{
	GIVEN("a character sitting on an object")
	{
		auto room = MakeRoomForSitting();
		OBJ_DATA *chair = new_obj();
		auto sitter = new char_data();

		PutObjInRoomForSitting(chair, room);
		PutCharInRoomForSitting(sitter, room);
		sitter->on = chair->self;

		REQUIRE(SittingOn(sitter) == chair);

		WHEN("the object is destroyed without going through obj_from_room")
		{
			// obj_from_room clears the reference for anyone in the room, so it
			// hides this case. The paths that do not run it -- extract_obj's
			// not-found bail-out, or an object freed while held -- are the ones
			// that used to leave a pointer into a recycled object behind.
			free_obj(chair);

			THEN("the reference reads as nothing rather than as freed memory")
			{
				REQUIRE(SittingOn(sitter) == nullptr);
			}
		}
	}
}

//
// Deref(ch->last_fought) -- the mob hunt target, and what clears it.
//
// Unlike ch->on and ch->fighting, this field's scrub really is lifetime
// machinery. The clear that matters lives inside extract_char itself, under a
// "remove all tracking" comment, and extract_char is only ever reached when a
// character is leaving the world for good. Every other place that clears
// last_fought is a mob deciding to give up the hunt -- losing the trail,
// a peace command, the quarry fleeing -- and those all leave both characters
// alive.
//
// So this is the first field whose scrub loop clause can actually go. These
// tests pin the behavior it provides, so that the handle can be shown to
// provide the same thing.
//
// Tracking() is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	CHAR_DATA *Tracking(CHAR_DATA *ch)
	{
		return Deref(ch->last_fought);
	}

	/// extract_char takes the NPC path -- it decrements pIndexData->count and
	/// skips the altar-room redirect that only applies to players -- so these
	/// are mobs, with the index data that path requires.
	CHAR_DATA *MakeTracker(ROOM_INDEX_DATA *room)
	{
		CHAR_DATA *mob = new_char();
		mob->pIndexData = new mob_index_data();
		SET_BIT(mob->act, ACT_IS_NPC);
		mob->hit = 100;

		mob->in_room = room;
		mob->next_in_room = room->people;
		room->people = mob;

		mob->next = char_list;
		char_list = mob;

		return mob;
	}

	ROOM_INDEX_DATA *MakeTrackingRoom()
	{
		auto room = new room_index_data();
		room->area = new area_data();

		return room;
	}

	void ClearTrackers()
	{
		CHAR_DATA *next;

		for (CHAR_DATA *ch = char_list; ch != nullptr; ch = next)
		{
			next = ch->next;
			free_char(ch);
		}

		char_list = nullptr;
	}
}

SCENARIO("extract_char clears every tracker's reference to the extracted character", "[last_fought]")
{
	GIVEN("two mobs hunting a third and one hunting someone else")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);
		CHAR_DATA *secondHunter = MakeTracker(room);
		CHAR_DATA *otherQuarry = MakeTracker(room);
		CHAR_DATA *unrelated = MakeTracker(room);

		hunter->last_fought = quarry->self;
		secondHunter->last_fought = quarry->self;
		unrelated->last_fought = otherQuarry->self;

		WHEN("the quarry is extracted from the world")
		{
			extract_char(quarry, true);

			THEN("nobody is left hunting it")
			{
				REQUIRE(Tracking(hunter) == nullptr);
				REQUIRE(Tracking(secondHunter) == nullptr);
			}

			THEN("a mob hunting somebody else is left alone")
			{
				REQUIRE(Tracking(unrelated) == otherQuarry);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a mob that gives up the hunt does not disturb the quarry", "[last_fought]")
{
	GIVEN("a mob hunting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);

		hunter->last_fought = quarry->self;

		WHEN("the hunter loses interest rather than the quarry dying")
		{
			// The other half of the field's contract, and the reason the
			// clears outside extract_char have to stay: this one says "stop
			// hunting", not "the quarry is gone".
			hunter->last_fought = nullptr;

			THEN("the hunter is no longer tracking but the quarry is untouched")
			{
				REQUIRE(Tracking(hunter) == nullptr);
				REQUIRE(quarry->valid);
				REQUIRE(Deref(quarry->self) == quarry);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a hunter's reference to a freed quarry reads as nothing", "[last_fought]")
{
	GIVEN("a mob hunting another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *quarry = MakeTracker(room);
		CHAR_DATA *hunter = MakeTracker(room);

		hunter->last_fought = quarry->self;

		REQUIRE(Tracking(hunter) == quarry);

		WHEN("the quarry is freed without extract_char's sweep running")
		{
			// extract_char used to clear this reference by walking char_list.
			// The paths that never reach that walk -- its own not-found
			// bail-out, say -- are the ones that used to leave a pointer into
			// a character struct the free list can hand straight back out.
			free_char(quarry);

			THEN("the reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Tracking(hunter) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->reply) -- who `reply` answers, and what clears it.
//
// Like last_fought, this field's expiry is genuine lifetime machinery: the
// clear is a char_list pass written inline in extract_char, and that pass does
// nothing else. The other two places that clear reply are game rules that
// leave both characters alive -- do_noreply, where a player deliberately
// closes their ears, and the wizard invis/incog commands, where cloaking drops
// your own pending reply.
//
// Replying() is the single point that has to change if the field's type
// changes; every assertion below is written through it and stays as-is.
//

namespace
{
	CHAR_DATA *Replying(CHAR_DATA *ch)
	{
		return Deref(ch->reply);
	}
}

SCENARIO("extract_char clears every pending reply aimed at the extracted character", "[reply]")
{
	GIVEN("two characters holding a reply to a third and one replying elsewhere")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);
		CHAR_DATA *secondListener = MakeTracker(room);
		CHAR_DATA *someoneElse = MakeTracker(room);
		CHAR_DATA *unrelated = MakeTracker(room);

		listener->reply = speaker->self;
		secondListener->reply = speaker->self;
		unrelated->reply = someoneElse->self;

		WHEN("the speaker is extracted from the world")
		{
			extract_char(speaker, true);

			THEN("nobody is left holding a reply to it")
			{
				REQUIRE(Replying(listener) == nullptr);
				REQUIRE(Replying(secondListener) == nullptr);
			}

			THEN("a reply aimed at somebody else is left alone")
			{
				REQUIRE(Replying(unrelated) == someoneElse);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("the reply sweep does not disturb the character being replied to", "[reply]")
{
	GIVEN("one character holding a reply to another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);

		listener->reply = speaker->self;

		WHEN("the listener drops the reply rather than the speaker dying")
		{
			// The other half of the contract, and the reason the clears
			// outside extract_char stay: this one says "stop answering them",
			// not "they are gone".
			listener->reply = nullptr;

			THEN("the listener has no reply but the speaker is untouched")
			{
				REQUIRE(Replying(listener) == nullptr);
				REQUIRE(speaker->valid);
				REQUIRE(Deref(speaker->self) == speaker);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a reply to a freed character reads as nothing", "[reply]")
{
	GIVEN("one character holding a reply to another")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *speaker = MakeTracker(room);
		CHAR_DATA *listener = MakeTracker(room);

		listener->reply = speaker->self;

		REQUIRE(Replying(listener) == speaker);

		WHEN("the speaker is freed without extract_char's sweep running")
		{
			// That sweep is gone now, so this is the only thing standing
			// between `reply` and a pointer into a character struct the free
			// list can hand straight back out to the next new_char.
			free_char(speaker);

			THEN("the reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Replying(listener) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->master) / Deref(ch->leader) / Deref(ch->pet) -- the follower graph.
//
// These three differ from the fields above in that their clears live in
// helpers -- stop_follower, die_follower, nuke_pets -- rather than inline in
// extract_char, and those helpers are reached overwhelmingly for reasons that
// have nothing to do with anyone dying. die_follower alone has four callers
// and only one of them (extract_char) is a lifetime event; the others are a
// player setting nofollow, a sphere of plasma, and an item prog putting its
// owner to sleep. So the sweeps stay and the fields convert in place.
//
// Two behaviors here are deliberate game rules that a naive migration would
// "fix" into regressions, and both are pinned below:
//
//   * A follower whose leader dies is made its OWN leader, not leaderless.
//   * A follower secretly trailing via gsn_trail is exempt from the sweep and
//     stays attached. That exemption is correct while the master is alive --
//     the point of the skill is that the target cannot shake you -- and it is
//     what makes `master` outlive its target when the master is extracted.
//
// Master()/Leader()/Pet() are the single points that have to change if the
// field types change; every assertion below is written through them.
//

namespace
{
	CHAR_DATA *Master(CHAR_DATA *ch)
	{
		return Deref(ch->master);
	}

	CHAR_DATA *Leader(CHAR_DATA *ch)
	{
		return Deref(ch->leader);
	}

	CHAR_DATA *Pet(CHAR_DATA *ch)
	{
		return Deref(ch->pet);
	}

	/// gsn_trail and gsn_animate_dead are both zero in an unbooted test
	/// binary, and die_follower branches on both -- an affect added for one
	/// would answer for the other. Give trail a distinct value for the
	/// duration of a test.
	class ScopedTrailSkill
	{
	public:
		ScopedTrailSkill() : saved(gsn_trail) { gsn_trail = 500; }
		~ScopedTrailSkill() { gsn_trail = saved; }

	private:
		short saved;
	};

	void MakeTrailing(CHAR_DATA *ch)
	{
		AFFECT_DATA af;
		init_affect(&af);
		af.type = gsn_trail;
		af.where = TO_AFFECTS;
		af.duration = -1;
		affect_to_char(ch, &af);
	}
}

SCENARIO("die_follower detaches ordinary followers", "[follower]")
{
	GIVEN("a master with two followers and an unrelated pair")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *followerOne = MakeTracker(room);
		CHAR_DATA *followerTwo = MakeTracker(room);
		CHAR_DATA *otherMaster = MakeTracker(room);
		CHAR_DATA *otherFollower = MakeTracker(room);

		followerOne->master = master->self;
		followerTwo->master = master->self;
		otherFollower->master = otherMaster->self;

		WHEN("the master sheds its followers without dying")
		{
			die_follower(master);

			THEN("its own followers are detached")
			{
				REQUIRE(Master(followerOne) == nullptr);
				REQUIRE(Master(followerTwo) == nullptr);
			}

			THEN("somebody else's follower is left alone")
			{
				REQUIRE(Master(otherFollower) == otherMaster);
			}

			THEN("the master is still perfectly alive")
			{
				REQUIRE(master->valid);
				REQUIRE(Deref(master->self) == master);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a follower whose leader dies becomes its own leader", "[follower]")
{
	GIVEN("a leader with a follower")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *leader = MakeTracker(room);
		CHAR_DATA *follower = MakeTracker(room);

		follower->leader = leader->self;

		WHEN("the leader sheds its group")
		{
			die_follower(leader);

			THEN("the follower leads itself rather than being left leaderless")
			{
				// Intentional game logic, not an oversight: a character with
				// no group still leads one, consisting of itself. Turning this
				// into a null would be a gameplay regression.
				REQUIRE(Leader(follower) == follower);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a secret trailer survives the follower sweep", "[follower]")
{
	GIVEN("a master with one open follower and one trailing it secretly")
	{
		ScopedTrailSkill trailSkill;

		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *openFollower = MakeTracker(room);
		CHAR_DATA *trailer = MakeTracker(room);

		openFollower->master = master->self;
		trailer->master = master->self;
		MakeTrailing(trailer);

		REQUIRE(is_affected(trailer, gsn_trail) == true);

		WHEN("the master sheds its followers while staying alive")
		{
			die_follower(master);

			THEN("the open follower is detached but the trailer is not")
			{
				// The whole point of the skill: the target cannot shake
				// someone who is trailing them. This exemption is correct
				// here, and must survive the field becoming a handle.
				REQUIRE(Master(openFollower) == nullptr);
				REQUIRE(Master(trailer) == master);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("stop_follower clears the master's pet when the pet stops following", "[follower]")
{
	GIVEN("a master with a pet")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *pet = MakeTracker(room);

		pet->master = master->self;
		master->pet = pet->self;

		WHEN("the pet stops following")
		{
			stop_follower(pet);

			THEN("both ends of the relationship are cleared")
			{
				REQUIRE(Pet(master) == nullptr);
				REQUIRE(Master(pet) == nullptr);
			}

			THEN("the pet itself is still alive")
			{
				REQUIRE(pet->valid);
				REQUIRE(Deref(pet->self) == pet);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("a secret trailer does not outlive the master it was trailing", "[follower]")
{
	GIVEN("a master being trailed secretly")
	{
		ScopedTrailSkill trailSkill;

		auto room = MakeTrackingRoom();
		CHAR_DATA *master = MakeTracker(room);
		CHAR_DATA *trailer = MakeTracker(room);

		trailer->master = master->self;
		MakeTrailing(trailer);

		REQUIRE(Master(trailer) == master);

		WHEN("the master is extracted from the world")
		{
			// The sweep in die_follower deliberately skips trailers, and that
			// is right while the master is alive. It is not right once the
			// master has been freed: the reference used to survive into a
			// character struct that the free list hands straight back out, so
			// the trailer silently ended up attached to whoever was allocated
			// next.
			extract_char(master, true);

			THEN("its reference reads as nothing rather than as recycled memory")
			{
				REQUIRE(Master(trailer) == nullptr);
			}

			THEN("the address really was reissued, which is what made this silent")
			{
				CHAR_DATA *newcomer = new_char();

				REQUIRE(newcomer == master);
				REQUIRE(Master(trailer) == nullptr);
			}
		}

		ClearTrackers();
	}
}

//
// Deref(ch->defending) / Deref(ch->analyzePC).
//
// Neither is cleared by extract_char. `defending` is scrubbed only on the
// voluntary-quit path (do_quit_new, just before it extracts the quitter) and
// by do_defend when a character stops defending; `analyzePC` is never cleared
// anywhere. So on any death that is not a quit -- which is most of them --
// both were left pointing into a character struct the free list can hand
// straight back out.
//
// That makes these the thinnest scrub story of any field in the graph: there
// is almost nothing to characterize, because there was almost nothing there.
// What is pinned below is the part that must not change (the reference is
// scoped to its own target and unrelated deaths leave it alone) and the part
// that does (it now expires).
//

namespace
{
	CHAR_DATA *Defending(CHAR_DATA *ch)
	{
		return Deref(ch->defending);
	}

	CHAR_DATA *Analyzing(CHAR_DATA *ch)
	{
		return Deref(ch->analyzePC);
	}
}

SCENARIO("an unrelated death leaves defending and analyzePC alone", "[defending]")
{
	GIVEN("a defender and an analyzer, plus a bystander who dies")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *protector = MakeTracker(room);
		CHAR_DATA *ward = MakeTracker(room);
		CHAR_DATA *analyzer = MakeTracker(room);
		CHAR_DATA *studied = MakeTracker(room);
		CHAR_DATA *bystander = MakeTracker(room);

		protector->defending = ward->self;
		analyzer->analyzePC = studied->self;

		WHEN("somebody else entirely is extracted")
		{
			extract_char(bystander, true);

			THEN("both references are untouched")
			{
				REQUIRE(Defending(protector) == ward);
				REQUIRE(Analyzing(analyzer) == studied);
			}
		}

		ClearTrackers();
	}
}

SCENARIO("defending and analyzePC do not outlive their target", "[defending]")
{
	GIVEN("a defender and an analyzer pointed at one character")
	{
		auto room = MakeTrackingRoom();
		CHAR_DATA *protector = MakeTracker(room);
		CHAR_DATA *analyzer = MakeTracker(room);
		CHAR_DATA *target = MakeTracker(room);

		protector->defending = target->self;
		analyzer->analyzePC = target->self;

		REQUIRE(Defending(protector) == target);
		REQUIRE(Analyzing(analyzer) == target);

		WHEN("the target dies")
		{
			// extract_char clears neither field -- it never did -- so before
			// these became handles both references survived into a recycled
			// character struct. analyzePC is the quieter of the two: every one
			// of its uses is an identity compare, so a recycled address does
			// not crash, it silently matches the wrong character.
			extract_char(target, true);

			THEN("both references read as nothing")
			{
				REQUIRE(Defending(protector) == nullptr);
				REQUIRE(Analyzing(analyzer) == nullptr);
			}

			THEN("they stay nothing even once the address is reissued")
			{
				CHAR_DATA *newcomer = new_char();

				REQUIRE(newcomer == target);
				REQUIRE(Defending(protector) == nullptr);
				REQUIRE(Analyzing(analyzer) == nullptr);
			}
		}

		ClearTrackers();
	}
}
