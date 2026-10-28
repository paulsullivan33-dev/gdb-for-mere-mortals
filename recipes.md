# Recipes

Outcome: four whole investigations,
start to finish, one per kind of trouble
the earlier chapters built tools for.
Each names the symptom as it is usually
reported, the session that answers it,
and the transcript that proves it.

## "The total is wrong, and the code looks right"

Symptom: no crash; an aggregate — a
total, a count, a checksum — is off by
a plausible amount. Reading the code
has not helped because every line looks
correct in isolation.

Session: watch the aggregate, not the
code. Chapter 7's investigation is the
template: break anywhere the variable
is in scope, `watch` it, and count the
firings. The watching turned "short by
2400" into "fired four times for five
items", and the last firing's frame
told the rest (`print i`, `print
items[4]`). Watchpoints accuse by
arithmetic; a count that does not match
the world is a fact no amount of source
reading can argue with.
(Transcript: `t04-watchpoint.txt`.)

## "It only fails with these arguments"

Symptom: fine from the test harness,
wrong from the command line; or fine
for the inputs anyone tried, wrong for
one customer's.

Session: bring the failing input into
custody — `run` with the exact arguments
(chapter 9) — and if the input is one of
a family, script the whole session in
batch mode so every candidate input gets
the identical interrogation. `convert`'s
bug hid at 0 and confessed at 100 and
−40, and the confession was an
*expression*: `print (9 / 5)` evaluated
the suspect arithmetic inside the
program's own language and printed the
lie. Whenever a formula is the suspect,
print its pieces before you print its
result.
(Transcript: `t05-batch-and-args.txt`.)

## "It crashes sometimes, when nobody is watching"

Symptom: the crash is real, the corpses
are not kept, and rerunning under GDB a
hundred times has produced a hundred
clean runs.

Session: arrange the evidence before
the crime. Read the core pattern, raise
the limit in the context that launches
the program — a shell profile for a
person, a service setting for a daemon —
and let the next crash write its own
report (chapter 10). Where the system
will not cooperate, `gcore` from a
scheduled attach, or simply running the
program under GDB in batch mode with a
script that waits for the signal, are
the fallbacks. The session after the
crash is short: backtrace, the top
frames' locals, and the one pointer
that is zero.
(Transcripts: `t07-core-dumps.txt`,
`t02-crash-backtrace.txt`.)

## "The program is already running"

Symptom: the misbehaviour is happening
*now*, in a process that was not started
under a debugger, and restarting it
would end the evidence along with the
incident.

Session: attach — `gdb -p PID` (or
`attach PID` at the prompt) stops the
running process where it stands and
gives you the full toolkit on it;
`detach` lets it go on. It is the right
tool and, in the lab, a forbidden one:
the sandbox sets the kernel's ptrace
scope to 1, which permits attaching only
to your own *descendant* processes, and
GDB refused, naming the setting itself:

```text
Could not attach to process.  If your uid matches the uid of the target
process, check the setting of /proc/sys/kernel/yama/ptrace_scope, or try
again as the root user.
```

(Transcript: `t10-attach.txt`.) On your
own machine, attaching to your own
long-running process commonly works —
you are its owner, and many systems
default to scope 0 or run the process as
a child of your shell — but treat the
refusal as the healthy default it is.
Attaching freezes someone's working
program and exposes its entire memory;
chapter 1's rule decides whose processes
those may be, and a production service
gets a maintenance window, not a
surprise.

## "It was fine before the optimiser"

Symptom: the debug build behaves, the
release build misbehaves, and debugging
the release build produces nonsense —
breakpoints landing mid-line, variables
that will not print.

Session: chapter 14's. Expect the map to
disagree with the territory, believe the
program's *outputs* over its variables,
and where a value matters, print it at
the place it is *used*, not where it was
born. Sometimes the honest answer is
that the bug is a compiler-visible
undefined behaviour the optimiser
lawfully exploited — and the neighbour
who finds those fastest is a sanitizer
(chapter 13).
(Transcript: `t09-optimized.txt`.)
