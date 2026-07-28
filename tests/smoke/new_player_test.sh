#!/usr/bin/env bash
#
# Smoke test: can a brand new character actually be created and stay in the world?
#
# boot_test.sh stops at the greeting screen, which is why a heap overrun in
# bust_a_prompt -- triggered by the first prompt a new character is ever sent --
# reached a release. This test goes the rest of the way: it drives nanny's whole
# creation sequence over TCP, lands the character in the newbie academy, and
# holds the connection open while the world ticks around it.
#
# Asserts, in order:
#
#   1. the process boots and reaches the port bind;
#   2. a character can be created and reaches the game;
#   3. it survives several world ticks in the room, and the server still answers
#      commands afterwards;
#   4. the server survives the character logging out again;
#   5. no sanitizer report and no glibc allocator complaint in the log.
#
# Requires a running, provisioned MariaDB, same as boot_test.sh.
#
# Config (all overridable via env):
#   RIFT_BIN                 path to the rift binary (auto-detected otherwise)
#   RIFT_CONFIG              source config.json to base the fixture on
#   RIFT_SMOKE_PORT          scratch port to bind (default 9994)
#   RIFT_SMOKE_BOOT_TIMEOUT  seconds to wait for the bind line (default 60)
#   RIFT_SMOKE_SETTLE        seconds to watch the server after logout (default 10)
#   RIFT_IDLE_SECS           seconds to stay in game (default 20)
#   plus the RIFT_RACE / RIFT_CLASS / RIFT_HOMETOWN knobs mkchar.py reads
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

PORT="${RIFT_SMOKE_PORT:-9994}"
BOOT_TIMEOUT="${RIFT_SMOKE_BOOT_TIMEOUT:-60}"
SETTLE="${RIFT_SMOKE_SETTLE:-10}"

# shellcheck source=lib.sh
source "$SCRIPT_DIR/lib.sh"

# A random alphabetic name: nanny requires 2-12 letters, no digits, and rejects
# anything matching a mob's name. Deleting the pfile afterwards is enough --
# Player.lst is rebuilt from the directory on every boot.
# Built in-shell rather than piped through head, which would SIGPIPE its producer
# and trip pipefail.
CHAR_NAME="Zz"
alphabet="abcdefghijklmnopqrstuvwxyz"
for _ in $(seq 1 8); do
	CHAR_NAME+="${alphabet:RANDOM % 26:1}"
done
PFILE="$REPO_ROOT/player/$CHAR_NAME.plr"

remove_pfile() {
	rm -f "$PFILE"
}
SMOKE_CLEANUP_HOOK=remove_pfile

smoke_setup
smoke_boot

# --- 2/3. create a character and keep it in the world --------------------------
# Bind the client to 127.0.0.5, which has no reverse DNS mapping, so the run also
# covers init_descriptor's gethostbyaddr-failed path.
echo "smoke: creating $CHAR_NAME (${RIFT_RACE:-planar}/${RIFT_CLASS:-healer}/${RIFT_HOMETOWN:-melcene})"
if ! RIFT_SRC="${RIFT_SRC:-127.0.0.5}" python3 "$SCRIPT_DIR/mkchar.py" "$PORT" "$CHAR_NAME"; then
	fail "character creation failed"
fi
echo "smoke: PASS  new character created, survived in game, still responsive"

# --- 4. survive the logout teardown --------------------------------------------
# mkchar.py has closed its socket by now. Do not kill the server yet: the heap
# overrun this test exists to catch stayed silent the whole time the character
# was in the room, and only announced itself once extract_char freed the buffers
# it had scribbled past. Whether that lands as a crash during play or as an
# allocator abort at logout depends on the heap layout, so watch both, and give
# the server a settle window instead of killing it straight away.
echo "smoke: watching ${SETTLE}s of logout teardown"
for _ in $(seq 1 "$SETTLE"); do
	kill -0 "$RIFT_PID" 2>/dev/null || fail "server died after the character logged out"
	smoke_assert_clean_log_quiet || fail "corruption reported after the character logged out"
	sleep 1
done
echo "smoke: PASS  server survived logout"

# --- 5. clean teardown, no sanitizer or allocator report -----------------------
kill "$RIFT_PID" 2>/dev/null || true
wait "$RIFT_PID" 2>/dev/null || true
RIFT_PID=""
smoke_assert_clean_log

echo "SMOKE PASS: created $CHAR_NAME, survived in game, clean log"
