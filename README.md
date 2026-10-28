# GDB for Mere Mortals

A program you wrote does one of two
unhelpful things: it crashes, or it runs
to the end and prints the wrong answer.
In both cases it knows exactly what went
wrong — the values in its variables, the
path it took, the moment a pointer went
bad — and in both cases it tells you
almost none of it. **GDB**, the GNU
debugger, exists to close that gap. You
run your program under GDB's control,
and GDB will stop it where you ask, let
it run until a value changes, show you
the chain of calls that led to a crash,
and read out variables as they stand at
that frozen moment. After a crash it can
do the same to the remains, if the
remains were kept.

This guide explains how that works: how
to take your first session against a
small practice program, how to read a
**backtrace** after a real crash, how
**breakpoints** stop a program where you
choose and **watchpoints** stop it when
a value changes, how to walk the **call
stack** one **frame** at a time, how to
drive a program that only misbehaves
with the right arguments, and how to
examine a **core dump** the next day.
The limits are stated plainly, including
the uncomfortable ones: a debugger can
read everything in a program's memory,
and an optimised program may no longer
contain the variables you wrote.

The rule that governs everything here is
stated in chapter 1 and repeated in
chapter 14: **your programs, your
machines, or explicit permission to
inspect them.** The lab obeyed it
strictly. Every program debugged in this
guide is a small fixture the lab wrote
for the purpose, run on one machine, and
the one process the lab tried to attach
to was its own.

The lab behind this guide ran the real
thing: **GDB 15.1**, taken from Ubuntu's
own packages, against seven small C
programs written as fixtures under the
system's **GCC 13.3**. Against them the
lab chased a genuine segfault to the
exact line and the null pointer that
caused it, set conditional breakpoints,
caught a running total in the act of
changing with a hardware watchpoint —
and so found an off-by-one error by
counting — walked a recursive call stack
frame by frame, proved an integer bug in
batch mode without typing a command
interactively, saved a core file with
`gcore` and read the frozen variables
back out of it a session later, and
compiled one program with optimisation
to watch its variables honestly report
themselves *optimised out*.

What the lab could not do, it did not
fake. Its sandbox refused to raise the
core-dump limit, so kernel-made core
dumps are taught around the verified
`gcore` route and the refusal is shown.
The same sandbox forbids attaching to a
running process, so the attach chapter
shows GDB's own refusal and explains the
setting behind it. Graphical front-ends,
remote debugging, and other languages
are taught from the documentation and
labelled as such wherever they appear.
`criteria.md` keeps the full ledger of
what was run and what was only read.

## What this guide covers

- [**What GDB is**](what-gdb-is.md) —
  the mental model: a program stopped in
  time, evidence instead of theories,
  and the permission rule that is part
  of the tool
- [**The moving parts**](the-moving-parts.md) —
  debug information, breakpoints and
  watchpoints, frames, and which part
  knows what
- [**Installing GDB**](installing-gdb.md) —
  the package install, the version
  check, and why your compiler flags
  matter as much as the debugger
- [**Your first session**](your-first-session.md) —
  compile with `-g`, break, run, step,
  print, finish: the whole loop in one
  sitting
- [**Chasing a crash**](chasing-a-crash.md) —
  a real segfault: the signal, the
  backtrace, and the null pointer in
  the frame
- [**Breakpoints and stepping**](breakpoints-and-stepping.md) —
  stopping by line and by function,
  conditions, and the difference
  between `step` and `next`
- [**Watching variables**](watching-variables.md) —
  watchpoints: stop when a value
  changes, and find a bug by counting
  how often one does
- [**Frames and the call stack**](frames-and-the-call-stack.md) —
  moving through the stack, reading
  each frame's arguments, recursion
  laid out flat
- [**Arguments and batch mode**](arguments-and-batch-mode.md) —
  programs that only misbehave with the
  right input, and scripted sessions
  that leave a transcript
- [**Core dumps, after the fact**](core-dumps.md) —
  examining a crash that already
  happened, the limits your system sets
  on core files, and the `gcore` detour
- [**Making GDB yours**](making-gdb-yours.md) —
  the `.gdbinit` file: settings that
  stick, pretty printing, and commands
  of your own
- [**Recipes**](recipes.md) — the total
  is wrong, it only fails with these
  arguments, it was fine before the
  optimiser, the program is already
  running
- [**The neighbours**](the-neighbours.md) —
  printing and logging, AddressSanitizer,
  Valgrind, LLDB, the IDE debuggers:
  what each sees that GDB doesn't, and
  vice versa
- [**Limits**](limits.md) — optimised-out
  variables, stripped binaries, a
  debugger changes the timing it
  measures, and the memory it can read
  is a responsibility
- [**Troubleshooting**](troubleshooting.md) —
  no symbols, breakpoints that never
  fire, the attach refusal, and the
  traps this guide's lab fell into so
  you don't have to
- [**Glossary**](glossary.md) — the
  terms, collected in one place

## What it doesn't cover

Debugging programs you have no right to
inspect — a debugger reads a process's
whole memory, secrets included, and
chapter 14 draws that line properly.
Kernel debugging, debugging other
people's production services, and
reverse engineering stripped binaries
for sport are likewise out. Graphical
front-ends and IDE integration are
pointed at, not taught; the command line
shown here is the engine every one of
them drives. Other languages get an
honest survey in chapter 14, but the
worked examples are C, the language GDB
was born speaking.

## How to use this guide

Read chapter 1 for the model and the
rules, and chapter 2 before installing
anything — knowing which part holds what
turns later confusion into a named
problem. Chapter 4 takes your first
session in the same sitting as the
install, against a program small enough
to check by eye. Do not skip chapter 5:
the crash is the moment most people
first reach for a debugger, and the
backtrace is the skill the rest of the
guide builds on. Chapter 7's watchpoints
are the superpower; chapter 10 is the
one to read before your first real crash
in production-shaped circumstances,
because core files are arranged before
the crash, not after. The lab's fixture
programs live in `gdb-lab/fixtures/`,
its transcripts in `gdb-lab/transcripts/`,
its `.gdbinit` in `gdb-lab/gdbinit`, and
its notes are `examples/lab-notes.txt`.

---

## Part of the series

This guide is one of the [For Mere Mortals](https://paulsullivan33-dev.github.io/mere-mortals/) guides: plain-language guides to the tools programmers and makers use.

Related guides:

- [C for Mere Mortals](https://paulsullivan33-dev.github.io/c-for-mere-mortals/)
- [Python for Mere Mortals](https://paulsullivan33-dev.github.io/python-for-mere-mortals/)
- [Linux for Mere Mortals](https://paulsullivan33-dev.github.io/linux-for-mere-mortals/)

## License

The writing in this guide is licensed under [CC BY 4.0](https://github.com/paulsullivan33-dev/gdb-for-mere-mortals/blob/main/LICENSE). Code examples and lab scripts are licensed under the [MIT License](https://github.com/paulsullivan33-dev/gdb-for-mere-mortals/blob/main/LICENSE-CODE).
