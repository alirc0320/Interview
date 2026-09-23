# Bug log — 03_periodic_event_loop

Findings from the first-draft `EventLoop::run()`, before the fixes applied in this session.

## 1. Cancelled events that come due hung `run()` forever (critical)

`solution.cpp`, inside `run()`: when the top-of-heap event was both due
(`event_time_elapsed`) and cancelled (`!is_valid`), neither branch matched
(`if (event_time_elapsed && is_valid)` was false, so it fell into a bare
`else { continue; }`). That branch never popped the queue, so the same
dead entry stayed on top forever — `run()` spun on it infinitely and never
reached any later event.

Fix: added an explicit `else if (!is_valid) { events.pop(); event_valid.erase(event_id); continue; }`
branch so a cancelled entry is discarded and the loop moves on.

## 2. `run()` never returned when the queue was empty (critical)

The outer `while (true)` only did anything inside `if (!events.empty())`;
once the queue drained there was no `else`, so the loop spun forever doing
nothing instead of returning, contradicting the loop's own doc comment
("... until no events remain").

Fix: added `else { return; }` for the empty-queue case.

## 3. `event_valid` grew without bound (moderate)

Entries were inserted in `add_event` but nothing erased them once an event
was done — not when a one-shot callback returned `false`, and not on
`cancel()` (which only flips a bool). Long-running loops would leak map
entries indefinitely.

Fix: erase the entry when a callback returns `false` (event finished), and
erase it in the new cancelled-entry branch from bug #1 above.

## 4. Missing `#include <unordered_map>` in the header (minor / fragile)

`solution.h` used `std::unordered_map` without including it, relying on it
being pulled in transitively via `<queue>`/`<tuple>`/`<functional>` —
compiler/stdlib-dependent, not guaranteed. Fixed by adding the include.

## Still open (not bugs, just noted)

- `run()` busy-spins (100% CPU) while waiting for the next deadline instead
  of `sleep_until`. Fine for a blocking single-threaded toy loop as spec'd,
  but worth mentioning if asked about efficiency in an interview.
- `using namespace std;` in the header pollutes every translation unit
  that includes it — style nit, not correctness.

## Verification

`make test PROB=03_periodic_event_loop` — 16/16 test cases, 22/22 checks
pass, built with `-fsanitize=address,undefined`, no hangs, no sanitizer
reports.

## Interviewer-lens rating

**Before fixes:** ~2.5 / 5 — "no hire" leaning "lean no hire."
The scheduling/rescheduling math (drift-free periodic reschedule from the
*previous* deadline, min-heap ordered by timestamp, unique monotonic IDs)
was correct and well-organized — that part reads as solid CS fundamentals.
But bugs #1 and #2 aren't edge-case nitpicks: cancel() combined with run()
is a named requirement in the prompt, and a candidate's own event loop
hanging forever on the *first* call to `run()` with no events is the kind
of thing that gets caught in the first two minutes of live testing. An
interviewer watching this run would stop you before the demo finished.

**After fixes:** ~4 / 5 — solid "hire" leaning "strong hire," bounded by a
few things I'd expect to get gently pressed on:
- Whether you'd sleep instead of busy-spin in a real system (you should be
  able to talk through `condition_variable::wait_until` or
  `sleep_until` unprompted).
- Whether cancelling mid-callback (see the passing
  `event_can_cancel_itself_while_returning_true` /
  `cancelling_a_periodic_event_stops_it_mid_stream` tests) was something
  you reasoned about deliberately vs. got right by luck — be ready to
  explain *why* it's correct (the map lookup happens after any reschedule
  push, so a cancel always wins before the next execution).
- Whether you'd single-thread-guard this if callers could call
  `add_event`/`cancel` from other threads (out of scope here, but a
  natural follow-up question).

Net: the fixed version is a strong result for a 45-minute onsite problem —
correct, tested, sanitizer-clean. The gap between "before" and "after" is
exactly the gap between "passes the happy path" and "survives an
interviewer actually running your test suite."
