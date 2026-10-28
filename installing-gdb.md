# Installing GDB

GDB is installed the way system tools
are: from your operating system's
packages. Unlike most tools in this
series it rarely travels alone — you
want the compiler beside it, because a
debugger without programs built for
debugging is a map reader without maps.
This chapter installs both, checks the
versions, and is honest about how the
guide's own lab did it slightly
differently.

## The package install

On a Debian or Ubuntu machine:

```bash
sudo apt install gdb build-essential
gdb --version
gcc --version
```

`build-essential` brings in GCC and the
standard headers and libraries; `gdb`
brings the debugger, its `gcore`
companion (chapter 10), and the text-mode
interface. On Fedora the pair is
`sudo dnf install gdb gcc`; on Arch,
`sudo pacman -S gdb gcc`. On macOS, GDB
exists but is a guest in someone else's
house — most Mac developers use LLDB
(chapter 13) — and this guide's worked
examples are Linux ones.

Check: the lab's versions, from
transcript `gdb-lab/transcripts/t00-version.txt`:

```text
$ gcc --version
gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
$ gdb --version
GNU gdb (Ubuntu 15.1-1ubuntu1~24.04.1) 15.1
```

Version numbers matter less with GDB
than with faster-moving tools — the core
commands in this guide are decades
stable — but a transcript from someone
else's session should always be read
against their version, not yours.

## How the lab's GDB arrived (an honest note)

The lab's machine is a sandbox that does
not permit system-wide installs, so its
GDB was not installed the way the
paragraph above describes. It came from
the very same Ubuntu packages, downloaded
and *unpacked* into a private folder —
the `gdb` package plus the four libraries
it needed — and is run from there with a
small wrapper that points the loader at
the unpacked libraries. Same GDB 15.1,
same behaviour, no system touched. The
point of telling you is the series rule:
you are reading procedures the lab ran,
and where the lab's route differed from
the one taught, the difference is on the
record. `criteria.md` carries the detail.

## The compiler flags are half the install

A debugger session is only as good as
the debug information in the program,
and that is decided at compile time,
long before GDB is involved. The two
flags this whole guide stands on:

```bash
gcc -g -O0 -o program program.c
```

- **`-g`** embeds the debug information:
  the line map, the variable names, the
  types. Without it, chapter 15 happens.
- **`-O0`** turns optimisation off, so
  the machine code follows the source in
  the order you wrote it and variables
  live where the map says they live.
  Release builds use `-O2`, and chapter
  14 shows what that does to a session:
  breakpoints land on surprising lines
  and variables report themselves
  `<optimized out>`.

Neither flag changes what the program
computes. A practical habit follows:
keep a debug build beside the release
build while you work — same source, two
compiles — and reach for the `-g -O0`
one when trouble starts. When the
trouble only exists in the optimised
build, chapter 14 is the chapter, and it
will not pretend otherwise.

Check: every fixture in the lab
(`gdb-lab/fixtures/`) was compiled with
exactly the line above — except
`shrunk.c`, which was compiled twice,
once each way, on purpose, for chapter
14. The transcripts name their binaries
(`shrunk-o0`, `shrunk-o2`) so the reader
can always tell which map GDB was
reading.
