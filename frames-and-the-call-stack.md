# Frames and the Call Stack

Outcome: you can read a backtrace as a
story, step *up* the story to the frame
where a decision was made, and read that
frame's arguments instead of the ones at
the bottom. The fixture is `countdown.c`,
which counts down by calling itself:

```c
void countdown(int n)
{
    if (n == 0) {
        printf("liftoff\n");
        return;
    }
    printf("%d...\n", n);
    countdown(n - 1);
}
```

Recursion is the honest way to meet
frames: the same function, alive four
times at once, with four different
arguments. If frames make sense here,
ordinary call chains hold no terror.

## The stack, growing

Break on the function and run:

```text
(gdb) break countdown
Breakpoint 1 at 0x1178: file countdown.c, line 5.
(gdb) run
Breakpoint 1, countdown (n=3) at countdown.c:5
(gdb) continue
3...
Breakpoint 1, countdown (n=2) at countdown.c:5
(gdb) continue
2...
Breakpoint 1, countdown (n=1) at countdown.c:5
```

Three stops, three living calls. The
breakpoint fires on *entry*, so each
stop shows a new frame being born — but
the older calls have not gone anywhere.
They are suspended at their line 10,
the `countdown(n - 1)` call, waiting for
the call below them to return.

## Reading the pile

```text
(gdb) backtrace
#0  countdown (n=1) at countdown.c:5
#1  0x00005555555551b5 in countdown (n=2) at countdown.c:10
#2  0x00005555555551b5 in countdown (n=3) at countdown.c:10
#3  0x00005555555551c9 in main () at countdown.c:15
```

Read it as a sentence, bottom first:
`main` (frame #3), at line 15, called
`countdown` with 3; that call (frame #2),
at line 10, called `countdown` with 2;
that call (frame #1), at line 10, called
`countdown` with 1 — frame #0, the
present, stopped at line 5. Note that
#1 and #2 stand at the *same address*:
they are the same call instruction in
the same function, occupied by two
different moments. An address names
code; a frame names a moment.

## Visiting other frames

`print n` from here answers 1 — the
current frame's `n`. To interrogate a
caller, *select* its frame:

```text
(gdb) frame 2
#2  0x00005555555551b5 in countdown (n=3) at countdown.c:10
10	    countdown(n - 1);
(gdb) info args
n = 3
(gdb) frame 0
#0  countdown (n=1) at countdown.c:5
```

`frame N` moves GDB's attention; `up`
and `down` move it one caller or one
callee at a time; `info args` and
`info locals` always report on the
selected frame. The program itself has
not moved a single instruction during
any of this — only your point of view
has. That separation is the skill: a
backtrace in a crash (chapter 5) is
read the same way, travelling up from
the wreck to the frame whose variable
was already wrong before the call.

Two more commands earn their keep here.
`backtrace full` prints each frame's
local variables along with its call —
long, and the first thing to reach for
when a crash's top frame is a stranger's
code and the story is in the frames
below. And in a deep stack, `frame`
with an argument you computed — the
return address, say — is rarely needed;
the numbers are enough for any stack a
human wrote.

## Letting it land

Two more `continue`s run the session
out: a stop at `countdown (n=0)` — the
frame that prints "liftoff" and starts
the unwinding — and then the returns,
each frame handing its caller back the
floor, until the program exits normally.

Check: transcript
`gdb-lab/transcripts/t06-frames.txt`
holds the whole walk. If your backtrace
shows the same four frames in the same
order with the same arguments, differing
only in addresses, you are reading the
stack the way GDB writes it.
