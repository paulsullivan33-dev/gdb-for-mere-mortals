# Chasing a Crash

Outcome: a program that dies with a
segmentation fault, run under GDB, gives
up the line it died on, the chain of
calls that took it there, and the value
that killed it — in about four commands.
This is the session most people first
reach for a debugger to have, and the
one worth practising before you need it.

The fixture is `store.c`, a tiny program
that keeps integers in three slots. A
helper, `find_free_slot`, hands out a
pointer to a free slot, or returns
`NULL` when none is left. `store_value`
writes through whatever it is handed:

```c
void store_value(int v)
{
    int *slot = find_free_slot();
    *slot = v;              /* crashes when slot is NULL */
}
```

`main` stores four values into three
slots. Run it plainly and it dies the
usual unhelpful death: the shell reports
a segmentation fault, the exit status is
139 — and that is all. No line, no
reason, not even the value it was
storing. Now the same run under GDB.

## Run it and let it die in custody

```text
$ gdb -q ./store
Reading symbols from ./store...
(gdb) run
Starting program: .../store

Program received signal SIGSEGV, Segmentation fault.
0x00005555555551d9 in store_value (v=40) at store.c:23
23	    *slot = v;              /* crashes when slot is NULL */
```

A **signal** is the operating system's
notification that something exceptional
happened; **SIGSEGV** is the one meaning
"you touched memory that is not yours to
touch". Outside GDB the signal kills the
program. Inside, GDB catches it first
and freezes the program at the faulting
instruction: function `store_value`,
argument 40, line 23 — the very write,
`*slot = v`.

## Ask how it got here

```text
(gdb) backtrace
#0  0x00005555555551d9 in store_value (v=40) at store.c:23
#1  0x000055555555520e in main () at store.c:31
```

The **backtrace** reads bottom-up as
history: `main`, at line 31 — the fourth
`store_value(40)` call — called
`store_value`, which is frame #0, the
present. Two frames is a short story,
but the shape never changes: the crash
is at the top, the reason is usually one
or two frames down the page of your own
source, and the library frames beyond
your code can wait until your code is
cleared.

## Ask what the values were

The line is guilty-looking, but a line
is not a cause. The cause is one
`print` away:

```text
(gdb) print slot
$1 = (int *) 0x0
(gdb) info locals
slot = 0x0
```

`slot` is `0x0` — address zero, the
`NULL` pointer. `find_free_slot` was
asked for a fourth slot, had none, and
said so in the only way its return value
can; `store_value` never checked, and
wrote the value 40 to address zero. The
operating system refuses writes to
address zero, and quite right too.
`info locals` confirms there is nothing
else in the frame to know: the local
story is one pointer, and it is null.

Notice what did not happen. No print
statements were added. No theory was
formed and tested. The program was not
re-run in the hope the crash would
repeat — under GDB the first crash is
the only one you need, because it stops
instead of vanishing.

## The fix, and what GDB is for

The fix belongs to the program, not the
debugger: `store_value` must check what
`find_free_slot` returned, or `main`
must stop asking. GDB's part is done —
it turned "Segmentation fault" into
"line 23, fourth call, `slot` was
`NULL`", which is a fact a programmer
can act on. Keep that shape in mind:
**GDB produces facts about one run.**
Deciding what the facts mean, and what
to change, stays with you.

Check: the session above is transcript
`gdb-lab/transcripts/t02-crash-backtrace.txt`,
complete with the end of the session —
quitting while the inferior is stopped
at the signal makes GDB ask whether you
mean it:

```text
(gdb) quit
A debugging session is active.

	Inferior 1 [process 89646] will be killed.

Quit anyway? (y or n) y
```

Answer `y`. The corpse is not evidence
any more; you have the facts it died
holding.
