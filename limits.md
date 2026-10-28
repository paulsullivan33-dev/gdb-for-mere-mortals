# Limits

Outcome: you know where GDB's evidence
runs out — in optimised code, in
stripped binaries, in timing, and in
the responsibility that comes with
reading a program's whole memory — so
you can tell a debugger-shaped problem
from one that wants a different tool.

## The optimised-out variable

Chapter 3's warning, kept honest: under
`-O2`, the compiler rearranges the
program freely, and the debug map
describes a program that partly no
longer exists. The lab compiled
`shrunk.c` twice and stopped both builds
at the same line. The `-O0` build
answers like a book:

```text
(gdb) print seed
$1 = 21
(gdb) print answer
$2 = 43
```

The `-O2` build had already moved the
stop to a different line — and answers:

```text
Breakpoint 1, main (argc=<optimized out>, argv=<optimized out>) at shrunk.c:10
(gdb) print seed
$1 = <optimized out>
(gdb) print answer
$2 = <optimized out>
```

`<optimized out>` is not GDB failing;
it is GDB reporting, accurately, that
at this point in this build the variable
has no location — its value was folded
into a register that has been reused,
or never materialised at all. The same
session's programme had printed both
printf lines *before* the stop: the
optimised program's order of events is
the compiler's, not the source's.
Working rules follow. Believe outputs
over variables. Print values where they
are used, not where they were born. And
when the bug itself only exists at
`-O2`, suspect the compiler-visible sins
— undefined behaviour the optimiser may
lawfully exploit — and send the
sanitizers in first (chapter 13).
(Transcript: `t09-optimized.txt`.)

## The stripped binary

Without `-g` there is no map: GDB still
runs the program, still catches the
crash, but `print` finds no names and
`info locals` reports "No symbol table
info available." Strip the symbol table
as well and even function names go — a
breakpoint on `main` is refused, and
the crash stops at a bare address in
`?? ()`. Debugging in that country is
possible (addresses, registers,
disassembly) but it is reverse
engineering, a different trade with its
own tools, and out of this guide's
scope. The practical limit is simpler:
**the time to arrange debug information
is build time.** Keep the `-g` build, or
its symbols, for every release you ship
to yourself. (Transcript:
`t90-no-symbols.txt`.)

## The observer effect

GDB stops time; some bugs live in time.
A race between threads may vanish when
one thread is frozen at a breakpoint
while its rival runs on — and GDB's
default is to stop *all* threads at a
stop, which changes the race again.
Multithreaded programs can be debugged —
GDB knows threads, switches between
them, and chapter 2's model holds — but
a fault that exists only at full speed,
in the gaps between events, is often
better served by logs, by ThreadSanitizer,
or by rr's recording than by any number
of breakpoints. The lab's fixtures are
single-threaded, and multi-thread
sessions are characterised here rather
than demonstrated; criteria.md says so.

## What GDB cannot see at all

- **Other machines.** GDB debugs the
  program in front of it. Remote
  debugging exists — `gdbserver` on the
  target, GDB at home — and is a real
  workflow for embedded boards and
  containers; the lab did not run it,
  and it is taught from documentation
  only, where it appears at all.
- **The past.** Apart from a core file
  someone arranged (chapter 10) and rr's
  recordings (chapter 13), execution
  unobserved is execution unrecoverable.
- **Other languages' insides.** GDB
  speaks many languages, but a Python or
  Go program's *own* abstractions — its
  frames and objects as that language
  sees them — usually want that
  language's own debugger, or
  GDB extensions written for the
  runtime. The C method transfers; the
  worked examples are C's.

## The limit that is a responsibility

The deepest limit is not technical.
Debugging gives you the whole of a
program's memory: chapter 1's rule —
**your programs, your machines, or
explicit permission** — is the boundary
of legitimate use, and core dumps extend
the same data beyond the session,
readable by whoever holds the file. The
operating system enforces a floor under
the rule (chapter 12's refusal to let
one user attach to another's process is
the same instinct), but the rule binds
first where it is easiest to break: on
your own machine, with your own root
prompt, looking at a colleague's core
file "just to help". Help by asking
them to run the session, or to share the
backtrace. The facts travel fine without
the memory attached.
