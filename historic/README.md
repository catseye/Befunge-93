Historical Befunge-93 Sources
=============================

Found amongst my Amiga disk backup images.

[bef-1.0rc2](bef-1.0rc2)
------------------------

Sources for what was probably not the first release of the Befunge
distribution (its language document, reference implementation, and
the initial example programs), but something very close to it.

Both the C source code for the interpreter, and the language document,
describes this as "version 1.0".  However, this version does not yet
mention or support the backquote (greater-than) command, and
subsequent release history (in the comment at the top of `bef.c`)
suggests that it was present in 1.0 and not added in a later version.

The language document is clearly the document on which the current
`Befunge-93.markdown` is based, but it includes an introductory HISTORY
section that was at some point subsequently removed.

The confusingly-named `src` directory contains example programs, along
with a `sources.doc` which describes and attributes each of them.

However, the last 3 examples listed in `sources.doc`, namely
`anagram.bf`, `numer.bf`, and `toupper.bf`, were not actually present
in the backup image.  But they did appear in later releases, and are
included in the `eg` directory of the reference distribution as of this
writing.

In addition, the `chars.bf` example differs from the one in the
reference distribution as of this writing, suggesting it was edited at
some point, but by who and for what purpose is not clear.

[bef-1.0rc1](bef-1.0rc1)
------------------

Sources, document, and example programs dated September 5, 1993.

The language document is the same as in bef-1.0, and so claims that
this is version 1.0.  The source code for the interpreter, however,
makes no claims as to its version (and in fact is pretty stingy on
comments across the board.)

In fact this C source may well have been the first thrown-together
version of the Befunge interpreter.

There are two example programs here which did not appear in 1.0:
`beep.bf`, which simply outputs a BEL character, and `huh.bf`, which
outputs a random (but not uniformly random) sequence of 6's and 7's.

[prehistoric](prehistoric)
--------------------------

As described in its contained README, this subdirectory contains documents
describing an entirely different language, also called "Befunge", dated
January 1993, illustrating how long the name "Befunge" existed before the
programming language now known as Befunge.
