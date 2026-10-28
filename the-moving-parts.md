# The Moving Parts

A debugging session has five things in
it, and most confusion in later chapters
is one of them being mistaken for
another. This chapter names them before
they start moving.

## The program under control

GDB calls the program it runs the
**inferior** — an old word, meaning
simply the one underneath. The inferior
is an ordinary program, built by an
ordinary compiler. GDB starts it, stops
it, and restarts it; `run` always starts
it fresh from the beginning. Nothing
about the inferior is special except one
thing, which is the next part.

## Debug information

A compiled program, on its own, is
addresses and machine instructions: the
names of your variables and the lines of
your source are gone, spent in the
compiling. **Debug information** is the
map back. When you compile with the
`-g` flag —

```bash
gcc -g -O0 -o basket basket.c
```

— the compiler embeds that map in the
program file: which address is which
line, which variable lives where, what
type everything is. GDB reads the map on
startup ("Reading symbols from
./basket...") and spends the whole
session translating between the machine's
world and yours. Chapter 15 shows what
happens without it, and it is bleak:
GDB can run the program and watch it
die, but it cannot name anything.

Two consequences are worth fixing now.
First, `-g` changes the program file,
not the program's behaviour; a build
with `-g` runs exactly as fast and as
wrong as one without. Second, the map
describes the program the compiler
*meant* to write — under optimisation
the compiler rearranges freely, and the
map sometimes points at a variable that
no longer exists. That is chapter 14's
`<optimized out>`, and it is the map
being honest about the territory.

## Breakpoints and watchpoints

A **breakpoint** stops the inferior when
execution reaches a place: a line
(`break basket.c:23`), a function
(`break main`), an address if it comes
to that. A breakpoint may carry a
**condition** — stop only if this is
true — and a count of times to ignore
it before stopping. Breakpoints are
numbered, listed with `info breakpoints`,
and can be disabled, enabled, and
deleted; chapter 6 works them properly.

A **watchpoint** stops the inferior when
a value *changes*, wherever the change
happens: `watch total` needs no line
number, because it is watching a piece
of memory, not a place in the code.
Where the machine allows, GDB uses the
processor's own debugging hardware for
this — the lab's sessions say "Hardware
watchpoint" — and chapter 7 shows one
catching a bug that no breakpoint was
pointed at.

Both are set by you, in advance, on
suspicion. The third stopper needs no
suspicion at all: a crash stops the
inferior by itself, which is chapter 5.

## The stack and its frames

When a function calls a function, the
program must remember where to return
to, and what the local variables were.
That memory is the **call stack**, and
each call's slice of it is a **frame**.
The **backtrace** (`bt`) prints the
stack: frame #0 is where the program is
stopped now, #1 is its caller, #2 the
caller's caller, up to `main` and the
library code that called it.

Frames are how GDB answers *"how did I
get here?"* — and, by letting you select
a frame and print *its* variables, *"what
did the world look like from up there?"*
Chapter 8 walks a recursive stack where
the same function appears four times
with four different arguments, which is
the moment frames stop being a diagram.

## The faces

Everything in this guide is done at
GDB's own prompt, the `(gdb)` prompt of
the **interactive** interface: you type
a command, GDB answers, the inferior
runs and stops. The same engine has
other faces. **Batch mode** (`-batch`
with `-ex` commands) runs a scripted
session and exits — chapter 9 uses it
to leave transcripts. **Command files**
(`-x`) and the `.gdbinit` file in your
home directory (chapter 11) feed GDB
commands from disk. And there are
graphical faces — a text-mode window
layout built into GDB, and the debugger
panels inside code editors, all driving
this same engine. The lab drove the
prompt and the batch mode; the graphical
faces are described, not demonstrated,
and criteria.md says so.

Check: you can see three of the parts in
one line of the lab's first transcript,
`gdb-lab/transcripts/t01-first-session.txt`:

```text
Breakpoint 1, main () at sum.c:11
```

A breakpoint (set by the lab) has fired;
the inferior is stopped in frame #0, in
`main`; and the source file and line are
known because `sum` was compiled with
`-g`. Every session in this guide is
those three facts, arranged differently.
