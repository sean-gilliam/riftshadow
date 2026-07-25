/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik Stærfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *  ROM 2.4 is copyright 1993-1996 Russ Taylor                             *
 *  ROM has been brought to you by the ROM consortium                      *
 *      Russ Taylor (rtaylor@efn.org)                                      *
 *      Gabrielle Taylor (gtaylor@pacinfo.com)                             *
 *      Brian Moore (zump@rom.org)                                         *
 *  By using this code, you have agreed to follow the terms of the         *
 *  ROM license, in the file Tartarus/doc/rom.license                      *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham         *
 *  In using this code you agree to comply with the Tartarus license       *
 *       found in the file /Tartarus/doc/tartarus.doc                      *
 ***************************************************************************/

#ifdef macintosh
#include <types.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "newmem.h"
#include "misc.h"
#include "utility.h"
#include "direction.h"


/* buffer sizes */
const int buf_size[MAX_BUF_LIST] = {16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384};

DESCRIPTOR_DATA *descriptor_free;
GEN_DATA *gen_data_free;
RUNE_DATA *rune_free;
QUEUE_DATA *queue_free;
OBJ_DATA *obj_free;
CHAR_DATA *char_free;
PC_DATA *pcdata_free;
OLD_CHAR *oldtype_free;

long last_pc_id;
long last_mob_id;

/* stuff for recycling descriptors */
DESCRIPTOR_DATA *new_descriptor(void)
{
	static DESCRIPTOR_DATA d_zero;
	DESCRIPTOR_DATA *d;

	if (descriptor_free == nullptr)
		d = new DESCRIPTOR_DATA;
	else
	{
		d = descriptor_free;
		descriptor_free = descriptor_free->next;
	}

	*d = d_zero;

	d->valid = true;
	return d;
}

void free_descriptor(DESCRIPTOR_DATA *d)
{
	if (!(d != nullptr && d->valid))
		return;

	free_pstring(d->host);

	if (d->outbuf)
		delete[] d->outbuf;

	d->valid = false;
	d->next = descriptor_free;
	descriptor_free = d;
}

/* stuff for recycling gen_data */
GEN_DATA *new_gen_data(void)
{
	static GEN_DATA gen_zero;
	GEN_DATA *gen;

	if (gen_data_free == nullptr)
	{
		gen = new GEN_DATA;
	}
	else
	{
		gen = gen_data_free;
		gen_data_free = gen_data_free->next;
	}

	*gen = gen_zero;

	gen->valid = true;
	return gen;
}

void free_gen_data(GEN_DATA *gen)
{
	if (!(gen != nullptr && gen->valid))
		return;

	gen->valid = false;
	gen->next = gen_data_free;
	gen_data_free = gen;
}

/* Trophy list elements own their victname string (rule-of-5). */
trophy_data::trophy_data(const char *name)
	: victname(name ? palloc_string(name) : nullptr)
{
}

trophy_data::~trophy_data()
{
	free_pstring(victname);
}

trophy_data::trophy_data(const trophy_data &other)
	: victname(other.victname ? palloc_string(other.victname) : nullptr)
{
}

trophy_data &trophy_data::operator=(const trophy_data &other)
{
	if (this != &other)
	{
		char *new_victname = other.victname ? palloc_string(other.victname) : nullptr;

		free_pstring(victname);

		victname = new_victname;
	}

	return *this;
}

trophy_data::trophy_data(trophy_data &&other) noexcept
	: victname(other.victname)
{
	other.victname = nullptr;
}

trophy_data &trophy_data::operator=(trophy_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(victname);

		victname = other.victname;
		other.victname = nullptr;
	}

	return *this;
}

/* mob speech memory management */

/* Speech/line list elements own their strings (freed on destruction; the
   parent std::list handles the unlinking the old free_speech/free_line did). */
line_data::~line_data()
{
	free_pstring(text);
}

speech_data::~speech_data()
{
	free_pstring(name);
}

IPROG_DATA *new_iprog(void)
{
	IPROG_DATA *ipr = new IPROG_DATA;

	ipr->wear_name = nullptr;
	ipr->remove_name = nullptr;
	ipr->get_name = nullptr;
	ipr->loot_name = nullptr;
	ipr->drop_name = nullptr;
	ipr->sac_name = nullptr;
	ipr->sac_name = nullptr;
	ipr->entry_name = nullptr;
	ipr->give_name = nullptr;
	ipr->greet_name = nullptr;
	ipr->fight_name = nullptr;
	ipr->death_name = nullptr;
	ipr->speech_name = nullptr;
	ipr->pulse_name = nullptr;
	ipr->invoke_name = nullptr;
	ipr->open_name = nullptr;
	ipr->look_name = nullptr;
	ipr->verb_name = nullptr;
	ipr->hit_name = nullptr;

	return ipr;
}

