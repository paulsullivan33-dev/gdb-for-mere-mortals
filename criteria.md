# Writing criteria for this guide

Every "for Mere Mortals" guide is written to the same nine-point
standard, so a reader who trusts one can trust the rest. Here it
is, with the verification notes for this particular guide below.

1. **Fix it first.** Reader outcomes lead, not action items: each
   chapter opens by naming the outcome it delivers, shows a check
   the reader can run to confirm they got it, and a reasonable
   reader finishes able to state what was done, what it means,
   and what to try next.
2. **Plain language.** Short sentences, everyday vocabulary,
   technical terms defined on first use. No slang, and no
   idioms that only make sense in one country — the reader may
   be translating as they go.
3. **No dead ends.** A guide never tells the reader to do
   something it cannot show them how to verify. Every claim a
   reader relies on — an install, a command, an output — is
   something this guide demonstrates or clearly labels
   documented-only below.
4. **Live-verified where it matters.** Procedures are run for
   real in the guide's lab wherever the build machine allows it,
   and the transcripts kept in the repository are the evidence.
   What could not be run is listed honestly below, never
   presented as if it had been.
5. **General audiences.** Nothing in this guide — examples,
   fixtures, names, addresses — is modelled on any particular
   reader or author's real life, network, or organisations.
   The lab's traffic is the lab talking to itself.
6. **Standalone.** A reader needs this guide, its lab, and
   nothing else. Where another guide would genuinely help, it
   is pointed to once, by name, and the chapter still completes
   without it.
7. **Honest about limits.** Chapter 14 names what the tool
   cannot do: variables the optimiser removed, binaries
   without symbols, bugs that live in timing and leave when a
   thread is frozen, the past it did not watch, and other
   languages' insides — and what it must not be used for:
   reading the memory of programs that are not yours to
   inspect.
8. **The ethics rule is in the text, early.** Chapter 1 and the
   README state the rule plainly — *your programs, your
   machines, or explicit permission* — before any procedure
   depends on it, and later chapters point back to it where
   the tooling would make crossing the line easy: attaching
   to a running process (chapter 12) and opening a core file
   that holds a program's whole memory (chapter 10).
9. **Builds clean.** The site builds with MkDocs in strict mode
   with no warnings, and a fresh reader following the chapters
   in order reaches the stated outcome of each.

## Verification notes for this guide

The build machine is a 2-CPU, 7.7 GB Ubuntu 24.04 sandbox.
The compiler is the system's GCC **13.3.0**. GDB **15.1** came
from Ubuntu's own packages — the `gdb` package plus the four
libraries it needed (`libbabeltrace1`, `libipt2`,
`libsource-highlight4t64`, `libdebuginfod1t64`) — downloaded
and unpacked, not installed, into a private folder, because
the sandbox does not permit system-wide installs; a small
wrapper points the loader at the unpacked libraries, and the
GDB run that way is the GDB of chapter 3's version transcript.
Every fixture was compiled from the sources kept in
`gdb-lab/fixtures/`; all transcripts live in
`gdb-lab/transcripts/`, and the example init file is
`gdb-lab/gdbinit`.

Run for real, with transcripts as evidence:

- Versions of both halves of the toolchain, GCC 13.3.0 and
  GDB 15.1 (t00).
- A complete first session on `sum.c`: breakpoint at `main`,
  `step` into a call, arguments printed, `finish` with its
  return value, the not-yet-assigned variable caught honestly
  (it printed the garbage in the slot) before one `next`
  completed the line (t01).
- A genuine segfault in `store.c`: SIGSEGV caught in custody,
  two-frame backtrace, the offending pointer printed as
  `(int *) 0x0` (t02).
- Breakpoints by function and by file-and-line, a conditional
  breakpoint (`if i == 2`) firing once, `info breakpoints`
  showing the condition, `next` over the loop's edge (t03).
- A hardware watchpoint on the basket's `total`: five
  firings — the initialisation (old value 32767, the slot's
  prior rubbish) and four additions — against five items in
  the basket; `print i` and `print items[4]` at the last
  firing naming the item never added; and the watchpoint
  tidily deleted before the run to the end (t04).
- Arguments and batch mode on `convert.c`: plain runs at
  three temperatures (right at 0 by luck, wrong at 100 and
  −40), then one scripted session — `break`, `run 100`,
  `print (9 / 5)` returning 1, `print fahr` returning 132 —
  with no interactive typing (t05).
- A recursive call stack in `countdown.c`: the breakpoint
  firing per call, a four-frame backtrace with two frames at
  the same address, `frame 2` selected and its argument read
  back as 3 (t06).
- Core files, both halves: the kernel route refused (core
  pattern read; the sandbox's hard core-size limit of 0
  cannot be raised from inside — the refusal and the
  consequent missing file are in the transcript), and the
  detour run end to end — `gcore` saved a core at a chosen
  stop, and a later session opened it and read `i` = 2 and
  `total` = 409 out of the frozen frame (t07).
- A `.gdbinit` applied and proven: `show pagination` off,
  `show listsize` 20, a user-defined `bmain` command setting
  its breakpoint, a struct pretty-printed across lines, and
  the history file present afterwards with owner-only
  permissions (t08).
- Optimisation, both builds of `shrunk.c` stopped at the
  same requested line: `-O0` prints both variables; `-O2`
  lands on a moved line with `argc`, `argv`, and both
  variables reporting `<optimized out>` (t09).
- The same crash under AddressSanitizer: its report names
  the write, the zero page, and the identical two frames —
  the neighbour comparison of chapter 13 run, not read (t11).
- The missing-symbols failures: a build without `-g` (names
  refused, `info locals` empty of meaning) and a stripped
  build (`break main` refused by name, the crash stopping at
  a bare address) (t90).

Documented only — described from the project's documentation,
not run, and the text does not claim otherwise:

- A *successful* attach to an already-running process. The
  lab tried, against its own sleeper process, and the
  kernel's ptrace scope (1 on this machine) refused; GDB's
  own message naming the setting is transcript t10, and
  chapter 12 teaches the procedure around that verified
  refusal.
- A kernel-written core dump, end to end, for the reason
  above: the raising of the limit is refused in this
  sandbox. The core *examination* half of chapter 10 is
  verified against a `gcore` file, which is the same kind
  of evidence.
- Remote debugging with `gdbserver`; the lab has one
  machine and no separate target.
- The graphical faces — `gdbtui`'s windows and the debugger
  panels of code editors. They drive the same engine; the
  lab used the prompt and batch mode, which leave
  transcripts.
- Multithreaded debugging sessions. Every fixture is
  single-threaded; thread commands are surveyed and the
  observer effect discussed in chapter 14.
- Other languages' runtimes (Python, Go, Rust specifics),
  reverse debugging, and rr; characterised in chapters 13
  and 14, not benchmarked or run.

Fixtures are this guide's own and deliberately generic:
seven small C programs — a sum, a slot store that crashes on
its fourth store, a shopping basket with an off-by-one loop,
a temperature converter with an integer-division bug, a
recursive countdown, one program built two ways to show what
optimisation does to a session, and a patient sleeper for
the attach attempt. Their data is invented: a five-item
shopping list, three temperatures, numbers small enough to
check by hand. No example is modelled on any real person,
program, service or organisation, and the only processes the
lab attached to, or tried to, were its own.
