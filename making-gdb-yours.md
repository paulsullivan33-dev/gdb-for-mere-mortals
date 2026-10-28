# Making GDB Yours

Outcome: GDB starts the way you like it,
every time, because a file in your home
directory — `.gdbinit` — runs your
standing commands before the first
prompt; and you can teach GDB new
commands of your own with `define`. The
lab's `.gdbinit` is small and every line
of it is proven in one session.

GDB reads `.gdbinit` from your home
directory at startup, before it reads
the program. (It says nothing about
having done so, which is why this
chapter proves each setting from inside
the session that used it.) The lab kept
its example in a separate home —
`gdb-lab/gdbinit` in this repository is
the file — so the other chapters'
sessions ran on plain defaults.

## The file

```ini
# The lab's example .gdbinit
set pagination off
set confirm off
set print pretty on
set listsize 20
set history save on
set history filename ~/.gdb_history

define bmain
    break main
end
document bmain
Set a breakpoint at the start of main.
end
```

Line by line:

- **`set pagination off`** — GDB normally
  pauses long output with a
  "---Type <return> to continue---"
  prompt. Fine interactively, maddening
  in a long backtrace; off is the common
  preference.
- **`set confirm off`** — stops GDB
  asking "Quit anyway?" (chapter 5 met
  the question). The lab's other
  transcripts show the prompt; this
  chapter's does not, because the file
  removed it — a setting proven by an
  absence.
- **`set print pretty on`** — structures
  print one field per line, indented.
  For anything bigger than a two-field
  struct this is the difference between
  reading and squinting.
- **`set listsize 20`** — `list` shows 20
  lines of source instead of 10.
- **`set history save on`** and friends —
  commands are kept in `~/.gdb_history`
  between sessions, so last Tuesday's
  clever breakpoint condition is a
  search away, not a reinvention.

## Proof, from inside the session

Each claim above gets its check in
transcript `gdb-lab/transcripts/t08-gdbinit.txt`:

```text
(gdb) show pagination
State of pagination is off.
(gdb) show listsize
Number of source lines gdb will list by default is 20.
```

`show` is the general verifier — every
`set` has one — and it is the habit to
keep: a `.gdbinit` you have not verified
with `show` is a rumour. The pretty
printing shows itself the first time a
struct is printed in the session:

```text
(gdb) print items[0]
$1 = {
  name = 0x555555556004 "notebook",
  price = 349
}
```

Compare chapter 7's one-line rendering
of the same kind of value
(`{name = ..., price = 2400}`) — same
data, two typographies, chosen by that
one line in the file. And when the
session ended, the history file existed
where the file said it would:

```text
$ ls -la ~/.gdb_history
-rw------- 1 root root 69 Oct 28 12:02 .gdb_history
```

Note the permissions while you are
there: a command history can contain
the values you printed — other people's
data, on the wrong day — and GDB writes
the file readable only by you. Keep it
that way.

## Commands of your own: `define`

The file's last block teaches GDB a
command. `bmain` is deliberately tiny —
a breakpoint at `main`, the first thing
half of all sessions do:

```text
(gdb) bmain
Breakpoint 1 at 0x1155: file basket.c, line 18.
```

Between `define NAME` and `end`, any
GDB commands may live, with `$arg0`,
`$arg1`… standing for arguments the user
supplies, and ordinary `if` and `while`
available. The `document` block that
follows gives the command a help text,
read back with `help bmain`. Real
`define`s grow teeth — a `plist` that
walks a linked list printing each node
is the classic — but the mechanism is
the one shown: name, body, end, proof.

Two cautions, both cheap. A `.gdbinit`
is code that runs at every startup, so
keep it small enough to reread; and if
a session ever behaves strangely on a
borrowed machine, `gdb -nx` starts
without reading any init file — the
clean room for checking whether your
own settings are the suspect.
