# bef(1)

### NAME

**bef** - interpret a Befunge-93 program

### SYNOPSIS

`bef [-d] [-o] [-u] [-q] [-i] [-=] [-l] [-t]
     [-r INPUTFILE] [-w OUTFILE] [-s STACKFILE] [-y DELAY]
     BEFFILE`

### DESCRIPTION

Load the file _BEFFILE_ as a Befunge-93 program and interpret it,
terminating when the Befunge-93 program terminates.

`bef` takes a few options.

    -d
        Run the program with a visual display of the playfield
        as it runs (debugger).  See IDIOSYNCRACIES below for
        important notes.

    -o
        Old versions of `bef` contained an off-by-one error;
        this flag reproduces that behaviour, should it ever
        be needed to maintain backwards compatibility.

    -u
        Old versions of `bef` had undefined behaviour (inherited
        from C) if an EOF occurred during a `&` instruction;
        this flag reproduces that behaviour, should it ever
        be needed to maintain backwards compatibility.
        (The current behaviour is to push -1 onto the stack.)

    -q
        Suppress all output except for the output that the
        Befunge program itself produces.

    -i
        Suppress the warning that is displayed when `bef`
        attempts to execute an unsupported instruction.

    -=
        Instruct `bef` to interpret Befunge-97-style directives
        embedded in the source code as directives rather than
        as Befunge-93 instructions.

    -l
        Old versions of `bef`, when given an input program
        line longer than 80 characters, would wrap it into the
        next line when loading it; this flag reproduces that
        behaviour, should it ever be needed to maintain
        backwards compatibility.  (The current behaviour is to
        truncate such lines.)

    -t
        Old versions of `bef`, when an `#` instruction was
        present at the very edge of the playfield, would wrap
        inconsistently when executing it; this flag reproduces
        that behaviour, should it ever be needed to maintain
        backwards compatibility.

    -r INPUTFILE
        Have the Befunge program, when it performs input
        (`~` or `&`), take that input from INPUTFILE instead
        of from standard input.

    -w OUTPUTFILE
        Have the Befunge program, when it performs output
        (`,` or `.`), send that output to OUTPUTFILE instead
        of to standard output.

    -s STACKFILE
        As the Befunge program runs, log the contents of the
        stack at each step to STACKFILE.

    -y DELAY
        Specify the delay, in milliseconds, between executing
        each instruction, when `-d` is being used.  Has no
        meaning when `-d` is not being used.

### IDIOSYNCRACIES

If _BEFFILE_ does not end with a file extension, `bef` will helpfully
append `.bf` to it before trying to load it, even if a file-extensionless
file by that name exists.

In most setups, if `-d` is given, but `-w` (resp. `-r`) is not given, then
`,` and `.` (resp. `~` and `&`) will have no effect at all; in particular,
they will not pop anything or push anything onto the stack.  This can
result in surprising behaviour when attempting to debug programs with I/O.
The simplest workaround is to give `-w` and `-r` options whenever the
`-d` option is used.

### AUTHOR

Originally written by Chris Pressey; bugfixes and contributions from
several contributors.  See comments in the source code for more details.

### SEE ALSO

**bef2c**(1), **befprof**(1), **every other Befunge-93 implementation ever**(1)
