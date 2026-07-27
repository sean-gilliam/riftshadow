#ifndef ENTITY_CHAR_DATA_H
#define ENTITY_CHAR_DATA_H

#include <time.h>
#include <list>
#include <memory>

#include "fwd.h"
#include "limits.h"
#include "../enums.h"					// CLASS_NONE
#include "../characterClasses/class.h"	// CClass, used by the inline accessors
#include "mob_index_data.h"				// Class() falls back through pIndexData
#include "pc_data.h"					// Profs() reaches into pcdata
#include "gen_data.h"					// GEN_DATA held by unique_ptr in ch->gen_data
#include "mem_data.h"					// MEM_DATA held by value in ch->memory
#include "affect_data.h"				// AFFECT_DATA held by value in ch->affected
#include "../stdlibs/handle.h"			// self, and the handle-typed cross-references

//
// One character (PC or NPC).
//


extern CProficiencies prof_none; //empty proficiencies for jackasses who are going to ref ch->Profs() without checking IS_NPC

class	char_data
{
public:
	// A handle naming this character, issued by new_char and retired by
	// free_char. It is how another entity refers to this one without risking a
	// pointer into a recycled slot; null for a character that never came from
	// new_char.
	Handle<CHAR_DATA> self;
	CHAR_DATA *next;
	CHAR_DATA *next_in_room;
	CHAR_DATA *master;
	CHAR_DATA *leader;
	// The character this one is in combat with. Non-owning, and the opponent
	// can be extracted independently, so it is a handle rather than a pointer.
	// stop_fighting clears it for both sides -- that is about the fight
	// ending, not about lifetime; almost every caller leaves both characters
	// alive.
	Handle<CHAR_DATA> fighting;
	CHAR_DATA *reply;
	CHAR_DATA *pet;
	// The character this mob is hunting. Non-owning, and the quarry can be
	// extracted independently, so it is a handle rather than a pointer. The
	// clears outside extract_char are a mob giving up the hunt -- losing the
	// trail, a peace command -- which is about intent, not lifetime.
	Handle<CHAR_DATA> last_fought;
	int tracktimer;
	time_t last_fight_time;
	char * last_fight_name;
	CHAR_DATA *hunting;
	CHAR_DATA *defending;
	std::list<MEM_DATA> memory;
	GAME_FUN *game_fun;
	MOB_INDEX_DATA *pIndexData;
	DESCRIPTOR_DATA *desc;
	std::list<AFFECT_DATA> affected;
	NOTE_DATA *pnote;
	OBJ_DATA *carrying;
	// The furniture this character is sitting/resting/sleeping on. Non-owning,
	// and the object can be destroyed independently, so it is a handle rather
	// than a pointer. Cleared outright when the character or the object leaves
	// the room -- that is about position, not lifetime.
	Handle<OBJ_DATA> on;
	ROOM_INDEX_DATA *in_room;
	ROOM_INDEX_DATA *was_in_room;
	AREA_DATA *zone;
	std::unique_ptr<PC_DATA> pcdata;
	std::unique_ptr<GEN_DATA> gen_data;
	PATHFIND_DATA *path;	// For smart pathfinding/tracking.  Mob only.
	PATHFIND_DATA *best;	// Stores best direction thus far.  Mob only.
	bool valid;
	char *name;
	char *true_name;
	long id;
	short version;
	char *short_descr;
	char *long_descr;
	char *description;
	char *prompt;
	char *prefix;
	short group;
	short cabal;
	short sex;

	CClass *Class()
	{
		return !my_class
			? (pIndexData && pIndexData->Class()
				? pIndexData->Class()
				:  CClass::GetClass(CLASS_NONE))
			: my_class;
	}

	void		SetClass(int nClassIndex)
	{
		my_class = CClass::GetClass(nClassIndex);
	}

	short race;
	short level;
	short trust;
	int played;
	int lines;							// for the pager
	time_t logon;
	short timer;
	short wait;
	short regen_rate;					// For imbue regeneration spell
	int hit;
	short max_hit;
	short mana;
	short max_mana;
	short move;
	short max_move;
	long gold;
	long gold_bank;
	int exp;
	long act[MAX_BITVECTOR];
	long comm[MAX_BITVECTOR];			// RT added to pad the vector
	long wiznet[MAX_BITVECTOR];			// wiz stuff
	long imm_flags[MAX_BITVECTOR];
	long res_flags[MAX_BITVECTOR];
	long vuln_flags[MAX_BITVECTOR];
	short invis_level;
	short incog_level;
	long affected_by[MAX_BITVECTOR];
	short position;
	short practice;
	short train;
	short carry_weight;
	short carry_number;
	short saving_throw;				//svs
	short alignment;
	short hitroll;
	short damroll;
	short armor[4];
	short wimpy;
	char *speechbuf[10];				// Buffer for various mob speech, progs, etc...

	//
	// stats
	//

	short perm_stat[MAX_STATS];
	short mod_stat[MAX_STATS];

	//
	// parts stuff
	//

	long form[MAX_BITVECTOR];
	long parts[MAX_BITVECTOR];
	short size;
	char *material;

	//
	// mobile stuff
	//

	ROOM_INDEX_DATA *home_room;
	int mobstyle;
	long off_flags[MAX_BITVECTOR];
	short damage[3];
	short dam_type;
	short start_pos;
	short pause;
	short ghost;
	int status;
	long progtypes[MAX_BITVECTOR];
	int hometown;
	float dam_mod;
	short defense_mod;
	bool moved;

	//
	// skill/spell-specific stuff
	//

	short arms;
	short legs;
	short balance;
	short batter;
	CHAR_DATA *analyzePC;
	int analyze;
	short pulseTimer;					// counter for racial combat pulse
	char *backup_true_name;				// Dev's SUPAR CLEVAR CORRUPTION FIX!!!
	float talismanic;
	bool disrupted;						// Has queue-using spell been disrupted?
	int bounty_timer;
	bool law_pass;
	bool stolen_from;
	CProficiencies *Profs() { return pcdata ? &pcdata->profs : &prof_none; }
private:
	CClass *	my_class;
};

#endif /* ENTITY_CHAR_DATA_H */