RACE_DATA *new_race_data(void)
{
	RACE_DATA *race_specs = new RACE_DATA;

	race_specs->next = nullptr;
	race_specs->first = nullptr;
	race_specs->name = nullptr;
	race_specs->number = -1;

	zero_vector(race_specs->act);
	zero_vector(race_specs->aff);
	zero_vector(race_specs->off);
	zero_vector(race_specs->imm);
	zero_vector(race_specs->res);
	zero_vector(race_specs->vuln);
	zero_vector(race_specs->form);
	zero_vector(race_specs->parts);

	return race_specs;
}

void free_race_data(RACE_DATA *race_specs)
{
	if (race_specs->next == nullptr)
	{
		delete race_specs;
	}
	else
	{
		free_race_data(race_specs->next);
		delete race_specs;
	}
}

PATHFIND_DATA *new_path_data(void)
{
	PATHFIND_DATA *path = new PATHFIND_DATA;

	path->room = nullptr;
	path->evaluated= false;
	path->dir_from = -1;
	path->steps = -1;
	path->prev = nullptr;

	for (int i = 0; i < 6; i++)
	{
		path->dir_to[i] = nullptr;
	}

	return path;
}

void free_path(PATHFIND_DATA *path)
{
	if (!path->dir_to[Directions::North]
		&& !path->dir_to[Directions::East]
		&& !path->dir_to[Directions::South]
		&& !path->dir_to[Directions::West]
		&& !path->dir_to[Directions::Up]
		&& !path->dir_to[Directions::Down])
	{
		delete path;
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (path->dir_to[i])
				free_path(path->dir_to[i]);
		}

		delete path;
	}
}

/* stuff for recycling extended descs -- UGLY */

extra_descr_data::~extra_descr_data()
{
	free_pstring(keyword);
	free_pstring(description);
}

extra_descr_data::extra_descr_data(const extra_descr_data &other)
	: keyword(other.keyword ? palloc_string(other.keyword) : nullptr)
	, description(other.description ? palloc_string(other.description) : nullptr)
{
}

extra_descr_data &extra_descr_data::operator=(const extra_descr_data &other)
{
	if (this != &other)
	{
		char *new_keyword = other.keyword ? palloc_string(other.keyword) : nullptr;
		char *new_description = other.description ? palloc_string(other.description) : nullptr;

		free_pstring(keyword);
		free_pstring(description);

		keyword = new_keyword;
		description = new_description;
	}

	return *this;
}

extra_descr_data::extra_descr_data(extra_descr_data &&other) noexcept
	: keyword(other.keyword)
	, description(other.description)
{
	other.keyword = nullptr;
	other.description = nullptr;
}

extra_descr_data &extra_descr_data::operator=(extra_descr_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(keyword);
		free_pstring(description);

		keyword = other.keyword;
		description = other.description;
		other.keyword = nullptr;
		other.description = nullptr;
	}

	return *this;
}

/* An affect owns its `name` string; the rest of its fields are trivially
 * copied. `owner` is a non-owning back-reference copied as-is. */
static void copy_affect_payload(AFFECT_DATA &dst, const AFFECT_DATA &src)
{
	dst.owner = src.owner;
	dst.where = src.where;
	dst.type = src.type;
	dst.level = src.level;
	dst.duration = src.duration;
	dst.location = src.location;
	dst.modifier = src.modifier;
	dst.mod_name = src.mod_name;
	copy_vector(dst.bitvector, src.bitvector);
	dst.aftype = src.aftype;
	dst.tick_fun = src.tick_fun;
	dst.pulse_fun = src.pulse_fun;
	dst.end_fun = src.end_fun;
	dst.init_duration = src.init_duration;
	dst.beat_fun = src.beat_fun;
}

affect_data::~affect_data()
{
	free_pstring(name);
}

affect_data::affect_data(const affect_data &other)
{
	copy_affect_payload(*this, other);
	name = other.name ? palloc_string(other.name) : nullptr;
}

affect_data &affect_data::operator=(const affect_data &other)
{
	if (this != &other)
	{
		char *new_name = other.name ? palloc_string(other.name) : nullptr;

		free_pstring(name);
		name = new_name;

		copy_affect_payload(*this, other);
	}

	return *this;
}

