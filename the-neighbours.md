# The Neighbours

Outcome: you know which tool to reach
for before GDB, after GDB, and instead
of GDB — and what each of them sees that
a debugger cannot. Every tool here is
characterised from its own behaviour or
documentation; one of them, the
sanitizer, the lab ran against the same
crash as chapter 5, so the comparison is
not theoretical.

## Printing and logging

The oldest neighbour is the `printf`:
put the value on the screen and keep
the change. Crude, and still the right
first move when the question is about a
program's *ordinary* behaviour across
many runs — logs survive the run, ship
with the program, and answer questions
nobody thought to debug. A debugger
examines one run in magnificent detail;
a log line examines ten thousand runs
superficially. Whole classes of trouble
live in the difference. The debugger's
counter-virtue is chapter 5's: it needs
no change to the program, and no rerun
in which the bug must agree to appear.

## AddressSanitizer: the compiler as bloodhound

Build the same crashing fixture with a
different flag and run it *without* a
debugger:

```bash
gcc -g -O0 -fsanitize=address -o store-asan store.c
./store-asan
```

**AddressSanitizer** (ASan) is not a
separate program but extra machinery
the compiler builds into yours: it
tracks which memory is legitimate and
reports the first touch that is not.
Against `store.c` it reported, in part:

```text
==90221==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
==90221==The signal is caused by a WRITE memory access.
==90221==Hint: address points to the zero page.
    #0 0x634f0e183325 in store_value .../store.c:23
    #1 0x634f0e183359 in main .../store.c:31
SUMMARY: AddressSanitizer: SEGV .../store.c:23 in store_value
```

(Full text: transcript
`gdb-lab/transcripts/t11-asan.txt`.)
Compare chapter 5: the same line, the
same zero address, reached without a
session at all — the program carried
its own accuser. For invalid reads and
writes, heap overflows, and use after
free, a sanitizer build is often the
*fastest* route, and it works on crashes
nobody can reproduce under a debugger.
Its costs are real: the program runs
larger and slower, must be rebuilt for
the purpose, and it finds only the sins
it instruments. GDB's counter-virtues:
it works on the build you already have,
and it answers questions beyond memory —
values, paths, timing of changes — once
you are stopped at the scene. Serious
work uses both, in whichever order the
symptom suggests. Its siblings —
UndefinedBehaviorSanitizer, ThreadSanitizer
for races — work the same way for their
own families of sin.

## Valgrind: the slow, thorough examiner

**Valgrind** runs your program on a
synthetic processor, checking every
memory touch, without any rebuild:
`valgrind ./store` would report the same
invalid write, and a great deal about
the heap besides — leaks included, which
neither GDB nor ASan volunteers. The
price is speed, often ten to fifty times
slower, and the occasional program that
will not run under it at all. Reach for
it when the sin is in the heap and the
program is small enough to forgive the
pace.

## LLDB and the IDE debuggers

**LLDB** is the LLVM project's debugger:
the same ideas — breakpoints, watchpoints,
frames, expressions — with a different
command dialect (`breakpoint set`,
`frame variable`), and the debugger
built into Apple's tools and most
Rust and Swift setups. Nothing in this
guide's *method* changes on LLDB; only
the spelling does. The debugger panels
inside code editors — VS Code and its
kin, the JetBrains family, and GDB's own
`gdbtui` text windows — are likewise
front-ends: they draw the stack and the
variables GDB or LLDB reports, and add
breakpoints you set by clicking the
margin. The lab used the command line
throughout because it leaves
transcripts; the front-ends are
described here, not demonstrated, and
criteria.md keeps that on the record.

## rr: the time machine

One neighbour deserves naming for the
bug class that defeats everything above:
the intermittent fault, gone whenever
watched. **rr** records a program's
execution — every input from the outside
world — so the run can be replayed
deterministically under GDB, backwards
as well as forwards: `reverse-step` to
the moment *before* the bad value. It
needs particular hardware support and a
compatible kernel, which is why the lab
characterises rather than runs it; when
a heisenbug is yours and rr runs on your
machine, it is the strongest tool in
this chapter.

The pattern across all five neighbours:
each trades something — a rebuild, a
slowdown, a dialect, a platform — for a
view GDB does not have. GDB remains the
instrument for the question the others
cannot phrase: *stop here, at this
moment, and show me everything.*
