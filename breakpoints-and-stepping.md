# Breakpoints and Stepping

Outcome: you can stop a program where
you choose — by line, by function, only
when a condition holds — and move it
forward at the grain you need, into a
call or over it. The fixture is
`basket.c`, which totals the prices of
five shop items and gets the total
wrong. This chapter sets the traps;
finding the bug itself is chapter 7's
pleasure.

`basket.c` in the part that matters:

```c
static struct item items[] = {
    { "notebook",  349 },
    { "pencil",     60 },
    { "mug",       899 },
    { "ruler",     125 },
    { "lamp",     2400 },
};

int main(void)
{
    int count = 5;
    int total = 0;
    int i;

    for (i = 0; i < count - 1; i++) {   /* bug: stops one early */
        total += items[i].price;
    }

    printf("items: %d, total: %d cents\n", count, total);
    return 0;
}
```

Run plainly, it prints `total: 1433` —
the true sum is 3833. Somewhere between
the loop and the truth, 2400 cents went
missing. Reading the code may already
have shown you the bug; pretend it has
not, because the next bug will not be so
kind, and the method is the chapter.

## Breakpoints by place, and by promise

A breakpoint takes a *location*: a
function name, or a file and line:

```text
(gdb) break main
Breakpoint 1 at 0x1155: file basket.c, line 18.
(gdb) break basket.c:23 if i == 2
Breakpoint 2 at 0x116c: file basket.c, line 23.
```

The second carries a **condition**: the
program runs straight through line 23
for `i` = 0 and 1 and stops only when
`i` is 2. Conditions are the difference
between a breakpoint and a chore — the
hundredth time through a loop is
reachable without ninety-nine
`continue`s. A condition can be any
expression the program's own variables
support: `break ... if items[i].price > 500`
stops only for the expensive items.

`info breakpoints` keeps the ledger,
conditions included:

```text
(gdb) info breakpoints
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x0000000000001155 in main at basket.c:18
2       breakpoint     keep y   0x000000000000116c in main at basket.c:23
	stop only if i == 2
```

`disable 1`, `enable 1`, `delete 2` work
on the numbers; a disabled breakpoint
stays in the list, out of the way, for
when the investigation wanders back.

## Step over, step into

From the conditional stop, the lab read
the state and took one line:

```text
Breakpoint 2, main () at basket.c:23
23	        total += items[i].price;
(gdb) print i
$1 = 2
(gdb) print total
$2 = 409
(gdb) next
22	    for (i = 0; i < count - 1; i++) {   /* bug: stops one early */
(gdb) print total
$3 = 1308
```

409 was notebook plus pencil; the mug's
899 has now landed, 1308. Two stepping
verbs divide the world:

- **`next`** executes the line and stops
  at the following one, treating any
  function call on the line as part of
  the line — it steps *over* calls.
- **`step`** (chapter 4) stops at the
  first line *inside* a call — it steps
  *into*.

Rule of thumb: `next` along code you
trust, `step` into code you suspect. If
you `step` into a library call by
accident, `finish` (chapter 4) climbs
back out with the return value. A third
verb, `until`, runs to the end of the
current loop body, for when stepping
line by line through an iteration is
more patience than the bug deserves.

## What the session proved, and what it didn't

The conditional breakpoint fired exactly
once, at the third item, and the running
total behaved at every point the lab
looked. Nothing found — yet. Breakpoints
show you the places you already suspect;
this bug lives in a place nobody thought
to stop at, which is why the next
chapter watches the *value* instead of
the places.

Check: transcript
`gdb-lab/transcripts/t03-breakpoints.txt`
holds the session, including the ending
worth knowing — after the stops, one
`continue` lets the program print its
wrong total and exit normally:

```text
(gdb) continue
Continuing.
items: 5, total: 1433 cents
[Inferior 1 (process 89721) exited normally]
```
