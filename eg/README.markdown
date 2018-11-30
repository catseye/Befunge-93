Befunge-93 Example Programs
===========================

Entries which have a **CC0** in the **License** column are in the public
domain, under the Creative Commons CC0 Public Domain Dedication. All
others are freely redistributable in this archive, but remain
copyrighted by their original authors. See the file LICENSE for more
information.

**NOTE**: in the interests of improving the general experience of exploring
these examples, some buggy and/or bitrotting examples have been removed
in the 2.25 release.  Refer to the 2.24 release if you wish to find them.

**NOTE**: many of these programs were developed on interpreters that did not
produce warnings.  When running them with `bef` it is recommended to
suppress extraneous output with `bef -q`.

**NOTE**: dates are given in Month/Day/Year Minus 1900 format, and many of
these dates are questionable, especially when Day is 1.

| Name                       | License | Author              | Date     | Notes                                                                                                                                                                                                                                                                                                                                                                                                 | 
| -------------------------- | ------- | ------------------- | -------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [anagram.bf](anagram.bf)   |         | Kalyna Zazelenchuk  | 9/5/93   | This program will input a string of sixteen characters or less, and (p)ut those characters in order on line 3 (the line with the \*'s). It will then randomly find numbers from 1 to 16, (g)et the characters at line 3 in random order, replace them with \*'s, and print them out. Effectively produces anagrams of words you type in. It uses the code from 'rand2.bf' to generate random numbers.
| [aturley.bf](aturley.bf)   | CC0     | Andrew Turley       | 10/30/97 | aturley's .sig; cellular automata gasket generator.
| [beer.bf](beer.bf)         |         | Kevin Vigor         | 4/9/97   | A simple program that prints the "beer song" (99 bottles of beer on the wall...) from 99 down to 0. Does not get the lyrics exactly right, but I think we can assume that's because it's had a few. Part of the Beer Song archive.
| [beer10.bf](beer10.bf)     |         | Chris Lahey         | 6/8/97   | A squashed (22x10) beer singing program, gramatically correct.
| [beer2.bf](beer2.bf)       |         | Keith Arner         | 4/10/97  | Another "99 Bottles of Beer" program.
| [beer3.bf](beer3.bf)       |         | Chris Lahey         | 4/11/97  | Another singing algorithm.
| [beer4.bf](beer4.bf)       |         | Denis Moskowitz     | 4/11/97  | Yet another singing algorithm.
| [beer5.bf](beer5.bf)       |         | David Johnston      | 4/11/97  | Another singing program.
| [beer6.bf](beer6.bf)       |         | Chris Lahey         | 4/11/97  | A crammed (14x21) singing program.
| [beer7.bf](beer7.bf)       | CC0     | Wim Rijnders        | 5/2/97   | A beer bottling program.
| [beer8.bf](beer8.bf)       | CC0     | Wim Rijnders        | 5/12/97  | Again with the bottles.
| [beer9.bf](beer9.bf)       |         | David Johnston      | 5/13/97  | Crammed (22x9) singing beer program which doesn't use good grammar or punctuation.
| [befbef2.bf](befbef2.bf)   | CC0     | Wim Rijnders        | 5/12/97  | A small subset Befunge-93 interpreter written in Befunge-93. Barely smart enough to execute the helloworld program within the \#-delimited block. Runs like a cow. This is the second revision (ca. 2000?) which has some bugs (pointed out by Amir Karger) fixed.
| [befunge1.bf](befunge1.bf) |         | Denis Moskowitz     | 7/17/97  | Befunge logo source (meaning, the program is roughly in the shape of the word "BEFUNGE".)
| [befunge2.bf](befunge2.bf) |         | Sam Holden          | 7/17/97  | Befunge logo source.
| [befunge3.bf](befunge3.bf) | CC0     | Ben Olmstead        | 7/18/97  | Befunge logo source.
| [befunge4.bf](befunge4.bf) |         | Kevin Vigor         | 8/13/97  | Befunge logo source which prints out some Befunge code (try running it.)
| [befungex.bf](befungex.bf) |         | Kevin Vigor         | 7/18/97  | Non-standard (oversize) Befunge logo source which also prints out some Befunge code.
| [calc.bf](calc.bf)         |         | Bryan L             | 4/10/97  | This is a befunged calculator. Meaning ".", the print command, will also pop the result off the stack. Fortunately, the duplication command ":" is supported. This calculator will blithely ignore anything that isn't a number or a supported command. The list of commands is found on line 3.
| [cascade.bf](cascade.bf)   | CC0     | Chris Pressey       | 7/20/97  | Like copyme.bf, but continues to replicate itself.
| [chars.bf](chars.bf)       |         | Kalyna Zazelenchuk  | 9/5/93   | Generates a printable ASCII table, with characters and corresponding codes in decimal, from 34 to 127.
| [chars2.bf](chars2.bf)     |         | ???                 | ?/?/9?   | Modified version of chars.bf that was distributed as chars.bf for the longest time. Outputs all chars on 1 line. Program is 3 rows tall instead of 2. Unclear who made these modifications, or why.
| [chars3.bf](chars3.bf)     |         | Chris Pressey       | 5/29/118 | Modified version of chars2.bf that does not use stringmode and thus can be compiled by bef2c.
| [copyme.bf](copyme.bf)     | CC0     | Wim Rijnders        | 6/5/97   | Program which reproduces itself in Befunge-space.
| [dladv.bf](dladv.bf)       | CC0     | Dmitry M Litvinov   | 12/1/96  | A minimalist adventure game. n - go north. s - go south. e - go east. w - go west. d - dress. l - labour.
| [drx.bf](drx.bf)           | CC0     | Chris Pressey       | 9/5/93   | "Like Eliza, except better :-)"
| [ea.bf](ea.bf)             | CC0     | Chris Pressey       | 9/5/93   | Makes 'enigmatic aphorisms.' A loose interpretation of a program in '1001 things to do with your Commodore 64.' This is an example of how to simulate a 'gosub' with a value.
| [easm2.bf](easm2.bf)       | CC0     | Chris Pressey       | ?/?/??   | Shortly after `ea.bf` was written, a version using self-modifying code for the branches (`easm.bf`) was written. Unfortunately it didn't work. This version of it works, but unfortunately doesn't quite do the same thing as `ea.bf`.
| [eterlan.bf](eterlan.bf)   | CC0     | Chris Pressey       | 11/29/118| Generates a 50,000-word avant garde novel called ETERLAN SEPTEBMER. This novel generator was written for NaNoGenMo 2018 to mark the 25th anniversary of Befunge (1993-2018). It may contain a quote from Mark Topham. The novel consists of many short paragraphs consisting of long words, most of which start with X or Y. You can think of it as a kind of fireworks.
| [euclid.bf](euclid.bf)     |         | Greg Wright         | 6/5/97   | Euclidean algorithm.  Expects two integers as input, and outputs the greatest common divisor of those numbers.
| [fact.bf](fact.bf)         | CC0     | Chris Pressey       | 9/5/93   | Asks for a number, and supplies the factorial of that number.
| [fact2.bf](fact2.bf)       |         | Jason Reed          | 5/18/97  | Factorial generator, (3x12)
| [fact3.bf](fact3.bf)       |         | Chris Lahey         | 6/8/97   | (23x1) factorial generator.
| [hello.bf](hello.bf)       | CC0     | Chris Pressey       | 9/5/93   | The ubiquitous "Hello, World!" program.
| [hex.bf](hex.bf)           | CC0     | Chris Pressey       | 9/5/93   | Translates ASCII input into hex output.
| [hwii.bf](hwii.bf)         | CC0     | Chris Pressey       | 9/5/93   | Prints out "Hello, World!' forwards, then backwards, then forwards, etc. Demonstrates how one can so easily change the direction of the PC to support their own wicked desires.
| [jsvine.bf](jsvine.bf)     |         | Jeremy S. Vine      | 5/29/113 | A Befunge program posted to the #esoteric IRC channel on freenode.net by Jeremy S. Vine, at the conclusion of an interview.
| [kmquine.bf](kmquine.bf)   |         | Keymaker            | 9/3/108  | The first of a series of really fine quines (self-printing programs) by Keymaker.
| [kmquine2.bf](kmquine2.bf) |         | Keymaker            | 9/3/108  | The second of a series of really fine quines by Keymaker.
| [kmquine3.bf](kmquine3.bf) |         | Keymaker            | 9/3/108  | The third of a series of really fine quines by Keymaker.
| [life.bf](life.bf)         | CC0     | Dmitry M Litvinov   | 6/1/97   | An implementation of John Conway's game of Life.
| [madd.bf](madd.bf)         |         | Greg Wright         | 6/5/97   | 3x3-Matrix Addition.  Expects two 3x3 matrices as input (essentially 18 integers), and outputs the sum of those two matrices.
| [mandel.bf](mandel.bf)     |         | Chris Lahey         | 4/11/97  | A Mandelbrot fractal generator.
| [maze.bf](maze.bf)         | CC0     | Chris Pressey       | 9/5/93   | A conversion of a Commodore-64 graphical pastime: a dead-simple random maze generator.
| [namegame.bf](namegame.bf) | CC0     | Dmitry M Litvinov   | 2/1/96   | "First, example easy writed stupid prog." Befunge bypasses cultural and lingual backgrounds. Try this "easily written" program and see what it does.
| [numer.bf](numer.bf)       | CC0     | Chris Pressey       | 9/5/93   | Produces single-digit numerological equivalents of words you type in. Words should consist of lowercase letters and be terminated by a linefeed character. (Users should be warned that the significance of the output of this program is of extremely questionable practical value. But it is consistant.) (This algorithm could be known as taking the "digital root of the word".)
| [pairing.bf](pairing.bf)   |         | Aaron Dale          | 3/20/97  | A program to implement the "pairing function" (\<x1,x2\> = (2\^(x1) \* (2\*(x2) + 1)) - 1), which maps any two natural numbers onto the set of natural numbers, without repetition.
| [pangram.bf](pangram.bf)   | CC0     | Chris Pressey       | 8/25/112 | Checks if the first line of input is a pangram -- a sentence which contains all the letters of the alphabet, such as "The quick brown fox jumps over the lazy dog" or "Pack my lunch box with five dozen liquor jugs." Case-insensitive, and ignores most punctuation, but "high" punctuation (above ASCII character 90) will likely confuse it.
| [pascserp.bf](pascserp.bf) | CC0     | Chris Pressey       | 7/12/97  | Generates a gasket (Sierpinsky triangle) using Pascal's Triangle.
| [pi.bf](pi.bf)             | CC0     | Ben Olmstead        | 6/25/97  | Produces the first hundred digits of pi in under one hundred characters of Befunge, but cheats in that it simply decompresses the digits encoded in the source code.
| [prime.bf](prime.bf)       |         | Kalyna Zazelenchuk  | 9/5/93   | Lists the integers between 2 and 127 (inclusive), and reports whether each one is prime. Also reports that -128 is not prime.
| [rand.bf](rand.bf)         | CC0     | Chris Pressey       | 9/5/93   | Generates random numbers between 1 and 9 as statistically evenly distributed as the computer's random number generator.
| [rand10.bf](rand10.bf)     |         | Ken Bateman         | 4/12/97  | A 3x10 random number generator.
| [rand11.bf](rand11.bf)     |         | Ken Bateman         | 4/12/97  | A 5x7 flexible random number generator. The number of bits is set with the 88+ at the top left. As shown this program will generate a random number from 0 to 32767.
| [rand12.bf](rand12.bf)     | CC0     | Ben Olmstead        | 4/24/97  | A 2x13 random number generator that generates numbers from -128 to 127. (On an interpreter with unsigned char cells, such as the one on which this example was originally developed, it will produce values from 0..255.)
| [rand13.bf](rand13.bf)     | CC0     | Ben Olmstead        | 4/24/97  | A 2x12, infinitely looping random number generator, which was originally intended to generate numbers in the range 0..31, but in actuality, it generates random powers of 2. Fixing it to generate 0..31 is left as an exercise for the reader.
| [rand14.bf](rand14.bf)     |         | Greg Wright         | 5/18/97  | 4x7 random number generator.
| [rand15.bf](rand15.bf)     |         | Artyom Baranov      | 4/5/96   | THE smallest Befunge RNG-16. (16x1)
| [rand2.bf](rand2.bf)       |         | Kalyna Zazelenchuk  | 9/5/93   | This program, crammed into a 14x14 block, makes random numbers from 1 to 16 using multiple "?" statements.
| [rand3.bf](rand3.bf)       | CC0     | Chris Pressey       | 9/5/93   | 12x9 random number generator, like rand.bf and rand2.bf.
| [rand4.bf](rand4.bf)       |         | Matthew D Moss      | 2/1/96   | A 7x7 random number generator in the tradition of rand.bf .. rand3.bf, but using mathematics to reduce the problem before writing the code
| [rand5.bf](rand5.bf)       | CC0     | Dmitry M Litvinov   | 2/1/96   | A 4x10 random number generator which produces the same output as rand.bf .. rand4.bf. Unlike most Befunge sources this one has a symmetrical beauty to it.
| [rand6.bf](rand6.bf)       | CC0     | Chris Pressey       | 3/10/96  | An 8x4 random number generator a la rand5.bf.
| [rand7.bf](rand7.bf)       |         | Artyom Baranov      | 2/28/96  | A 13x3 random number generator a la rand5.bf and rand6.bf.
| [rand8.bf](rand8.bf)       |         | Chris Howe          | 4/9/97   | A 4x9 flexible random number generator; if you change the 4 in position (1,2) to another integer n it will generate a number between 1 and 2\^n (inclusive) for 0\<n\<9.
| [rand9.bf](rand9.bf)       |         | Chris Lahey         | 4/11/97  | A 3x7 random number generator.
| [robot.bf](robot.bf)       | CC0     | Chris Pressey       | 9/5/93   | You control an 'O' going through a maze of '\*''s. You can type in 'n', 's', 'e', or 'w', and the 'O' travels in that map direction. This not-particularly-challenging game ends when you hit a '\*'.
| [rot13.bf](rot13.bf)       |         | ???                 | 9/11/97  | Performs the rot13 algorithm.
| [rpn.bf](rpn.bf)           | CC0     | Dmitry M Litvinov   | 2/22/96  | A command-line calculator with postfix notation.  Enter an expression like `10 3 + 567 89 * -` and press Enter to evaluate it (to obtain, in this case, `-50450`).
| [rpn2.bf](rpn2.bf)         |         | Kimberley Burchett  | 11/16/96 | A version of `rpn.bf` crammed into a 23x12 block.
| [selflis2.bf](selflis2.bf) | CC0     | Chris Pressey       | 12/1/96  | A self-reproducing 80x1 program. This program can also be used to test for the @ vs StringMode bug.
| [selflis3.bf](selflis3.bf) |         | Kevin Vigor         | 5/8/97   | A 14-byte (almost-)self-reproducing program (the result contains an extra space, but is itself a 15-byte self-reproducing program.)
| [selflis5.bf](selflis5.bf) |         | David Johnston      | 5/7/97   | Crammed (14x6) self-listing program.
| [selflis6.bf](selflis6.bf) |         | Denis Moskowitz     | 5/8/97   | 14-byte quine. This was submitted as a tiny fix to `selflis3.bf` which makes it reproduce itself exactly.
| [selflist.bf](selflist.bf) | CC0     | Dmitry M Litvinov   | 2/21/96  | A self-reproducing Befunge program, 13x4.
| [serp2.bf](serp2.bf)       |         | Kevin Vigor         | 4/9/97   | Generates and prints a Sierpinsky triangle, a simple type of fractal. Since the resolution is 20x30, the detail of the fractal isn't really visible, but hey, what can you do? This is the second revision, released 4/10/97, which is Improved.
| [sinus.bf](sinus.bf)       | CC0     | Dmitry M Litvinov   | 12/1/96  | Program to generate sine wave patterns. Based on cos(a+b)=cos(a)\*cos(b)-sin(a)\*sin(b) and sin(a+b)=sin(a)\*cos(b)+cos(a)\*sin(b). sin and cos values are kept in one stack cell = abs(sin) \* 2\^16 + abs(cos).
| [sort.bf](sort.bf)         |         | Kalyna Zazelenchuk  | 9/5/93   | Same as anagram.bf, except sorts the letters of your word in ascending order.
| [surprise.bf](surprise.bf) |         | Timothy Howe        | 11/26/96 | A big surprise.
| [switchbx.bf](switchbx.bf) |         | Zach Baker          | 7/10/97  | A real purty 'switch' statement. NOTE: This is not a runnable problem in itself. It's just a (two-dimensional) snippet of code.
| [testbrdg.bf](testbrdg.bf) | CC0     | Chris Pressey       | 10/27/111| Tests whether the \# instruction interacts properly with wrapping.
| [testmodu.bf](testmodu.bf) | CC0     | Chris Pressey       | 8/23/111 | Tests how your implementation of Befunge-93 calculates modulus (there is no right way.)
| [testpfcl.bf](testpfcl.bf) | CC0     | Chris Pressey       | 10/5/118 | Prints out the largest and smallest values that can be stored in a playfield cell in your implementation of Befunge-93. A very large number of C compilers interpret `char` to mean a signed 8-bit value, so a very common result of running this is "127 -128".
| [toupper.bf](toupper.bf)   | CC0     | Chris Pressey       | 9/5/93   | Converts letters to upper-case. An example of the \` (greater) statement.
| [wumpus.bf](wumpus.bf)     | CC0     | Wim Rijnders        | 8/15/97  | The classic game of Hunt the Wumpus!
