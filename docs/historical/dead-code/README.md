# The 2016 C++ Migration Attempt

These headers are from a previous attempt at the C→C++ migration, added in `1a7511c` (2016-11-03, the
repository's first commit). About 30 classes were declared in `classlist.h`; roughly five were ever
defined, and **nothing here was ever instantiated by the running game**. `CRoom::first` in `mud.c`
existed solely to satisfy the linker for a class nothing constructed.

They are preserved because the *design thinking* is worth reading — `character.h` in particular is a
serious piece of work — and because `mud.h`'s commented-out `GameEngine` / `Interpreter` members
describe a destination the current migration still broadly agrees with.

## Do not reintroduce these

Three of these headers carry constants that **conflict with live definitions in `merc.h`**. All were
dormant only because of include-graph accidents, and re-adding any of these files could arm them.

| Constant | Value here | Value in `merc.h` | Status when archived |
|---|---|---|---|
| `ACT_IS_NPC` | `1` — `autogen/stddefs.h:32` | `0` — `merc.h:1089` | Latent. `stddefs.h` had zero includers, and `mud.c:9` carried `//#include "autogen/fundefs.h()"` — one uncommented line from an NPC-flag bug. |
| `WIZ_LINKS` | `1` — `socket.h:10` | `4` — `merc.h:2259` | **Live collision.** `socket.h` really was compiled on Linux (`mud.c:6`, in the `#else` of an `#ifdef _WIN32`). |
| `MAX_STRING_LENGTH` | `4096` — `file.h:17` | `4608` — `merc.h:121` | Latent, but `mud.c` saw both. Defused by the same `#undef` trick; see the `CFile` section below. |

The `WIZ_LINKS` case left visible scar tissue. `merc.h` used to guard it with:

```cpp
#ifdef WIZ_LINKS
#undef WIZ_LINKS
#define WIZ_LINKS   4
#else
#define WIZ_LINKS   4
#endif
```

Both branches produce `4`. That `#undef` existed for no reason other than to defeat `socket.h`'s `1`
— someone hit the collision and worked around the symptom rather than removing the source. Once
`socket.h` was archived the guard was dead, and `6edd02b` collapsed it to a plain `#define`
(`WIZ_LINKS` has since become an enumerator in `enums.h`). `MAX_STRING_LENGTH` below was cleared the
same way.

## Added later: `race.h` and `skill.h` (2026-08-01)

These two were missed when the rest of the set was archived, and their continued presence in `code/`
was later mistaken for evidence that something used them. It was not. `CRace` and `CSkill` each have
**zero method definitions and zero instantiations**, and every mention outside their own header is
commented out (`mud.c:472`, `mud.h:83-84`). Neither header carried anything else worth rescuing —
`race.h`'s `MAX_STAT` is used only by `CRace::max_stats` itself, and `skill.h` is nothing but the
class.

They came out with **no constant collisions** (unlike `socket.h` and `stddefs.h` above) and **no call
sites touched**: four vestigial `#include` lines were dropped, two of them in `handler.c` and
`magic.c`, which include `skill.h` **and** the live, unrelated `skills.h` two lines apart. Nothing in
either file references `CSkill`.

**A header still sitting in `code/` is not evidence that the class it declares is used** — the same
conflation the survey note below warns about, arriving from the opposite direction.

## Added later: `file.h` and `file.c` — `CFile` (2026-08-01)

From the same first commit as everything else here, and dead the same way: `CFile` appears nowhere
outside its own declaration and definition. `mud.c` included `stdlibs/file.h` but never named the
class — and said so, in a comment at `CMud::LoadGreetingScreen` explaining it *"can't use cfile
because of weird \r action."* This is the first `.c` in the archive; the rest are headers.

**It was the source of a third constant collision, and this one had left the same scar tissue as
`WIZ_LINKS`.** `file.h` defined `MAX_STRING_LENGTH` as **4096**; `merc.h` defines it as **4608**, and
carried this to defend itself:

```cpp
#ifdef MAX_STRING_LENGTH
#undef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH			4608
#else
#define MAX_STRING_LENGTH			4608
#endif
```

Both branches produce `4608` — the identical shape, and the identical workaround-the-symptom
decision, as the `WIZ_LINKS` guard above. `file.h`'s own comment even records the collision
(*"MAX_STRING_LENGTH also defined in merc.h ... Leaving it here for now"*).

**It was never live.** `mud.c` was the only file to see both definitions, it includes `file.h` on
line 1 and `merc.h` on line 7, and its two `MAX_STRING_LENGTH` / `MSL` uses are at lines 191-192 —
long after `merc.h` has `#undef`'d its way back to 4608. A buffer sized between those two includes
would have been 512 bytes short; none was. With `file.h` archived the guard has been collapsed to a
plain `#define`, as was done for `WIZ_LINKS`.

## Added later: `exception.h` (2026-08-01)

Six lines, zero includers, and half of them commented out — `class Exception`'s only body built a
formatted message and then didn't report it (`// bug(ebug);`). It surfaced only because it was the
**last remaining consumer of the refcounted string type**, whose implementation was deleted the same
day; `MUNCH_VARARG`, the varargs macro it used, went with that implementation.

`rift.h` carried the matching `#define throwbug(...) throw Exception(...)`, which had **no uses** —
and could not have had any, since nothing included the header that declares `Exception`. It was
removed alongside. `ERROR_TRAP()` / `CATCH_ERRORS()` in `rift.h` are unrelated and still live: they
catch `const char *` and `...`, never this class.

## What was *not* archived

- **`code/direction.h`** — `MAX_EXIT`, `MAX_TRACKS`, and the `Directions` enum were rescued out of
  `room.h` before it moved. The enum came from `a0ab2d7` (2021-10-25), which is real modernization
  work; 14 translation units depended on it. The `room.h` here is the original, `CRoom` and all.
- **`code/dbconnection.h`** — still in use see: `DbSession::Connect` takes a `const DbConnection &`,
  and `Config::GetDbConnection` returns one. It is a plain POD config struct with no ties to the previous attempt, and was swept into the list on a name match. It stays in `code/`.
- **`class CMud;`** — `classlist.h` was reachable tree-wide via `rift.h`, but exactly one of its ~34
  declarations was required: `rift.h`'s `extern CMud RS;`. `rift.h` now carries that one line
  directly. (`rift.h` cannot include `mud.h` — `mud.h` includes `rift.h`.)
- **`mud.h`'s commented-out members** — the `GameEngine` / `Interpreter` comments, and the
  `mud.h:78-82` note about *"THE coolest hacks in all recorded history"*, are deliberately kept.
  The former still describes where the engine is meant to end up; the latter is part of the
  project's character.
- **`CQueue`, `CLogger`, `CMud`, `CClass`, `CProficiencies`** — 2016-era in *naming* only. They are
  alive and used. This archive holds what is **dead**, not what is **old**.

## A note on surveying this tree

The original survey undercounted the dead set four separate ways, each a survey that returned
a confident wrong number:

1. `classlist.h` was recorded at 0 includers. It was included by `rift.h` — and therefore most of
   the tree — but `rift.h` contained a raw NUL byte, so `grep` classified it as binary and skipped
   it. The NUL has since been removed, but the count was never re-run.
2. `room.h` was recorded at 11 includers. It had 14. Two used `#include "../room.h"`, which a
   pattern matching `"room.h"` cannot see (the quote is not adjacent to `room`).
3. The 14th was in `tests/`, which no survey searched.
4. `dbconnection.h` was classified dead by name association, without checking its consumers.

When working in this tree, prefer re-running the search over trusting a number written down
earlier — and search `tests/` as well as `code/`.
