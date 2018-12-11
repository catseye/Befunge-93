Welcome to the Befunge-93 Reference Distribution
================================================

_Version 2.25 - Silver Jubilee Edition_

This is the reference distribution for the Befunge-93 programming language.
It contains the specification and reference implementation (`bef`), which
are direct descendants of the originals, along with a few related tools,
many example programs, and an assortment of historical documents.

About Befunge-93
----------------

Befunge-93 is an esoteric programming language where the program exists in a
two-dimensional grid of cells, where each cell contains a single instruction,
and execution can proceed in any cardinal direction across this grid -- not
just left-to-right, but also right-to-left, top-to-bottom, and bottom-to-top.

For more information, see the Befunge-93 specification in the `doc` directory.

Contents
--------

*   `README.markdown`: these post-modernist existential rants
*   [`LICENSE`](LICENSE):
    a bedtime story written to give your lawyer sweet dreams
*   [`doc/Befunge-93.markdown`](doc/Befunge-93.markdown):
    the specification for Befunge-93, such as it is
*   [`doc/bef.markdown`](doc/bef.markdown):
    docs for the interpreter, in the style of a man page
*   [`src/bef.c`](src/bef.c):
    Befunge-93 reference interpreter/debugger v2.25 source code
*   [`src/bef2c.c`](src/bef2c.c):
    Befunge-93 to ANSI C compiler v1.0 source code
*   [`src/befprof.c`](src/befprof.c):
    Befunge-93 profiler v1.0 source code
*   [`eg/`](eg/):
    Various and sundry contributed Befunge-93 programs
    (see [the README there](eg/README.markdown) for details)
*   [`historic/`](historic/):
    Historical early archive versions of (things called) Befunge
    (see [the README there](historic/README.md) for details)

For More Information
--------------------

See the official entry for [Befunge-93][] at [Cat's Eye Technologies][].

[Befunge-93]: http://catseye.tc/node/Befunge-93
[Cat's Eye Technologies]: http://catseye.tc/

Jubilant Befunging!  
Chris Pressey  
London, England  
December 12, 2018
