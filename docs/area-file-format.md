# Riftshadow Area File (`.are`) Format

This document describes the on-disk format of area files (`area/*.are`) — the files that
define the game world: mobiles, objects, rooms, resets, programs, and specials. It is a
reference for builders and developers debugging world data.

Everything here is derived directly from the loaders and the OLC writer; line references
point at the authoritative implementation:

- **Section dispatcher:** `boot_db` loop, [mud.c:395](../code/mud.c#L395)
- **Loaders:** [db.c](../code/db.c) and [db2.c](../code/db2.c)
- **Writer (the canonical spec for the write format):** `save_area()`, [olc_save.c:929](../code/olc_save.c#L929)
- **Parsing primitives & flag decode:** [db.c:2573-2763](../code/db.c#L2573)

Reference samples: [area/academy.are](../area/academy.are), [area/area0.are](../area/area0.are).
The master list of areas to load is `area/area.lst`.

> **Naming note:** Riftshadow's section keywords differ from stock ROM/DikuMUD. It uses
> `#MOBS`/`#OBJS` (not `#MOBILES`/`#OBJECTS`), adds `#IMPROGS`/`#SPECS`, and splits resets
> into `#RESETS` (keyword form) vs `#OLDRESETS` (numeric form). See §12 for the full list
> of deviations.

---

## 1. Overall structure

An area file is a flat sequence of `#`-prefixed sections. The boot loop
([mud.c:399](../code/mud.c#L399)) reads a `#`, then a section keyword, and dispatches. It
loops until it reads a keyword starting with `$` (`#$`, end of file).

```
#AREA          (or #AREADATA)   ← header; exactly one, first
#MOBS   ... #0
#OBJS   ... #0
#ROOMS  ... #0
#RESETS ... S                   (or #OLDRESETS ... S)
#IMPROGS ... END
#SPECS  ... END
#$                              ← end-of-file marker
```

Section keyword → loader ([mud.c:407-416](../code/mud.c#L407)):

| Keyword    | Loader           | Notes |
|------------|------------------|-------|
| `#AREA`    | `load_area`      | Positional header (current). |
| `#AREADATA`| `new_load_area`  | Keyed header (legacy/OLC alternate). |
| `#MOBS`    | `load_mobs`      | Mobile prototypes. |
| `#OBJS`    | `load_objs`      | Object prototypes. |
| `#ROOMS`   | `load_rooms`     | Rooms + exits. |
| `#RESETS`  | `load_newresets` | Keyword-argument reset form (current). |
| `#OLDRESETS`| `load_resets`   | Numeric-argument reset form (legacy). |
| `#IMPROGS` | `load_improgs`   | Mob/obj/room/area program bindings. |
| `#SPECS`   | `load_specs`     | Hardcoded C spec-function bindings. |
| `#SOCIALS` | `load_socials`   | Only in `social.are`. |
| `#$`       | breaks loop      | End of file. |

The write order (`save_area`, [olc_save.c:958](../code/olc_save.c#L958)) is: MOBS, OBJS,
ROOMS, RESETS, IMPROGS, SPECS. There is **no** `#SHOPS`, `#SPECIALS`, `#HELPS`, or
`#MOBPROGS` section — shops are inline in mob records, and mob/obj/room programs live in
`#IMPROGS`. An unknown section produces a "Boot_db: bad section name" warning.

---

## 2. Parsing primitives (how fields are tokenized)

Defined in [db.c:2573-2958](../code/db.c#L2573):

- `fread_letter` — skip whitespace, return one char.
- `fread_number` — skip ws, optional `+`/`-`, digits. A trailing `|` means "OR with the
  next number". Saturates at `INT_MAX`; a malformed number is fatal (`exit(1)`).
- `fread_word` — skip ws; if the first char is `'` or `"`, read until the matching quote
  (spaces allowed inside, e.g. `'detect invis'`); otherwise read until whitespace.
- `fread_string` — skip leading ws, read until a `~`. Converts `\n`→`\n\r`, drops bare
  `\r`. An immediate `~` yields the empty string. **The `~` terminates every descriptive
  string.**
- `fread_string_eol` — read until end-of-line (no `~`); used by socials.
- `fread_to_eol` — discard the rest of a line (used to swallow trailing args/comments).
- A leading `*` on a reset/prog line is a comment (skipped to end of line).

---

## 3. Flag encoding

There are **three distinct flag encodings** in area files. Mixing them up is the most
common source of confusion when hand-editing.

### 3a. Named-token lookup (most common)
Flags written as words: `ACT SENTINEL`, `AFF DETECT_INVIS`, `WEAR TAKE`, `ITEM GLOW`,
`ROOM DARK`. `flag_lookup` ([lookup.c:46](../code/lookup.c#L46)) does a case-insensitive
**prefix** match against a named table and returns the bit; `-1` (`NO_FLAG`) on miss.
Because it is prefix-matched, `SENT` still matches `SENTINEL`. Tables live in `tables.c`
(`act_flags`, `affect_flags`, `off_flags`, `imm_flags`, `wear_flags`, `extra_flags`,
`room_flags`, `sect_table`, `direction_table`, `position_table`, etc.).

### 3b. Letter-run bitvector — `fread_flag_new` ([db.c:2700](../code/db.c#L2700))
Used for multi-word bitvectors: **area flags, room exit_info, mob FORM, mob PARTS**. Each
set bit is one letter (`vector_convert`, [db.c:2726](../code/db.c#L2726)):
`A`–`Z` → bits 0–25, `a`–`z` → bits 26–51. A single `0` = no bits; `|` chains runs.
Example: `AC` = bits 0 and 2.

### 3c. Letter-run single long — `fread_flag` ([db.c:2651](../code/db.c#L2651))
Used for **object `value[]` slots that hold flags** (weapon flags, container flags, etc.,
and the generic default value line). Different math (`flag_convert`,
[db.c:2738](../code/db.c#L2738)): each letter is a power of two summed into one integer —
`A`=1, `B`=2, … `Z`=2²⁵, `a`=2²⁶ … `z`=2⁵¹. `0` = none; `|` chains; leading `-` negates.

> Encodings 3b and 3c agree on **which bit** a given letter means (`A`=bit0); they differ
> only in representation (multi-word vector vs single accumulated long). This is the same
> convention used in player files — see [player-file-format.md §3](player-file-format.md).

---

## 4. `#AREA` header

Positional, one field per line; strings end in `~`. Loader: `load_area`
([db.c:721](../code/db.c#L721)). Decoded against [area/academy.are](../area/academy.are):

```
#AREA
academy.are~                        file_name    (fread_string)
The Shalaran Academy~               name
Seikilos~                           credits
1 10                                low_range high_range   (recommended level range)
24500 24599                         min_vnum max_vnum       (vnum bounds)
normal                              area_type    flag_lookup(word, area_type_table)
none                                climate      climate_lookup(word)
0                                   area_flags   fread_flag_new (letter-run bitvector)
9                                   security     (fread_number)
Seikilos Jathruk Zethus Carantoc~   builders
```

- **`credits`** (line 3) and **`builders`** (line 10) are distinct string fields — do not
  confuse them. In the sample, `Seikilos` is the credit line and the last line lists all
  builders.
- `area_type` (`area_type_table`): `normal, road_river, cabal, quest, city, unopened,
  shrine`. `unopened` triggers extra vnum validation on resets; `shrine` auto-sets
  no-summon/no-gate on all its rooms ([db.c:4076](../code/db.c#L4076)).
- On load, the `AREA_LOADING` bit is force-set and `age` defaults to 15.

### `#AREADATA` alternate (legacy)
Keyed rather than positional, terminated by `End` (`new_load_area`,
[db.c:841](../code/db.c#L841)):

```
#AREADATA
Name <text>~
Builders <text>~
Credits <text>~
Security <n>
VNUMs <min> <max>
End
```

Modern saves always use `#AREA`.

---

## 5. `#MOBS` — mobile prototypes

Records repeat until a `#0` vnum line. Loader: `load_mobs`
([db2.c:408](../code/db2.c#L408)); writer: `save_mobile`
([olc_save.c:133](../code/olc_save.c#L133)).

### 5a. Header lines
```
#24500                       vnum           '#' + number (0 ends the section)
blacksmith black smith ...~  player_name    keywords
a dwarven blacksmith~        short_descr
A short, powerfully ...~     long_descr     (multi-line; first char upper-cased)
Powerful muscles ...~        description    (multi-line look text)
0 0 0 dwarf~                 alignment group xp_mod race
```

The `0 0 0 dwarf~` line is: `alignment`, `group`, `xp_mod`, then `race` (a `~`-terminated
string, `race_lookup`).

### 5b. Numeric stat lines ([db2.c:464-531](../code/db2.c#L464))
```
55 1d1+4999 8d8+20 0 100 pound       level  hitdice  damagedice  hitroll  dam_mod  attacktype
0 0 0 0                               ac[PIERCE] ac[BASH] ac[SLASH] ac[EXOTIC]
standing male none                   start_pos  sex  wealth
0 0 medium                           FORM  PARTS  size
```

- **Dice** `XdY+Z`: `X`=number of dice, `Y`=size, `Z`=bonus. Parsed as number / skip `d` /
  number / skip `+` / number.
- **⚠ No mana dice.** Unlike stock ROM, the mob stat line has **only** hit and damage dice.
  Mana is hardcoded to `1d1+99` ([db2.c:489](../code/db2.c#L489)); the ROM mana-read is
  commented out. This is the single biggest deviation from a stock ROM mob record.
- `attacktype` via `attack_lookup` (e.g. `pound`, `pierce`, `none`).
- `start_pos`, `sex`, `size` via their named-table lookups.
- **`wealth`**: the token — whether a numeric literal or a `wealth_table` word — is used as
  an **exponent**, and the stored wealth is `pow(10, token-1)` (values ≤ 0 store 0). A
  numeric token is **not** stored verbatim: e.g. `3` stores `100`, not `3`
  ([db2.c:522-531](../code/db2.c#L522)). The table words map to their index:
  `none`→0, `poor`→1, `moderate`→10, `affluent`→100, `rich`→1000, `peerless`→10000. The
  writer always emits the word form, so numeric literals only appear in hand-edited files.
- `FORM` and `PARTS` are letter-run bitvectors, OR-merged with race defaults.

**Race seeding:** before the flag block is read, `act`/`aff`/`off`/`imm`/`res`/`vuln`/
`form`/`parts` are copied from the mob's **race table** and `ACT_IS_NPC` is force-set
([db2.c:510](../code/db2.c#L510)). The file then stores only the *delta*.

### 5c. Two-token flag block ([db2.c:553](../code/db2.c#L553))
Lines of `KEYWORD VALUE`, each OR-ing one named flag into a field. The loader stays in this
block while the token pair matches one of these prefixes:

| Line | Table | Field |
|------|-------|-------|
| `ACT <flag>`  | `act_flags`    | `act` |
| `AFF <flag>`  | `affect_flags` | `affected_by` |
| `OFF <flag>`  | `off_flags`    | `off_flags` |
| `IMM <flag>`  | `imm_flags`    | `imm_flags` |
| `RES <flag>`  | `imm_flags`    | `res_flags` |
| `VUL <flag>`  | `imm_flags`    | `vuln_flags` |

Two richer sub-blocks also appear here:

- **`CLASS <classname> …`** ([db2.c:607](../code/db2.c#L607)) — sets the mob's class. A
  `warrior` is followed by two combat `style` tokens; a `sorcerer` by two element tokens
  (`ele_major ele_para`).
- **`SPEECH <name>`** ([db2.c:626](../code/db2.c#L626)) — opens a scripted-dialogue block of
  `LINE <delay> <type> <text>~` lines (type via `speech_table`), terminated by `END`.

### 5d. Single-letter directive block ([db2.c:714](../code/db2.c#L714))
After the two-token block, a loop reads one leading letter; an unknown letter is pushed
back (`ungetc`) and ends the record. Each keyword's **first letter is the code**; the rest
of the word is discarded.

| Keyword | Meaning |
|---------|---------|
| `F <field> <flags>` | **Remove** race-granted flags from a field (`act/aff/off/imm/res/vul/for/par`). |
| `A '<skill>' <affflag>` | Spell-affect the mob radiates (`skill_lookup` + `affect_flags`). |
| `C '<spell>'` | Auto-cast spell name. |
| `CA <disc> <cabal>` | Cabal assignment (`cabal_lookup`). |
| `B <criterion> <cmp> <value> <vnum> <msgtype> <msg>~` | **Barred entry** gate (Rift-specific). `cmp` = `EQUALTO`/`LESSTHAN`/`GREATERTHAN`; `msgtype` = `SAY`/`EMOTE`/`ECHO` (ECHO takes two `~` strings). |
| `NOTES <text>~` | Builder notes. |
| `LIMIT <low> <high>` | Level restriction (`restrict_low/high`). |
| `SHOPKEEPER OPEN <h> CLOSE <h> EXIT <dir>` | Inline **shop** (replaces the old `#SHOPS`). |
| `TEACHES <profname>~` | Proficiency taught. |
| `YELL <text>~` | Attack yell. |

Sample: `B level GREATERTHAN 3 24563 SAY Hold, $n...~` and
`SHOPKEEPER OPEN 12 CLOSE 12 EXIT east`.

The record is hashed by `vnum % MAX_KEY_HASH`; a `#0` vnum line ends the section.

---

## 6. `#OBJS` — object prototypes

Records until `#0`. Loader: `load_objs` ([db2.c:1002](../code/db2.c#L1002)); writer:
`save_object` ([olc_save.c:372](../code/olc_save.c#L372)).

### 6a. Header
```
#24507                    vnum       ('#'+number; 0 ends)
warm copper ring~         name       (keywords)
a warm copper ring~       short_descr
A glowing copper ring..~  description
treasure                  item_type  item_lookup(word) → type_flags
copper~                   material   (fread_string; material_lookup)
```

### 6b. Value line — `value[0..4]`, meaning depends on `item_type`
([db2.c:1083](../code/db2.c#L1083) load / [olc_save.c:389](../code/olc_save.c#L389) write):

| item_type | value[0] | value[1] | value[2] | value[3] | value[4] |
|-----------|----------|----------|----------|----------|----------|
| `weapon` | weapon class | num dice | dice type | attack type | weapon flags (`fread_flag`) |
| `container` | capacity | flags (`fread_flag`) | key vnum | max weight | weight mult |
| `drinkcontainer`/`fountain` | capacity | current | liquid | poisoned | — |
| `wand`/`staff` | level | max charges | cur charges | spell | — |
| `potion`/`pill`/`scroll` | level | spell1 | spell2 | spell3 | spell4 |
| `armor` / default | 5 numbers/flags (default branch reads all 5 via `fread_flag`) | | | | |

Spell/liquid/attack values are name tokens (quoted on write) resolved via `skill_lookup` /
`liq_lookup` / `attack_lookup`.

### 6c. Level / weight / cost / condition ([db2.c:1133](../code/db2.c#L1133))
```
1 1 35 P        level  weight  cost  <condition-letter>
```
Condition letter → percent: `P`=100, `G`=90, `A`=75, `W`=50, `D`=25, `B`=10, `R`=0
(default 100).

### 6d. Trailing single-letter directives ([db2.c:1169](../code/db2.c#L1169))
A loop reads a leading letter, discards the remainder of the word, then reads args. An
unknown letter ends the record; a blank line separates records.

| Keyword | Meaning |
|---------|---------|
| `WEAR <flag>` | Wear-location bit (`wear_flags`). |
| `ITEM <flag>` | Extra flag (`extra_flags`) — e.g. `GLOW`, `NOLOCATE`. |
| `RESTRICT <flag>` | Usage restriction (`restrict_lookup`). |
| `APPLY <location> <modifier>` | Stat apply (`apply_locations` + number). |
| `FLAG AFF '<skill>' <affflag> <SHOW\|NOSHOW>` | Char-affect while worn. |
| `FLAG IMM/RES/VUL <flag>` | Object imm/res/vuln (`imm_flags`). |
| `E <keyword>~ <text>~` | Extra description. |
| `MSG WEAR <s1>~ <s2>~` / `MSG REMOVE <s1>~ <s2>~` | Wear/remove echoes. |
| `CABAL <name>` | Cabal binding. |
| `LIMIT <n>` | Global instance limit (`limtotal`). |
| `TIMER <n>` | Start timer. |
| `VERB <word>` | Trigger verb. |
| `NAMEOFLOC <text>~` | Custom wear-location name. |
| `NOTES <text>~` | Builder notes. |

---

## 7. `#ROOMS`

Records until `#0`. Loader: `load_rooms` ([db.c:3999](../code/db.c#L3999)); writer:
`save_rooms` ([olc_save.c:562](../code/olc_save.c#L562)).

### 7a. Header
```
#24576                    vnum
Bloodhawk Meadow~         name          (trailing '.' is chopped)
The meadow opens ...~     description   (multi-line)
field                     sector_type   sect_lookup(word)
```

### 7b. Body — leading-letter directives, terminated by `S` ([db.c:4094](../code/db.c#L4094))

| Letter | Format | Meaning |
|--------|--------|---------|
| `H <n>` | Heal rate (default 100). |
| `M <n>` | Mana rate (default 100). |
| `G <n>` | Guild number. |
| `D <dir> <toVnum> <exitflags> <key>` + `<keyword>~` + `<description>~` | **Exit** (see below). |
| `E <keyword>~ <desc>~` | Extra description. |
| `A <word> <cond> <alt_name>~ <alt_desc>~` | Conditional (ALTDESC) description. |
| `C <word> <cabal>` | Cabal binding. |
| `T <word> <type> <quality> <complexity> <timer> <trig_echo>~ <exec_echo>~` | Trap (`trap_table`; quality ≤ 10). |
| `O <word> <owner>~` | Room owner string. |
| `R <word> <flag>` | Room flag (`room_flags`; special-cases `dark`). |
| `S` | End of room record. |

**Exit (`D`)** ([db.c:4125](../code/db.c#L4125)): `direction` via `direction_table` (must
be 0–5); the destination room vnum; `exit_info` as a letter-run bitvector
(door/closed/locked/pickproof/…); the key vnum; then the `keyword~` and `description~`
strings. Example:
```
D NORTH 24580 0 0
~
The meadow continues to the north.
~
```
= a north exit to room 24580, no exit flags, no key, empty keyword, one-line description.

---

## 8. `#RESETS` — how the world is populated

Current keyword-argument form (`load_newresets`, [db.c:4211](../code/db.c#L4211); writer
`save_resets`, [olc_save.c:870](../code/olc_save.c#L870)), terminated by a lone `S`. Each
line begins with a command letter; the descriptive words (`TO`, `IN`, `GLIMIT`, `LLIMIT`,
`COUNT`) are read and discarded. Placement state is tracked so `GIVE`/`EQUIP`/`PUT` attach
to the most recently loaded mob/object.

| Line (as written) | letter | Meaning |
|-------------------|--------|---------|
| `MOB <vnum> TO <room> GLIMIT <g> LLIMIT <l>` | `M` | Load mob into room, with global + per-room limits. |
| `OBJECT <vnum> TO <room>` | `O` | Load object into room. |
| `PUT <vnum> IN <container> COUNT <n>` | `P` | Put object(s) into a container. |
| `GIVE <vnum>` | `G` | Give object to the last-loaded mob. |
| `EQUIP <vnum> <wearloc>` | `E` | Equip object on the last-loaded mob (`wear_locations`). |
| `DOOR <room> <dir> <OPEN\|CLOSED\|LOCKED>` | `D` | Set a door's initial state. |
| `FOLLOW <leaderVnum> <mobNo>` | `F` | Make the last mob follow a leader. |
| `RANDOMIZE <room> <n>` | `R` | Randomize a room's exits. |

### `#OLDRESETS` (legacy numeric form)
`load_resets` ([db.c:954](../code/db.c#L954)) uses the same command letters but purely
positional numbers: `<letter> <if_flag> <arg1> <arg2> [<arg3>] [<arg4>]`. Door `arg3` is
`0/1/2` = open/closed/locked. Terminated by `S`.

---

## 9. `#IMPROGS` — program bindings

Binds named C program functions to mobs/objects/rooms/the area. Loader: `load_improgs`
([db2.c:253](../code/db2.c#L253)); writer: `save_progs`
([olc_save.c:648](../code/olc_save.c#L648)). Terminated by `END`.

Line format: `<letter> [<vnum>] <progtype> <funcname>`.

| Letter | Target | Format |
|--------|--------|--------|
| `A` | Area | `A <progtype> <funcname>` |
| `M` | Mob | `M <vnum> <progtype> <funcname>` |
| `I` | Object (item) | `I <vnum> <progtype> <funcname>` |
| `R` | Room | `R <vnum> <progtype> <funcname>` |
| `E` | `END` — end section | |

`progtype` selects the trigger (e.g. mob: `greet/give/fight/death/speech/…`; room:
`entry/move/drop/speech/open`; item: `wear/get/drop/invoke/verb/…`; area:
`pulse/reset/sun/tick/aggress`). Each binding sets a bit in the target's `progtypes` mask
and stores the function name. Example: `A tick_prog tick_prog_academy_reset`.

---

## 10. `#SPECS` — hardcoded special functions

Binds compiled C `spec_func`s (from `mspec_table`/`ispec_table`) to mobs/objects. Loader:
`load_specs` ([db2.c:330](../code/db2.c#L330)); writer: `save_specs`
([olc_save.c:821](../code/olc_save.c#L821)). Terminated by `END`.

| Letter | Format | Table |
|--------|--------|-------|
| `M <vnum> <specname>` | Mob special | `mspec_table` |
| `I <vnum> <specname>` | Object special | `ispec_table` |
| `E` | `END` — end section | |

Example: `M 24500 mspec_academy_smith`. An unknown spec name warns but is not fatal.

---

## 11. `#SOCIALS` (only `social.are`)

Not part of normal areas. Loader: `load_socials` ([db2.c:63](../code/db2.c#L63)). Per
social: a name line, then **8 message lines read with `fread_string_eol`**
(line-terminated, **not** `~`-terminated), in order: `char_no_arg`, `others_no_arg`,
`char_found`, `others_found`, `vict_found`, `char_not_found`, `char_auto`, `others_auto`.
A line of `$` means that message is `nullptr`; `#0` as the name ends the section.

---

## 12. Gotchas & Riftshadow-specific notes

- **`~` is the universal string terminator.** An empty string is a bare `~`. Embedded `\r`
  is stripped; `\n` becomes `\n\r`. On write, strings are sanitized so a stray `~`/`\r`
  can't corrupt the terminator.
- **Three flag encodings** (§3). Multi-word bitvectors (area flags, exit_info, FORM, PARTS)
  use `fread_flag_new` (`A`=bit0 positional); object value slots use `fread_flag`
  (`A`=2⁰ power-of-two sum); everything else uses prefix-matched named tokens. They are not
  interchangeable in the file even though 3b/3c agree on bit meaning.
- **Prefix-matched named flags.** `flag_lookup` matches by prefix, so a truncated token
  (`SENT` → `SENTINEL`) still resolves; a genuinely unknown token returns `NO_FLAG` and
  several loaders treat that as fatal.
- **`|` is an OR operator** inside `fread_number`, `fread_flag`, and `fread_flag_new`
  (`A|B|32`).
- **Section keywords are renamed vs stock ROM:** `#MOBS`/`#OBJS`, plus Rift-only
  `#IMPROGS`/`#SPECS` and the `#OLDRESETS`/`#RESETS` split. Reset lines are self-describing
  keywords (`MOB … TO … GLIMIT …`) rather than terse ROM numeric codes.
- **Mobs have no mana dice** — hit + damage dice only; mana is hardcoded `1d1+99`.
- **Mob flags are race-seeded, then edited.** act/aff/off/imm/res/vuln/form/parts start
  from the race table; the file stores only the delta (`ACT/AFF/…` add lines and `F` lines
  that *remove* race-granted bits). A mob record is therefore **not self-contained** — it
  depends on the race tables.
- **Shops are inline in the mob** (`SHOPKEEPER OPEN … CLOSE … EXIT …`); the standalone
  `#SHOPS`/`load_shops`/`save_shops` are stubbed out. **Wealth** is a power-of-ten derived
  from a table index, not a raw gold amount.
- **Rift-only extensions** worth knowing when debugging: mob `B` barred-entry gates,
  `CLASS`+styles/elements, `SPEECH`/`LINE` dialogue, `TEACHES`, `YELL`; room heal/mana/
  guild rates, ALTDESC (`A`), traps (`T`), owners (`O`), per-flag `R` lines; object
  `FLAG AFF/IMM/RES/VUL`, `RESTRICT`, `VERB`, `MSG WEAR/REMOVE`, condition letter.
- **End markers vary by section:** `#0` ends MOBS/OBJS/ROOMS/SOCIALS; a lone `S` ends
  RESETS and each room record; `END` ends IMPROGS/SPECS; `#$` ends the file. Mob/object
  records also end implicitly when the next `#` or an unrecognized directive letter is seen
  (`ungetc`).
- **Duplicate vnums and malformed numbers are fatal** during boot. A malformed number and
  a duplicate mob/object vnum call `exit(1)`; a duplicate room vnum exits via `bugout()`
  with `exit(3)` ([db.c:4034](../code/db.c#L4034)). Either way, the game will not boot.

---

## See also

- [player-file-format.md](player-file-format.md) — the companion reference for `.plr`
  files. The two formats share the flag-encoding conventions and the `~` string terminator.
- [olc.hlp](olc.hlp) — in-game OLC (online creation) command help.
