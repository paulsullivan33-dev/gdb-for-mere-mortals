# What GDB Is

A program in trouble is a witness that
will not talk. It crashed at 2 a.m. and
left one line in a log. It printed a
total that is wrong by exactly one item,
and every line of its source looks
correct when you read it. The usual
response is to form a theory and test it
by adding print statements, rebuilding,
rerunning, and waiting for the trouble
to happen again. Sometimes the trouble
does not happen again, which is worse.

**GDB is a debugger: a program that runs
another program under its control.** It
starts your program for you and stays
attached while it runs. It can stop the
program at a line you name — a
**breakpoint** — or stop it when a
variable changes — a **watchpoint**. While
the program is stopped, GDB can show you
its variables, the contents of its
memory, and the **call stack**: the chain
of function calls that led to this exact
moment. Then it can move the program
forward one line at a time, or let it
run on. When the program crashes under
GDB, it does not vanish: it freezes at
the fault, and GDB shows you where, and
what the values were.

The name is plain: GDB is the GNU
project's debugger, decades old, free,
and present on nearly every Linux
machine or one package away. It speaks
many languages; this guide works in C,
the one it was born speaking, because
C's small lies — a null pointer, an
index one past the end — are the ones a
debugger exposes most clearly.

## Why anyone would do this

Four honest situations account for
nearly every session in this guide:

- **The crash.** The program dies with a
  segmentation fault and no explanation.
  Under GDB it dies *in custody*: the
  signal, the line, the backtrace, and
  the offending value are all on the
  table (chapter 5).
- **The wrong answer.** No crash, no
  error — just a number that should be
  3833 and is 1433. Reading the source
  has not helped, because the bug is not
  in what the code says but in what it
  *does*. Watching the total change, one
  addition at a time, settles it
  (chapter 7).
- **The shy bug.** It only happens with
  particular arguments, or in production,
  or after the crash is already over.
  Batch mode and core dumps are GDB's
  answers (chapters 9 and 10).
- **Learning.** Stepping through a
  program you wrote, watching frames pile
  up in a recursive call, teaches what
  execution actually is faster than any
  diagram (chapter 8).

## The rule, stated before everything else

A debugger is not a passive observer. It
can read every byte of a program's
memory — including the passwords, keys
and private data the program is holding —
and write to it, too: under GDB you can
change a variable mid-run. A core dump,
chapter 10's frozen remains, keeps a
copy of all of that on disk. So this
guide has one rule, and it is not fine
print:

> **Debug only programs that are yours —
> on machines you own, or where you have
> explicit permission to inspect them.**

Your own code, crashing on your own
machine: yes, that is the whole point.
A colleague's service, because you found
the core file lying on a shared disk:
the core file is a copy of that program's
memory, secrets included, and opening it
is reading them. Attaching to a process
you do not own crosses the same line, and
chapter 12 shows the operating system
itself enforcing a version of it. The
lab in this guide debugged only programs
it wrote, on one machine, and the one
"secret" any fixture handled was a
shopping list.

If a future idea you have for this tool
does not fit inside the rule, the idea is
the problem. Chapter 14 returns to this
with the responsibility spelled out a
little further.

## A first taste

After installing (chapter 3), a session
looks like this. The program is the
lab's `store`, which crashes on purpose;
the commands are in the order a person
types them when a crash is all they know:

```text
(gdb) run
Starting program: .../store

Program received signal SIGSEGV, Segmentation fault.
0x00005555555551d9 in store_value (v=40) at store.c:23
23	    *slot = v;              /* crashes when slot is NULL */
(gdb) print slot
$1 = (int *) 0x0
```

That is the whole pitch in two commands:
the crash names its line and its
function, and `print` shows the pointer
was null — zero, `0x0` — at the moment of
the crash. No theory, no added logging,
no rerun. Chapter 5 takes this session
apart properly; the full record is the
lab's transcript
`gdb-lab/transcripts/t02-crash-backtrace.txt`.

## Who it is for

The programmer whose program crashed and
who wants evidence instead of a theory.
The programmer whose program did *not*
crash and wishes it had, because a wrong
answer with no error is harder to chase
than a corpse. The student who learns
what a call stack is by standing inside
one. The common thread is one person,
their own programs, and a question only
the running program itself can answer.

## What it is not

GDB is not a proofreader: it will not
tell you your program is correct, only
what it did on the runs you watched. It
is not a profiler — it stops time rather
than measuring it, and a bug that lives
in timing may leave when GDB arrives
(chapter 14 is honest about this). It is
not a substitute for tests: it examines
one execution at a time, in glorious
detail, and says nothing about the
executions you did not run. And it is
not invisible or read-only: you are
holding the program by the throat, and
chapter 14 covers what that power means
for programs that are not yours to hold.
