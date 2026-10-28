# Troubleshooting

Outcome: the six failures every new GDB
user meets, each with its cause and its
check. Four of them the lab met on
purpose and kept the transcripts; two
are the ones the text of earlier
chapters warned about, gathered here.

## "No symbol table is loaded" / "No symbol ... in current context"

**Cause:** the program was compiled
without `-g`, or you are asking about a
variable outside the frame where it
lives. GDB can run the program and catch
its crash but cannot name anything in
it (transcript `t90-no-symbols.txt`):

```text
Breakpoint 1, 0x00005555555551c2 in store_value ()
No symbol table is loaded.  Use the "file" command.
No symbol table info available.
```

**Check:** rebuild with `-g -O0`
(chapter 3) and confirm `info locals`
answers. If the error names one variable
in a program that otherwise names its
own, you are in the wrong frame —
`frame` to the one that owns it
(chapter 8).

## `Function "main" not defined`

**Cause:** the binary's symbol table has
been stripped, so even function names
are gone; GDB offers to make the
breakpoint *pending* (to wait for a
library that might provide it) and, in
batch mode, answers its own question
"no". **Check:** `file program` — a
stripped binary says so. There is no
flag to undo a strip after the fact;
the fix happened at build time, which
is chapter 14's point. Rebuild with
symbols and keep them.

## The breakpoint never fires

**Causes, in the order the lab has met
them:** the breakpoint is on a line the
optimiser moved or merged (chapter 14 —
the lab's `-O2` breakpoint asked for
line 9 and landed on line 10); the
condition is never true — test it by
removing the condition and printing the
variables at the unconditional stop;
the line is in code the program never
executes, which is itself the finding;
or the breakpoint is disabled — `info
breakpoints` shows its state. The
general check is chapter 6's ledger:
look at the list before blaming the
program.

## "Value optimized out"

**Cause:** `-O2` (or higher) — the
variable has no location at that point
in that build. It is a report, not an
error (transcript `t09-optimized.txt`).
**Check:** print the value where it is
used rather than where it was born, or
reproduce the fault in a `-g -O0` build.
If the fault refuses to reproduce
unoptimised, chapter 14's working rules
apply and the sanitizers go in first.

## "Could not attach to process"

**Cause:** the kernel's ptrace scope,
set to 1 on the lab machine and on many
distributions, allows attaching only to
your own descendants — not to a sibling
process you merely own. GDB's message
names the setting itself (transcript
`t10-attach.txt`). **Check:**
`cat /proc/sys/kernel/yama/ptrace_scope`.
On your own machine, the intended routes
are to debug the process from the shell
that launched it, to run it under GDB
from the start, or — as the machine's
administrator, deliberately — to set the
scope to 0. Treat the refusal as a
control working, not a wall to tunnel
under, and see chapter 1's rule for the
reason it exists.

## The core file that is not there

**Cause:** chapter 10's pair — the core
pattern points somewhere you are not
looking (a handler service, not a file),
or the size limit is zero in the context
that ran the program, and cannot be
raised from inside a sandbox that owns
you rather than the reverse. **Check:**
read the pattern, raise the limit in the
launcher's context, and prove the
arrangement with a deliberate crash of a
toy program *before* the real program's
crash is the one you needed. `gcore` is
the detour when the kernel's route is
closed (transcript `t07-core-dumps.txt`).

## The session that behaves strangely

**Cause:** your own `.gdbinit` — a
forgotten `set confirm off`, a `define`
shadowing a builtin, settings from a
different machine's habits. **Check:**
`gdb -nx` starts with no init file at
all; if the strangeness vanishes, it was
yours, and chapter 11's advice applies:
keep the file small enough to reread,
and verify settings with `show` in the
session that depends on them.