affect_data::affect_data(affect_data &&other) noexcept
{
	copy_affect_payload(*this, other);
	name = other.name;
	other.name = nullptr;
}

affect_data &affect_data::operator=(affect_data &&other) noexcept
{
	if (this != &other)
	{
		free_pstring(name);

		copy_affect_payload(*this, other);
		name = other.name;
		other.name = nullptr;
	}

	return *this;
}

TRAP_DATA *new_trap(void)
{
	TRAP_DATA *trap = new TRAP_DATA;

	return trap;
}

void free_trap(TRAP_DATA *trap)
{
	free_pstring(trap->exec_echo);
	free_pstring(trap->trig_echo);

	delete trap;
}

RUNE_DATA *new_rune(void)
{
	static RUNE_DATA rune_zero;
	RUNE_DATA *rune;

	if (rune_free == nullptr)
	{
		rune = new RUNE_DATA;
	}
	else
	{
		rune = rune_free;
		rune_free = rune->next;
	}

	*rune = rune_zero;
	return rune;
}

void free_rune(RUNE_DATA *rune)
{
	rune->next = rune_free;
	rune_free = rune;
}

QUEUE_DATA *new_queue(void)
{
	static QUEUE_DATA queue_zero;
	QUEUE_DATA *queue;

	if (queue_free == nullptr)
	{
		queue = new QUEUE_DATA;
	}
	else
	{
		queue = queue_free;
		queue_free = queue->next;
	}

	*queue = queue_zero;
	return queue;
}

void free_queue(QUEUE_DATA *queue)
{
	queue->next = queue_free;
	queue_free = queue;
}

/* stuff for recycling objects */

OBJ_DATA *new_obj(void)
{
	static OBJ_DATA obj_zero;
	OBJ_DATA *obj;

	if (obj_free == nullptr)
	{
		obj = new OBJ_DATA;
	}
	else
	{
		obj = obj_free;
		obj_free = obj_free->next;
	}

	*obj = obj_zero;

	obj->valid = true;
	return obj;
}

void free_obj(OBJ_DATA *obj)
{
	if (!(obj != nullptr && obj->valid))
		return;

	obj->affected.clear();
	obj->charaffs.clear();	// obj owns its charaffs copy now (was shared with the index)
	obj->extra_descr.clear();
	obj->apply.clear();		// obj owns its apply copy now (was shared with the index)

	free_pstring(obj->name);
	free_pstring(obj->description);
	free_pstring(obj->short_descr);

	// free_pstring( obj->owner     );
	obj->valid = false;
	obj->next = obj_free;
	obj_free = obj;
}

/* stuff for recycling characters */

CHAR_DATA *new_char(void)
{
	static CHAR_DATA ch_zero;
	CHAR_DATA *ch;
	int i;

	if (char_free == nullptr)
	{
		ch = new CHAR_DATA;

		// if (bDebug)
		// 	RS.Logger.Debug("Char free is null.  . . . . !");
	}
	else
	{
		ch = char_free;
		char_free = char_free->next;
	}

	*ch = ch_zero;

	ch->valid = true;

	ch->name = &str_empty[0];
	ch->short_descr = &str_empty[0];
	ch->long_descr = &str_empty[0];
	ch->description = &str_empty[0];
	ch->prompt = &str_empty[0];
	ch->prefix = &str_empty[0];
	ch->logon = current_time;
	ch->played = 0;
	ch->lines = PAGELEN;

	for (i = 0; i < 4; i++)
	{
		ch->armor[i] = 0;
	}

	ch->position = POS_STANDING;
	ch->hit = 20;
	ch->max_hit = 20;
	ch->mana = 100;
	ch->max_mana = 100;
	ch->move = 100;
	ch->max_move = 100;
	ch->arms = 2;
	ch->legs = 2;
	ch->regen_rate = 0;
	ch->balance = 0;
	ch->batter = 0;
	ch->analyze = 0;
	ch->talismanic = 0;
	ch->law_pass= false;

	zero_vector(ch->imm_flags);

	for (i = 0; i < MAX_STATS; i++)
	{
		ch->perm_stat[i] = 13;
		ch->mod_stat[i] = 0;
	}

	return ch;
}

