# Your First Session

Outcome: by the end of this chapter you
have run a program under GDB, stopped it
inside a function, read its arguments,
watched a value come back from a call,
and let it finish — the complete loop
every later chapter is a variation on.
The program is the lab's smallest
fixture, `sum.c`, which adds two numbers
in a helper function and prints the
result:

```c
#include <stdio.h>

int add(int a, int b)
{
    int result = a + b;
    return result;
}

int main(void)
{
    int total = add(2, 3);
    printf("total = %d\n", total);
    return 0;
}
```

Nothing in it is broken. That is the
point: learn the controls on a program
whose behaviour you can predict exactly,
so every surprise in the session is
GDB's behaviour, not the program's.

## Compile, load, stop, run

```bash
gcc -g -O0 -o sum sum.c
gdb -q ./sum
```

`-q` quiets GDB's opening banner. GDB
reads the debug information and waits:

```text
Reading symbols from ./sum...
(gdb)
```

At the prompt, set a breakpoint at
`main` and run:

```text
(gdb) break main
Breakpoint 1 at 0x1173: file sum.c, line 11.
(gdb) run
Starting program: .../sum

Breakpoint 1, main () at sum.c:11
11	    int total = add(2, 3);
```

The program is frozen on line 11, before
the line has executed. Everything about
it is available and nothing about it has
happened yet. (Your addresses will
differ from the lab's — modern systems
deliberately shuffle them; the file and
line are the parts to compare.)

## Step in, look around, step on

`step` executes one line — and when the
line calls a function, it steps *into*
the call:

```text
(gdb) step
add (a=2, b=3) at sum.c:5
5	    int result = a + b;
(gdb) print a
$1 = 2
(gdb) print b
$2 = 3
```

You are now standing inside `add`, on
its first line, and its arguments are
readable by name. The `$1`, `$2` are
GDB's own numbering of the values it has
shown you — they can be used later in
the session (`print $1 + $2` works),
though plain names serve most purposes.

One more `step` executes the addition;
`result` is now real:

```text
(gdb) step
6	    return result;
(gdb) print result
$3 = 5
```

## Finish, and the small trap after it

`finish` runs the current function to
its return and reports what it carried
back:

```text
(gdb) finish
Run till exit from #0  add (a=2, b=3) at sum.c:6
0x0000555555555182 in main () at sum.c:11
11	    int total = add(2, 3);
Value returned is $4 = 5
```

You are back in `main`, on line 11
again — and here is the trap the lab
hit, kept because you will hit it too:

```text
(gdb) print total
$5 = 32767
```

That is not a bug in the program or the
debugger: it is the rubbish that was
sitting in `total`'s memory slot,
because the assignment on line 11 has
not completed yet. `finish` stopped the
instant `add` returned, with the value
still in transit. One `next` — execute
this line, stepping *over* any calls
rather than into them — completes the
line:

```text
(gdb) next
12	    printf("total = %d\n", total);
(gdb) print total
$6 = 5
(gdb) continue
Continuing.
total = 5
[Inferior 1 (process 89706) exited normally]
(gdb) quit
```

`continue` lets the program run until
the next stop or, as here, the end. The
program's own output (`total = 5`) and
GDB's report of its exit close the
session.

Check: the session above is the lab's
transcript `gdb-lab/transcripts/t01-first-session.txt`,
recorded exactly as typed. If your run
matches it line for line except for
addresses and the process number, you
have the whole loop: **break, run, step,
print, finish, next, continue.** Every
chapter after this one adds one idea to
that loop.
