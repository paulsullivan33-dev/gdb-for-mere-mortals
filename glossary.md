# Glossary

**AddressSanitizer (ASan)** — compiler
machinery built into a program with
`-fsanitize=address` that tracks
legitimate memory and reports the first
illegitimate touch (chapter 13).

**Attach** — connecting GDB to a process
already running, rather than starting
one (chapters 12 and 15).

**Backtrace** — the printed call stack:
frame #0 at the stop, its callers above
it, read as "how did I get here"
(chapters 5 and 8). Short form: `bt`.

**Batch mode** — GDB run with `-batch`
and `-ex` commands, executing a scripted
session and exiting (chapter 9).

**Breakpoint** — an order to stop the
program when execution reaches a place:
a line, a function, an address — a
condition may be attached (chapter 6).

**Condition** — an expression guarding a
breakpoint or watchpoint: stop only if
it is true (chapter 6).

**Core dump (core file)** — a copy of a
dead process's memory, written by the
kernel or by `gcore`, examinable after
the fact (chapter 10).

**Debug information** — the map from
machine code back to source lines,
variable names and types, embedded by
compiling with `-g` (chapters 2 and 3).

**`finish`** — run the current function
to its return and report the value it
carried back (chapter 4).

**Frame** — one function call's slice of
the call stack: its arguments, locals
and return point. Frames are moments,
not code (chapter 8).

**`gcore`** — GDB's command to write a
core file from a program stopped in a
session (chapter 10).

**Hardware watchpoint** — a watchpoint
kept by the processor's own debugging
registers, so the program runs at speed
between changes (chapter 7).

**Inferior** — GDB's name for the
program it is running: the one
underneath (chapter 2).

**`next`** — execute the current line,
stepping *over* any function calls it
makes (chapters 4 and 6).

**Optimised out** — GDB's report that a
variable has no location at this point
in an optimised build: `<optimized out>`
(chapter 14).

**Pending breakpoint** — a breakpoint on
a name not yet known, held until a
library provides it (chapter 15).

**Ptrace scope** — the kernel setting
(`/proc/sys/kernel/yama/ptrace_scope`)
governing who may attach to which
processes (chapters 12 and 15).

**Signal** — the operating system's
notification of an exceptional event;
SIGSEGV is the invalid-memory one
(chapter 5).

**`step`** — execute the current line,
stepping *into* any function call it
makes (chapter 4).

**Stripped** — of a binary: with its
symbol table (and any debug information)
removed, leaving addresses without names
(chapters 14 and 15).

**Watchpoint** — an order to stop the
program when a value *changes*, wherever
the change happens (chapter 7).
`rwatch` stops on reads, `awatch` on
either.
