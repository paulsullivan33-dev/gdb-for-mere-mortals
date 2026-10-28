# Arguments and Batch Mode

Outcome: you can debug a program whose
bug appears only for particular inputs,
and you can run a whole debugging
session as a script — no typing, no
prompt — so it can be repeated exactly
and its output kept as evidence. Both
matter because shy bugs do not perform
on demand, and because "it worked when
I tried it" is not a finding.

The fixture is `convert.c`, which turns
Celsius into Fahrenheit:

```c
int to_fahrenheit(int celsius)
{
    int fahr = (9 / 5) * celsius + 32;  /* bug: 9 / 5 is 1 in integer maths */
    return fahr;
}
```

Run plainly, it lies fluently:

```text
$ ./convert 100
100 C = 132 F
$ ./convert 0
0 C = 32 F
$ ./convert -40
-40 C = -8 F
```

100 °C should be 212 °F, and −40 is the
famous point where the scales agree —
so −40 should print −40. But 0 prints a
correct 32, which is how this bug
survives casual testing: it is right
exactly where a quick check looks.

## Giving the program its arguments

Under GDB, arguments ride along with
`run`:

```text
(gdb) run 100
```

Three spellings cover the cases:
`run 100` (arguments for this run),
`set args -40` (arguments remembered for
every `run` in the session), and on the
command line, `gdb --args ./convert -40`
(arguments known before GDB even starts).
The program cannot tell the difference —
`argc` and `argv` arrive as if from a
shell — which is precisely what makes
the shy bug reproducible in custody.

## Batch mode: the session as a script

**Batch mode** runs GDB with a list of
commands and no human: `-batch` says
"execute and exit", and each `-ex` adds
one command, in order. The lab's whole
investigation of `convert` is one line:

```bash
gdb -q ./convert -batch \
  -ex 'break to_fahrenheit' \
  -ex 'run 100' \
  -ex 'print (9 / 5)' \
  -ex 'print celsius' \
  -ex next \
  -ex 'print fahr'
```

and its output is the finding:

```text
Breakpoint 1, to_fahrenheit (celsius=100) at convert.c:8
8	    int fahr = (9 / 5) * celsius + 32;  /* bug: 9 / 5 is 1 in integer maths */
$1 = 1
$2 = 100
9	    return fahr;
$3 = 132
```

`print (9 / 5)` deserves a pause: GDB
evaluates expressions in the *program's*
language, and in C, `9 / 5` with two
integers is integer division — 1, not
1.8. The formula multiplies the Celsius
value by 1 and adds 32: 132 for 100,
−8 for −40, and a plausible 32 for 0.
The debugger did not just show the
symptom; it evaluated the faulty
expression on its own and produced the
smoking gun, `$1 = 1`.

Batch mode's honest uses:

- **Evidence.** The command above, pasted
  into a bug report, lets anyone rerun
  the session and watch the same `$3`
  appear. This guide's transcripts lean
  on the same idea.
- **Repetition.** A session that took an
  hour to assemble interactively becomes
  a file of `-ex` lines — or a `-x`
  command file, the grown-up form — and
  can be re-run after every fix attempt.
- **Automation.** A test harness can run
  the program under GDB and grep the
  output for the value that must never
  appear. Chapter 11's `.gdbinit` is the
  same impulse, pointed at comfort
  instead of repetition.

One discipline note: batch output is
only as trustworthy as the command list.
It shows what GDB did, in order, with no
selection — keep the whole of it, as
the lab's transcripts do, including the
lines that turned out not to matter.

Check: the plain runs and the batch
session above are transcript
`gdb-lab/transcripts/t05-batch-and-args.txt`,
captured exactly as they were run.
