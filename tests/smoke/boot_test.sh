#!/usr/bin/env bash
#
# This script creates a codified smoke test. It is a "boot-and-greet" test
# that ensures the MUD boots, loads areas, and serves a greeting screen.
# It is designed to catch issues that may arise from changes in the codebase
# that could cause the MUD to fail to load properly, even if all unit tests
# pass. The script requires a running, provisioned MariaDB and can be configured
# via environment variables.
#
# It boots `rift` on a scratch port against a *provisioned* database (the
# dev-install.sh / db/install.sh environment) and asserts, in order:
#
#   1. the process boots and reaches the port bind (stays up past table init);
#   2. areas actually load
#   3. a TCP client receives the greeting screen;
#   4. no sanitizer report was emitted (meaningful under -DRIFT_SANITIZE=ON).
#
# Just these checks alone will catch a majority of the game bugs. For what
# happens after the greeting -- creating a character and living in the world --
# see new_player_test.sh.
#
# Requires a running, provisioned MariaDB. CI wiring (a MariaDB service +
# db/install.sh, under ASAN).
#
# Config (all overridable via env):
#   RIFT_BIN               path to the rift binary (auto-detected otherwise)
#   RIFT_CONFIG            source config.json to base the fixture on
#   RIFT_SMOKE_PORT        scratch port to bind (default 9993)
#   RIFT_SMOKE_BOOT_TIMEOUT  seconds to wait for the bind line (default 60)
#   RIFT_SMOKE_MIN_AREAS   minimum areas that must load (default 50)
#
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"

PORT="${RIFT_SMOKE_PORT:-9993}"
BOOT_TIMEOUT="${RIFT_SMOKE_BOOT_TIMEOUT:-60}"
MIN_AREAS="${RIFT_SMOKE_MIN_AREAS:-50}"

# Binary lookup, fixture config, boot, teardown and the sanitizer check are
# shared with new_player_test.sh.
# shellcheck source=lib.sh
source "$SCRIPT_DIR/lib.sh"

# --- 1. boot ------------------------------------------------------------------
smoke_setup
smoke_boot

# --- 2. areas actually loaded -------------------------------------------------
areas="$(grep -oE "Loaded [0-9]+ areas\." "$LOG" | grep -oE "[0-9]+" | head -1 || true)"
[[ -n "$areas" ]] || fail "no 'Loaded N areas.' line in boot log"
[[ "$areas" -ge "$MIN_AREAS" ]] || fail "only $areas areas loaded (expected >= $MIN_AREAS)"
echo "smoke: PASS  $areas areas loaded"

# --- 3. greeting served -------------------------------------------------------
GREETING="$WORK/greeting.txt"
if ! exec 3<>"/dev/tcp/127.0.0.1/$PORT"; then
	fail "could not open TCP connection to port $PORT"
fi
# The MUD holds the socket open at the name prompt, so it never sends EOF; read
# for a few seconds then move on.
timeout 3 cat <&3 >"$GREETING" || true
exec 3<&- 3>&- || true
bytes="$(wc -c <"$GREETING")"
[[ "$bytes" -gt 100 ]] || fail "greeting too short ($bytes bytes) -- server did not serve a greeting"
echo "smoke: PASS  greeting served ($bytes bytes)"

# --- 4. clean teardown, no sanitizer report -----------------------------------
kill "$RIFT_PID" 2>/dev/null || true
wait "$RIFT_PID" 2>/dev/null || true
RIFT_PID=""
smoke_assert_clean_log

echo "SMOKE PASS: booted, $areas areas loaded, greeting served, clean log"
