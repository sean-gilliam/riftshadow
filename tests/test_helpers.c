#include "../code/merc.h"
#include "../code/prof.h"

void TestHelperSetupPlayerBuffer(CHAR_DATA *player, char *name = "player1", char *room_name = "room1")
{
	player->name = name;
	player->pcdata = std::make_unique<pc_data>();
	auto dnew = new descriptor_data();
	dnew->self = descriptorHandles.Add(dnew);	// as new_descriptor would
	player->desc = dnew->self;
	dnew->outbuf = new char[2];
	dnew->outtop = 0;
	dnew->fcommand = false;
	dnew->outsize = 2;
	player->in_room = new room_index_data();
	player->in_room->name = room_name;
}

void TestHelperSetupTrainer(CHAR_DATA *trainer, char *name = "trainer1")
{
	trainer->name = name;
	trainer->pIndexData = new mob_index_data();
	memset(trainer->pIndexData->profs_taught, -1, sizeof(short) * MAX_PROFS_TAUGHT_BY_MOB);
}

void TestHelperCleanupPlayerObject(CHAR_DATA *player)
{
	if (player == nullptr)
		return;

	// ~descriptor_data frees the output buffer and retires the handle, so this
	// is one delete and not the hand-rolled teardown it used to be.
	delete Deref(player->desc);

	if(player->pIndexData != nullptr)
	{
		delete player->pIndexData;
	}

	player->pcdata.reset();

	if (player->in_room != nullptr)
		delete player->in_room;

	delete player;
}