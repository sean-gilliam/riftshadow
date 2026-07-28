# Riftshadow Player File (`.plr`) Format

This document describes the on-disk format of player save files (`player/<Name>.plr`).
It is a reference for developers and builders debugging player data. Everything here is
derived directly from the load/save code in [code/save.c](../code/save.c); line
references point at the authoritative implementation.

- **Written by:** `save_char_obj()` → `fwrite_char()` / `fwrite_obj()` / `fwrite_pet()` / `fwrite_charmie()` ([save.c:90](../code/save.c#L90))
- **Read by:** `load_char_obj()` → `fread_char()` / `fread_obj()` / `fread_pet()` / `fread_charmie()` ([save.c:908](../code/save.c#L908))
- **Location:** `RIFT_PLAYER_DIR/<Capitalized-Name>.plr`
- **Current version:** `6` (the `Vers` field, [save.c:249](../code/save.c#L249); `VERSION` in [merc.h:227](../code/merc.h#L227))

Players are **not** saved when they are an NPC or when the game is running on port
4000 (the test port) — see [save.c:97](../code/save.c#L97).

---

## 1. Overall structure

A `.plr` file is a sequence of **sections**, each introduced by a `#`-prefixed keyword
on its own token. The dispatcher is `load_char_obj()` ([save.c:1029](../code/save.c#L1029)).

| Section     | Meaning                                          | Reader            |
|-------------|--------------------------------------------------|-------------------|
| `#PLAYER`   | The character record itself (always first)       | `fread_char`      |
| `#O`        | A carried/worn object (repeats; may nest)        | `fread_obj`       |
| `#OBJECT`   | Legacy alias for `#O`                             | `fread_obj`       |
| `#PET`      | The character's charmed pet (0 or 1)             | `fread_pet`       |
| `#CHARMED`  | A saved charmie (undead/necromancer minions)     | `fread_charmie`   |
| `#END`      | End-of-file marker; stops the loader             | —                 |

Example skeleton (see [player/Zorro.plr](../player/Zorro.plr) for a real file):

```
#PLAYER
Name Zorro~
... fields ...
End

#O
Vnum 24571
... object fields ...
End

#PET
Vnum 80
... pet fields ...
End
#END
```

**Write order** ([save.c:134-159](../code/save.c#L134)): `#PLAYER`, then all carried
objects, then the pet (only if it is in the same room as the player), then any charmed
undead minions, then `#END`.

### Section body grammar

Inside a section, each line is `Keyword value(s)`. The reader loops reading a *word*
(`fread_word`), switches on `UPPER(word[0])`, and matches the full keyword. The section
terminator is the literal word `End` (note: **`End`**, not `#END` — the `#END` at the
very bottom is the whole-file terminator). An unrecognized keyword is skipped to
end-of-line (`fread_to_eol`), so unknown/legacy fields are silently ignored rather than
causing a load failure.

Two parser conventions from [merc.h:63-92](../code/merc.h#L63):

- `KEY("Literal", field, value)` — matches a keyword and assigns a scalar/string.
- `KEYV("Literal", field)` — matches a keyword and reads a **flag vector** (`fread_flag_new`).

---

## 2. String and number conventions

- **Strings** end with a tilde `~` (`fread_string`). A string may span multiple lines;
  everything up to the next `~` is the value. Example:
  ```
  Desc Obscured within a cloud of smoke...
  hovers near its master.
  ~
  ```
- **Words** (`fread_word`) are whitespace/quote-delimited single tokens. Skill and group
  names are written single-quoted, e.g. `Sk 59 'dagger'`.
- **Numbers** (`fread_number`) are plain integers; a leading `-` is allowed and `|` can
  chain values. Overflow saturates at `INT_MAX`.
- Fields marked below as "written only if non-default" are **omitted** from the file when
  they equal the default/prototype value, so a given `.plr` will not contain every key.

---

## 3. Flag encoding (shared by the whole game)

Many fields are bit flags rendered as **letter runs**. This is the single most important
convention to understand when reading these files by eye.

**Writing** — `print_flags()` ([save.c:38](../code/save.c#L38)) emits one letter per set
bit, by bit **index**:

| Bit index | 0 | 1 | 2 | ... | 25 | 26 | 27 | ... | 51 |
|-----------|---|---|---|-----|----|----|----|-----|----|
| Letter    | A | B | C | ... | Z  | a  | b  | ... | z  |

So `Act BCDGIQ` means bits **1, 2, 3, 6, 8, 16** are set. An empty flag set is written as
the single character `0`.

**Reading** — `fread_flag_new()` ([db.c:2700](../code/db.c#L2700)) reverses this
(`vector_convert`, [db.c:2726](../code/db.c#L2726)): `A`–`Z` → bits 0–25, `a`–`z` → bits
26–51. A leading `0` means "no bits". (There is also an older single-`long` variant,
`fread_flag`/`flag_convert`, used mainly by area files; it maps the *same letters to the
same bits*, just accumulated into one integer.)

To decode a specific field you look up the bit **index** in the relevant table in
[merc.h](../code/merc.h) — e.g. `Act` uses the `PLR_*` constants, `Comm` uses `COMM_*`,
`AfBy` uses `AFF_*`.

---

## 4. `#PLAYER` section — field reference

Written by `fwrite_char()` ([save.c:235](../code/save.c#L235)); read by `fread_char()`
([save.c:1141](../code/save.c#L1141)). Many keys have both a long historical name and a
short name (the reader accepts both; the writer only emits the short one). Both are listed.

### 4.1 Identity & meta

| Key | Values | Meaning |
|-----|--------|---------|
| `Name` | string~ | Character's true name (also the filename). |
| `Ghost` | int~ | Ghost/decay timer state. |
| `Id` | long | Unique player id. |
| `LogO` | long | Last logoff time (Unix epoch). On load this drives HP/mana/move regen for time offline, [save.c:1435](../code/save.c#L1435). |
| `Vers` / `Version` | int | Save format version (currently 6). Gates several fields — see §6. |
| `ShD` | string~ | Short description (immortal/polymorph). |
| `LnD` | string~ | Long description. |
| `Desc` | string~ | Full look description. |
| `Prom` | string~ | Custom prompt string. |
| `Race` | string~ | Race name (`race_lookup`). |
| `Cla` | string~ | Class name (`CClass::Lookup`). |
| `Cabal` | string~ | Cabal name (`cabal_lookup`). Only written if the player is in a cabal ([save.c:274](../code/save.c#L274)). |
| `CabalLevel` | int | Rank/level within the cabal (paired with `Cabal`). |
| `Tribe` | int | Horde tribe id. Only written when the cabal is `CABAL_HORDE` ([save.c:280](../code/save.c#L280)). |
| `Newbie` | int~ | Newbie flag (reset to false at the start of every load, [save.c:1158](../code/save.c#L1158)). |
| `Sex` | int | Current sex (0 neuter / 1 male / 2 female). |
| `TSex` / `TrueSex` | int | True sex (before `change sex` etc.). |
| `Beauty` | int | Beauty stat. |
| `Levl` / `Level` / `Lev` | int | Level. |
| `LLev` / `LastLevel` | int | Last level (for de-leveling checks). |
| `Tru` / `Trust` | int | Trust/admin override level (omitted if 0). |
| `Sec` | int | OLC security clearance. |
| `Plyd` / `Played` | int | Total seconds played. |
| `Titl` / `Title` | string~ | Player title (a leading space is auto-added unless it starts with punctuation, [save.c:1753](../code/save.c#L1753)). |
| `EXTitl` / `EXTitle` | string~ | Extended title. |
| `Pass` / `Password` | string~ | Encrypted password. |
| `Bin` / `Bamfin` | string~ | Custom teleport-in ("bamf") message (immortals). |
| `Bout` / `Bamfout` | string~ | Custom teleport-out message. |
| `Dmsg` | string~ | Custom immortal death message. |
| `Role` | string~ | Player-written roleplay bio. |
| `History` | string~ | Buffered communication history. |
| `LogonTime` | string~ | Human-readable last logon (write-only, see §7). |
| `TimePlayed` | int~ | Minutes this session (write-only, informational). |

### 4.2 Location, vitals, stats

| Key | Values | Meaning |
|-----|--------|---------|
| `HomeTown` | int | Hometown/recall id. |
| `Room` | int | Room vnum to load into (falls back to Limbo if the room is gone). |
| `HMV` / `HpManaMove` | 6 ints | `hit max_hit mana max_mana move max_move`. |
| `HMVP` / `HpManaMovePerm` | 3 ints | Permanent `hit mana move` (trained maxes). |
| `Attr` / `AttrPerm` | 5 ints | Permanent stats: `STR INT WIS DEX CON`. |
| `AMod` / `AttrMod` | 5 ints | Stat modifiers (same order). |
| `ACs` | 4 ints | Armor class: pierce, bash, slash, exotic (`AC_PIERCE/BASH/SLASH/EXOTIC`). The 4th slot is shown to players in-game as "magic", [act_info.c:2593](../code/act_info.c#L2593). |
| `Hit` / `Hitroll` | int | Hitroll (omitted if 0). |
| `Dam` / `Damroll` | int | Damroll (omitted if 0). |
| `Save` / `SavingThrow` | int | Saving throw (omitted if 0). |
| `Alig` / `Alignment` | int | Alignment (-1000..1000). |
| `Etho` | int | Ethos. |
| `OAli`, `OEth` | int | "Original" align/ethos (pre-corruption). |
| `Indu` | int | Cabal induction counter. |
| `Wimp` / `Wimpy` | int | Wimpy flee threshold (omitted if 0). |
| `Pos` / `Position` | int | Position enum (see §8). `FIGHTING` is saved as `STANDING`. |
| `Prac` / `Practice` | int | Practice sessions (omitted if 0). |
| `Trai` | int | Training sessions (omitted if 0). |
| `Scro` | int | Scroll/pager line count. |
| `Cnd` / `Condition` | 6 ints | Conditions: drunk, full, thirst, hunger, starving, dehydrated (`COND_*`, [merc.h:2111](../code/merc.h#L2111)). The last two only exist in v6 — see §6. |

### 4.3 Currency, experience, progression

| Key | Values | Meaning |
|-----|--------|---------|
| `Gold` | long | Gold on hand (written as 0 if none). |
| `Bgold` | long | Banked gold. |
| `Exp` | int | Experience points. |
| `Sp` | int | Spendable stat/skill points. |
| `Deaths` | int | Death count. |
| `Died` | int | Death status (only written when `HAS_DIED`). |
| `Dtime` | int | Death timer (paired with `Died`). |
| `DeathTime` | int | Aggregate death time. |
| `RollTime` | long | When the character was last re-rolled. |
| `Born` | int | Birth date (in-game). |
| `Agemod` | int | Age modifier. |
| `Rep` | int | Reputation. |
| `Special` | int | Class "special" resource. |
| `Bounty` | long | Bounty on this player. |
| `BountyKilled` | int | Times killed as a bounty target. |
| `BCredits` | int | Bounty credits earned. |
| `Paladin_path` | int | Paladin subclass path. |
| `EleMaj`, `ElePar` | int | Sorcerer major/paragon element (only for sorcerers). |
| `Souls` | int | Necromancer soul count (only for necromancers). |
| `StartWeap` | int | Starting weapon skill index (omitted if < 0). |
| `ProfPoints` | 2 ints | Proficiency points + proficiency-advance timer. |
| `Prof` | string~ int | One proficiency: name + level (repeats). |

### 4.4 Flag fields (letter-encoded — see §3)

| Key | Constants | Meaning |
|-----|-----------|---------|
| `Act` | `PLR_*` | Player action flags (autoloot, holylight, deny, etc.). Omitted if empty. |
| `AfBy` / `AffectedBy` | `AFF_*` | Currently-affected-by flags. Omitted if empty. |
| `Comm` | `COMM_*` | Communication/channel toggles. |
| `Wizn` | `WIZ_*` | Wiznet subscription (immortals; omitted if empty). |
| `Immune` | `IMM_*` | Immunity flags. |
| `Resist` | `RES_*` | Resistance flags. |
| `Vuln` | `VULN_*` | Vulnerability flags. |
| `Styles` | style bits | Known combat styles. |
| `TrSet` | trust bits | Granted trust/permission set. |
| `Invi` / `InvisLevel` | int | Wizinvis level (omitted if 0). |
| `Inco` | int | Incognito level (omitted if 0). |
| `Instyle` | int | Currently active style index. |

> Note: on load, race-granted `imm/res/vuln/form/parts` are re-applied from the race
> table and OR-ed with the saved `Immune/Resist/Vuln` values ([save.c:1113-1124](../code/save.c#L1113)).

### 4.5 Repeating / list fields

| Key | Values | Meaning |
|-----|--------|---------|
| `Sk` / `Skill` | int `'name'` | Learned skill: percent + quoted skill name. One line per skill. Value `-2` is a *remembered-but-suppressed* sentinel (e.g. a revoked cabal skill): it is written so it round-trips, but `get_skill` treats it as 0% ([handler.c:518](../code/handler.c#L518), [dioextra.c:927](../code/dioextra.c#L927)). Only skills with `learned > 0` **or** `== -2` are saved ([save.c:563](../code/save.c#L563)). |
| `Gr` / `Group` | `'name'` | A known skill group. Adds all its skills. |
| `Alias` / `Alia` | word string~ | A command alias: keyword + expansion. Up to `MAX_ALIAS`. |
| `Quest` | int int | `index value` for a completed/active quest slot. |
| `CScheme` | word word | Color scheme: `event_name color_name` (one line per event). |
| `Not` | 5 longs | Last-read timestamps: note, idea, penalty, news, changes. |
| `Sect` | ints `-1` | Time spent in each sector type, one number per slot in `sect_time[]` index order, `-1` terminated. **Note:** the first number is slot 0, which has no `SECT_*` constant (the named sectors in §8 start at value 1), so the Nth number (0-indexed) is the time for sector *value* N. |
| `DeposItems` | ints | Vnums of items in bank deposit (`MAX_STORED_ITEMS`). |
| `kls` | 4 ints | PK kills: total, good, neutral, evil. |
| `frg` | 4 ints | PK frags ×1,000,000 (stored as fixed-point). |
| `kld` | 2 ints | Times killed: by-PC, by-mob. |
| `frgd` | int | Times fragged ×1,000,000. |
| `rkls` … `EndRKLS` | words | Recent-kill victim names (up to 100). |
| `Lssr` / `Grtr` / `Arch` | ints | Anti-paladin lesser/greater/arch devil data (anti-paladins only). |
| `Trophies` … `XYZ` | int names | Horde trophy-belt scalps (count then names; `XYZ` sentinel). |
| `FingEQ` | ints | Vnums worn in each `MAX_WEAR` slot (write-only, see §7). |
| `TrackAObj`, `TrackLObj` | int~ | Carried-object counts (write-only, informational). |

### 4.6 Affects (`Affc`)

Each active magical/skill affect is one `Affc` line ([write save.c:584](../code/save.c#L584),
[read save.c:1231](../code/save.c#L1231)):

```
Affc '<skill>' <where> <level> <duration> <modifier> <location> <bitvector> <aftype> <owner> '<name>'
```

| Field | Meaning |
|-------|---------|
| `'<skill>'` | Skill/spell name (`skill_lookup`). |
| `<where>` | Target field: `TO_*` — 0 affects, 1 object, 2 immune, 3 resist, 4 vuln, 5 weapon ([merc.h:843](../code/merc.h#L843)). |
| `<level>` | Caster level. |
| `<duration>` | Ticks remaining (`-1` = permanent). |
| `<modifier>` | Magnitude applied to `location`. |
| `<location>` | `APPLY_*` stat/attribute affected ([merc.h:1770](../code/merc.h#L1770)). |
| `<bitvector>` | Letter-encoded `AFF_*` bits set while active. |
| `<aftype>` | `AFT_*` — 0 spell, 1 skill, 2 power, 3 malady, 4 commune, 5 invis, 6 rune, 7 timer ([merc.h:792](../code/merc.h#L792)). |
| `<owner>` | Name of the caster/owner, or `none`. |
| `'<name>'` | Custom affect name, or `'none'`. |

The example from Zorro: `Affc 'outfit' 0 0 67 0 0 Z 2 none 'none'` → the *outfit* affect,
on the character (`where`=0), 67 ticks left, `AFF` bit 25 (`Z`), aftype 2 (power),
no owner, unnamed.

Affects with type `word of command`, `disguise`, or `indomitable spirit` are deliberately
**not** saved ([save.c:579](../code/save.c#L579)).

---

## 5. `#O` object section — field reference

Written by `fwrite_obj()` ([save.c:754](../code/save.c#L754)); read by `fread_obj()`
([save.c:2038](../code/save.c#L2038)).

**Key design point:** an object is stored as a *diff against its prototype*
(`pIndexData`). A field is written **only if it differs** from the vnum's template. This
is why a real `#O` block (like Zorro's) is short — most fields inherit from the area file.

| Key | Values | Meaning |
|-----|--------|---------|
| `Vnum` | int | Object prototype vnum (should be the first key). |
| `Oldstyle` | (flag) | Present if the prototype is old-format; forces a fresh clone on load. |
| `Nest` | int | Container nesting depth (0 = top level). See §5.1. |
| `Name` | string~ | Keywords (only if changed from proto). |
| `ShD` | string~ | Short description (only if changed). |
| `Desc` | string~ | Ground/long description (only if changed). |
| `Owner` | string~ | Owner name (personalized/quest items). |
| `ExtF` / `ExtraFlags` | flags | `ITEM_*` extra flags (glow, hum, nodrop, …), letter-encoded. |
| `WeaF` / `WearFlags` | flags | Wearable-location flags, letter-encoded. |
| `Ityp` / `ItemType` | int | Item type override (`ITEM_*`, see §8). |
| `Wt` / `Weight` | int | Weight (if changed). |
| `Mat` | string~ | Material name (if changed). |
| `WLName` | string~ | Custom wear-location display name. |
| `Cond` | int | Condition/durability (0-100ish). |
| `Wear` / `WearLoc` | int | Currently worn slot (`WEAR_*`; `-1` = carried, not worn). |
| `Lev` / `Level` | int | Object level (if changed). |
| `Time` / `Timer` | int | Decay timer (omitted if 0). |
| `Cost` | int | Value in gold. |
| `Val` | 5 ints | `value[0..4]` — type-specific slots. |
| `Vals` / `Values` | 4 ints | Legacy 4-value form (read only). |
| `Spell` | int `'name'` | Spell baked into a slot: `slot(1-3) skillname` (potions/scrolls/pills/wands/staves). |
| `Affc` | … | Object affect, same layout as §4.6 but with no trailing `'name'` field ([save.c:867](../code/save.c#L867)). |
| `AddApp` | 3 ints | Extra apply added beyond the prototype: `location modifier type`. |
| `ExDe` / `ExtraDescr` | string~ string~ | An added extra description: `keyword~ text~`. |

**Not saved:** keys (`ITEM_KEY`) and empty maps are dropped on save
([save.c:770](../code/save.c#L770)) — they are re-created from resets rather than persisted.

### 5.1 Object nesting (`Nest`)

Containers and their contents are flattened using the `Nest` depth counter. The writer
recurses so that, on read, items load front-to-back ([save.c:764](../code/save.c#L764)).
On load, an object with `Nest N` (N>0) is placed inside the most recent object recorded at
depth `N-1` via the `rgObjNest[]` stack ([save.c:2249](../code/save.c#L2249)). `MAX_NEST`
bounds the depth.

---

## 6. Version differences (`Vers`)

The `Vers`/`Version` field selects backward-compatible behavior:

- **`Cnd` / `Condition`** — versions **> 5** store **6** condition values; older files store
  4 and the last two default to 0 ([save.c:1348-1374](../code/save.c#L1348)).
- **version < 2** — on load, the class base/default skill groups and `recall` at 50% are
  granted to backfill skills added after the file was written
  ([save.c:1128](../code/save.c#L1128)).

New saves always use version 6.

---

## 7. Gotchas & asymmetries

- **Diff-against-prototype (objects & pets).** `#O` and `#PET` records only contain fields
  that differ from the prototype mob/object. Editing an area file's prototype changes what
  a player "inherits" on next load. Do not assume a missing key means a zero value — it
  means "same as prototype".
- **Write-only / informational fields.** Several fields are written by `fwrite_char` but
  have **no reader** in `fread_char`, so they are ignored on load and exist only for
  external tooling / eyeballing: `FingEQ`, `LogonTime`, `TimePlayed`, `TrackAObj`,
  `TrackLObj`. Do not rely on them round-tripping.
- **`End` vs `#END`.** `End` closes a section; `#END` closes the file. A stray `#END`
  inside a section, or a missing `End`, will desync the loader.
- **Fixed-point frags.** `frg`/`frgd` are floats multiplied by 1,000,000 on write and
  divided back on read — expect large integers in the file.
- **Position normalization.** A player caught mid-fight is saved as `STANDING`, never
  `FIGHTING` ([save.c:380](../code/save.c#L380)).
- **Regen on load.** HP/mana/move are boosted on load proportional to time offline (capped
  at 100%), unless poisoned/plagued ([save.c:1435](../code/save.c#L1435)).
- **Unknown keys are ignored**, so a typo in a hand-edited file usually fails silently
  (the line is skipped) rather than erroring.
- **Pet affects are not saved** — the `Affc` loop in `fwrite_pet` is commented out and
  flagged "VARY VARY BAD" ([save.c:725](../code/save.c#L725)).

---

## 8. Reference enums

Quick decode tables. Authoritative source is [merc.h](../code/merc.h).

**Positions** (`Pos`): 0 dead · 1 mortal · 2 incap · 3 stunned · 4 sleeping · 5 resting ·
6 sitting · 7 fighting · 8 standing.

**Wear locations** (`Wear`, and `FingEQ`/`Sect` ordering): -1 none · 0 light · 1 finger_L ·
2 finger_R · 3 neck_1 · 4 neck_2 · 5 body · 6 head · 7 legs · 8 feet · 9 hands · 10 arms ·
11 shield · 12 about · 13 waist · 14 wrist_L · 15 wrist_R · 16 wield · 17 hold ·
18 dual_wield/float · 19 brand · 20 strapped · 21 cosmetic.

**Item types** (`Ityp`): 1 light · 2 scroll · 3 wand · 4 staff · 5 weapon · 8 treasure ·
9 armor · 10 potion · 11 clothing · 12 furniture · 13 trash · 15 container · 17 drink_con ·
18 key · 19 food · 20 money · 22 boat · 23 corpse_npc · 24 corpse_pc · 25 fountain ·
26 pill · 27 protect · 28 map · 29 portal · 30 warp_stone · 31 room_key · 32 gem ·
33 jewelry · 34 campfire · 35 cabal_item · 36 skeleton · 37 urn · 38 gravitywell ·
39 book · 40 pen · 41 altar · 43 stone.

**Sectors** (`Sect` order): 1 city · 2 field · 3 forest · 4 hills · 5 mountain · 6 water ·
7 inside · 8 underwater · 9 air · 10 desert · 11 road · 12 conflagration · 13 burning ·
14 trail · 15 swamp · 16 park · 17 vertical · 20 cave.
