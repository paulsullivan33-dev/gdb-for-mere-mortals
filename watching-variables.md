# Watching Variables

Outcome: you can order GDB to stop the
program the moment a value changes —
wherever in the code the change happens —
and use that to find a bug whose location
you do not know. In this chapter the
method catches the basket's missing
2400 cents without anyone reading the
loop condition carefully, which is
usually how it goes in real life too.

Chapter 6 ended with a suspicion and no
address: the total is short by one item,
but every stop the lab chose showed the
arithmetic behaving. The question
changes shape: not *"stop at this line"*
but *"stop whenever `total` moves, and
show me the move."* That is a
**watchpoint**.

## Setting a watchpoint

Stop the program anywhere `total` is in
scope — the `main` breakpoint from
chapter 6 will do — and watch it:

```text
(gdb) break main
Breakpoint 1 at 0x1155: file basket.c, line 18.
(gdb) run
Breakpoint 1, main () at basket.c:18
18	    int count = 5;
(gdb) watch total
Hardware watchpoint 2: total
```

"Hardware watchpoint" is the good news:
the processor itself is watching that
memory, so the program runs at nearly
full speed between changes. (Where
hardware cannot, GDB falls back to
checking after every instruction — a
*software* watchpoint — which is honest
work at a crawl. For a variable written
rarely, either serves; chapter 14 notes
the difference.) The watchpoint is
numbered like a breakpoint and shares
its ledger: `info breakpoints` lists it,
`delete 2` removes it, as the end of
this session will need.

## Counting the changes

Now `continue`, and read what the
watchpoint reports. It stops five times,
and each stop is a small deposition:

```text
Hardware watchpoint 2: total

Old value = 32767
New value = 0
```

The first firing is the declaration
itself, `int total = 0` — and look at
the old value: 32767, whatever happened
to be sitting in that memory before.
Variables are born holding rubbish until
they are initialised, and the watchpoint
shows you the rubbish without blushing.

```text
Old value = 0      New value = 349
Old value = 349    New value = 409
Old value = 409    New value = 1308
Old value = 1308   New value = 1433
```

Four additions: 349, 60, 899, 125. For a
basket of **five** items. Nobody had to
suspect the loop; arithmetic did the
accusing. At the last firing the lab
asked the two obvious questions:

```text
(gdb) print i
$1 = 3
(gdb) print items[4]
$2 = {name = 0x55555555601e "lamp", price = 2400}
```

The loop's last act was item 3, the
ruler. Item 4 — the lamp, 2400 cents,
the exact amount the total is short —
was never reached. The loop condition
`i < count - 1` stops one early, and now
it is not a guess read off the source
but a fact observed in the run: the
watchpoint fired four times for five
items. One-character fix, found by
counting.

## Housekeeping: delete it before the end

The lab's first attempt at this session
ended untidily, and the untidiness is
instructive, so it is described rather
than hidden. `total` lives on `main`'s
stack frame; when `main` returns, the
memory stops meaning anything, and GDB —
which takes its promises seriously —
stopped the program on its way out to
announce the watchpoint's death. The
tidy habit: when the watching is done,
remove the watchpoint, then run on.

```text
(gdb) delete 2
(gdb) continue
Continuing.
items: 5, total: 1433 cents
[Inferior 1 (process 89842) exited normally]
```

## The rest of the family

- **`rwatch`** stops when the value is
  *read*, **`awatch`** on read or write —
  for "who is even looking at this
  variable?" mysteries. Hardware support
  varies; GDB will tell you if it cannot.
- **`display`** is the softer cousin:
  `display total` prints the value after
  every stop, without stopping for it —
  a dashboard where watchpoints are
  alarms.
- A watchpoint can carry a condition
  like a breakpoint's: `watch total if i == 3`
  stops only for the change you meant.

Check: the full session is transcript
`gdb-lab/transcripts/t04-watchpoint.txt` —
five firings, the count that does not
match the basket, and the `delete`
before the clean exit.
