#!/usr/bin/env bash
#
# Shared scaffolding for the smoke tests: locating the binary, building a
# throwaway config on a scratch port, booting, and tearing down.
#
# Sourced by boot_test.sh and new_player_test.sh. Not executable on its own.
#
# Callers set PORT (and may set BOOT_TIMEOUT) before sourcing, then call
# smoke_setup / smoke_boot.

# --- locate the binary ---------------------------------------------------------
# Auto-detect the *most recently built* rift among the common in-source and
# out-of-source locations. CI should pass RIFT_BIN explicitly for a deterministic
# target.
smoke_find_bin() {
	if [[ -n "${RIFT_BIN:-}" ]]; then
		[[ -x "$RIFT_BIN" ]] || fail "RIFT_BIN is not executable: $RIFT_BIN"
		return
	fi

	local newest="" cand
	for cand in "$REPO_ROOT/code/rift" "$REPO_ROOT/build/code/rift"; do
		if [[ -x "$cand" ]] && { [[ -z "$newest" ]] || [[ "$cand" -nt "$newest" ]]; }; then
			newest="$cand"
		fi
	done

	RIFT_BIN="$newest"
	[[ -n "$RIFT_BIN" && -x "$RIFT_BIN" ]] || {
		echo "SMOKE FAIL: rift binary not found; build it or set RIFT_BIN" >&2
		exit 1
	}
}

fail() {
	echo "SMOKE FAIL: $*" >&2
	echo "----- last 40 log lines -----" >&2
	tail -40 "$LOG" >&2 2>/dev/null || true
	exit 1
}

# A caller with its own artifacts to remove sets SMOKE_CLEANUP_HOOK to a function
# name rather than installing its own EXIT trap -- smoke_setup owns that trap and
# would overwrite one registered beforehand.
cleanup() {
	if [[ -n "${SMOKE_CLEANUP_HOOK:-}" ]]; then
		"$SMOKE_CLEANUP_HOOK" || true
	fi
	if [[ -n "${RIFT_PID:-}" ]]; then
		kill "$RIFT_PID" 2>/dev/null || true
		wait "$RIFT_PID" 2>/dev/null || true
	fi
	rm -rf "$WORK"
}

# --- workspace + fixture config ------------------------------------------------
smoke_setup() {
	WORK="$(mktemp -d)"
	LOG="$WORK/boot.log"
	RIFT_PID=""
	trap cleanup EXIT

	smoke_find_bin

	# In-memory rewire of our config file to the smoke tests values.
	local src_config="${RIFT_CONFIG:-$REPO_ROOT/config.json}"
	[[ -f "$src_config" ]] || fail "config not found: $src_config"
	FIXTURE_CONFIG="$WORK/config.json"
	cp "$src_config" "$FIXTURE_CONFIG"
	sed -i -E "s/(\"Port\"[[:space:]]*:[[:space:]]*)[0-9]+/\1$PORT/" "$FIXTURE_CONFIG"
}

# --- boot ----------------------------------------------------------------------
smoke_boot() {
	# The area/player/logs/configs dirs are addressed relative to CWD's parent
	# (rift.h RIFT_ROOT_DIR ".."), which RIFT_CONFIG does *not* override -- so CWD
	# must be an immediate child of the repo root. code/ always is.
	local run_cwd="$REPO_ROOT/code"

	echo "smoke: booting $RIFT_BIN on port $PORT (cwd=$run_cwd, config=$FIXTURE_CONFIG)"
	( cd "$run_cwd" && RIFT_CONFIG="$FIXTURE_CONFIG" exec "$RIFT_BIN" "$PORT" ) >"$LOG" 2>&1 &
	RIFT_PID=$!

	local booted=0 _
	for _ in $(seq 1 $((BOOT_TIMEOUT * 2))); do
		if ! kill -0 "$RIFT_PID" 2>/dev/null; then
			fail "process exited during boot (before binding port $PORT)"
		fi
		if grep -q "binding on port $PORT" "$LOG"; then booted=1; break; fi
		sleep 0.5
	done
	[[ "$booted" -eq 1 ]] || fail "did not reach 'binding on port $PORT' within ${BOOT_TIMEOUT}s"
	echo "smoke: PASS  boot reached port bind"
}

# --- sanitizer / allocator check -----------------------------------------------
# Meaningful under -DRIFT_SANITIZE=ON, but the glibc allocator messages fire in
# an ordinary build too -- they are how the new-player heap overrun announced
# itself before it was fixed.
SMOKE_BAD_LOG_RE="AddressSanitizer|LeakSanitizer|UndefinedBehaviorSanitizer|runtime error:|SUMMARY: .*Sanitizer|double free|corruption|invalid next size|malloc\(\): |free\(\): "

# Non-fatal form, for polling a still-running server. Returns non-zero on a hit.
smoke_assert_clean_log_quiet() {
	! grep -qE "$SMOKE_BAD_LOG_RE" "$LOG"
}

smoke_assert_clean_log() {
	if grep -qE "$SMOKE_BAD_LOG_RE" "$LOG"; then
		fail "sanitizer or allocator reported a memory error"
	fi
	echo "smoke: PASS  no sanitizer or allocator report"
}