void free_char(CHAR_DATA *ch)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if (!(ch != nullptr && ch->valid) || !ch)
		return;

	if (is_npc(ch))
		mobile_count--;

	for (obj = ch->carrying; obj != nullptr; obj = obj_next)
	{
		obj_next = obj->next_content;
		extract_obj(obj);
	}

	for (auto it = ch->affected.begin(); it != ch->affected.end(); )
	{
		auto next = std::next(it);
		it->pulse_fun = nullptr;
		it->tick_fun = nullptr;
		it->end_fun = nullptr;
		affect_remove(ch, &*it);
		it = next;
	}

	ch->memory.clear();

	free_pstring(ch->name);
	free_pstring(ch->short_descr);
	free_pstring(ch->long_descr);
	free_pstring(ch->true_name);
	free_pstring(ch->description);
	free_pstring(ch->prompt);
	free_pstring(ch->prefix);

	if (ch->pcdata != nullptr)
		free_pcdata(ch->pcdata);

	ch->next = char_free;
	char_free = ch;

	ch->valid = false;
}

PC_DATA *new_pcdata(void)
{
	int alias;

	static PC_DATA pcdata_zero;
	PC_DATA *pcdata;

	if (pcdata_free == nullptr)
	{
		pcdata = new PC_DATA;
	}
	else
	{
		pcdata = pcdata_free;
		pcdata_free = pcdata_free->next;
	}

	*pcdata = pcdata_zero;

	for (alias = 0; alias < MAX_ALIAS; alias++)
	{
		pcdata->alias[alias] = nullptr;
		pcdata->alias_sub[alias] = nullptr;
	}

	pcdata->buffer = new BUFFER;

	pcdata->valid = true;

	pcdata->trusting = nullptr;
	pcdata->death_status = 0;
	return pcdata;
}

OLD_CHAR *new_oldchar(void)
{
	static OLD_CHAR oldtype_zero;
	OLD_CHAR *oldtype;

	if (oldtype_free == nullptr)
	{
		oldtype = new OLD_CHAR;
	}
	else
	{
		oldtype = oldtype_free;
		oldtype_free = oldtype_free->next;
	}

	*oldtype = oldtype_zero;
	return oldtype;
}

void free_oldchar(OLD_CHAR *old)
{
	free_pstring(old->name);
	free_pstring(old->short_descr);
	free_pstring(old->long_descr);
	free_pstring(old->description);

	old->next = oldtype_free;
	oldtype_free = old;
}

void free_pcdata(PC_DATA *pcdata)
{
	if (!(pcdata != nullptr && pcdata->valid))
		return;

	free_pstring(pcdata->pwd);
	free_pstring(pcdata->bamfin);
	free_pstring(pcdata->bamfout);
	free_pstring(pcdata->title);
	delete pcdata->buffer;

	for (int i = 0; i < 100; i++)
	{
		if (pcdata->recentkills[i] != nullptr)
			free_pstring(pcdata->recentkills[i]);
	}

	pcdata->trophy.clear();

	/*
	for (alias = 0; alias < MAX_ALIAS; alias++)
	{
		if(pcdata->alias[alias] != nullptr)
		{
			free_pstring(pcdata->alias[alias]);
			free_pstring(pcdata->alias_sub[alias]);
		}
	}
	*/

	pcdata->valid = false;

	pcdata->next = pcdata_free;
	pcdata_free = pcdata;
}

/* stuff for setting ids */

long get_pc_id(void)
{
	int val;

	val = (current_time <= last_pc_id) ? last_pc_id + 1 : current_time;
	last_pc_id = val;
	return val;
}

long get_mob_id(void)
{
	last_mob_id++;
	return last_mob_id;
}

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
int get_size(int val)
{
	int i;

	for (i = 0; i < MAX_BUF_LIST; i++)
		if (buf_size[i] >= val)
		{
			return buf_size[i];
		}

	return -1;
}

buf_type::~buf_type()
{
	if (string)
		free_pstring(string);
}

bool buf_type::add(const char *text)
{
	int len;
	char *tptr;

	if (text[0] == '\0' || text == nullptr)
		return false;

	if (!string || !strlen(string)) // like a virgin.. touched for the very first tiiiiime
	{
		string = palloc_string(text);
		size = strlen(text) + 1;
		return true;
	}

	len = strlen(string) + strlen(text) + 1;

	if (len > 32766)
		return true;

	tptr = string;
	string = new char[len];

	if (!string)
		return false;

	size = len;

	strcpy(string, tptr);
	strcat(string, text);

	delete[] tptr;
	return true;
}

void buf_type::clear()
{
	free_pstring(string);
	string = nullptr;
	state = BUFFER_SAFE;
}

const char *buf_type::str() const
{
	return string;
}
