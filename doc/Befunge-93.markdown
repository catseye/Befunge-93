Befunge-93 Documentation
========================

*A Twisted, Deranged Programming Language in the Tradition of
[brainfuck][] and [FALSE][]*

[brainfuck]: http://esolangs.org/wiki/Brainfuck
[FALSE]:     http://esolangs.org/wiki/FALSE

Chris Pressey, Cat's Eye Technologies  
*Original document September, 1993*  
*Updated December, 1996*  
*Updated Yet Again September, 2004*  
*Converted from HTML to Markdown August 2012*  
*Updated for Silver Jubilee, 2018*

### The Basics of Befunge-93 ###

Most likely the most unique element of Befunge-93 programming is the
Program Counter (PC.) In almost all computer programming languages, the
program counter is continually moving forward through the program,
occasionally jumping to another spot in the code (but continuing
forward thereafter, nonetheless.)

The PC in Befunge-93, however, is subject to different rules. It may go
*forward, backward, left, or right*. A Befunge-93 program is treated as
an 80x25 torus (a page which wraps around on the edges) of ASCII text.
Certain commands change the direction of the progress of the PC. By
default, the PC points to the upper-left corner of the program, and is
oriented to travel left-to-right.

Each command in Befunge-93 is a single character, as is the largest data
unit that can be specified in the program source; Befunge-93 programs
have a maximum size of 80x25 total commands and data bytes. There are no
run-time variables, only a single run-time stack. Befunge-93 programs
allow for self-modification. Due to the 2-dimensional nature of the PC,
they also allow for some extremely quirky code.

### The Stack ###

Something like Forth and PostScript, Befunge-93 supports a LIFO, Reverse
Polish Notation (RPN or *postfix*) stack of signed long integers (that
is, each cell of the stack can hold as much as a C language
`signed long int` on the same platform.) The act of placing a value on
the stack is called *pushing*, and the act of taking a value off the
stack is called *popping*. The digits from `0` to `9` are valid
Befunge-93 commands which push their respective values onto the stack. A
double quote `"`, when encountered, toggles *stringmode*, and while
stringmode is active, all character cells will have their ASCII value
pushed onto the stack until another `"` is located.

There are a few basic calculation commands:

-   `+` addition
-   `-` subtraction
-   `/` integer division
-   `*` multiplication
-   `%` modulo
-   `!` logical negation

These are explained in greater detail in the Commands section.

In order to push a number greater than 9 on the stack, calculations must
be done with numbers less than or equal to 9. In any other language this
would be a pain. In Befunge-93 it is a joy. For example, to push '123'
onto the stack, one might push 9, then 9, then multiply (leaving 81),
then push 7, then 6, then multiply (leaving 81 and 42,) then add
(leaving 123.) In Befunge, this would look something like :

    99*76*+

This is, of course, assuming that the PC starts at or before the first
`9` and is working towards the right. If this snippet represents a
entire Befunge-93 program, this assumption is correct: the PC starts at
the upper-left of the torus and is initially oriented to execute
rightward.

NB. If the stack is empty when you pop something off, be warned that
this will *not* generate an underflow! It will simply push a 0 value
onto the stack. Hope you can live with it!

### The Program Counter in Detail ###

There are 5 commands which unconditionally control the PC direction:
`>`, `<`, `v`, `^`, and `?`. `>` makes the PC travel to the right; `<`
to the left; `v` down; `^` up; and `?` in a *random* direction. So, the
following example is an infinite loop:

    ><

As is:

    >v
    ^<

As is:

    >v>v
     >^
    ^  <

Note that ` ` (space) is a null command which does nothing.

Should the PC encounter the 'edge' of the program, such as if you were
to try to execute:

    <

The PC will 'wrap around' to the other 'edge' of the program. This
example, then, is an infinite loop as well.

### Decision Making ###

The standard 'if' statement in Befunge-93 is either `_` or `|`,
depending on how you want to branch. Both of these instructions pop a
value off the stack and check to see if it is true (non-zero,) and
change the direction of the PC accordingly:

-   `_` acts like `<` if the value is true or `>` if it is false; and
-   `|` acts like `^` if the value is true or `v` if it is false.

'While' loops can be made by sticking an 'if' in an infinite loop. For
example,

    >_@

(This program fragment pops all of the non-zero values off the stack,
and the first zero value, then exits [`@` is the exit command.])

### Input ###

The `&` (ampersand) command will get a numeric value (in decimal) from
the standard input and push it on the stack. `~` (tilde) will get the
next ASCII character from standard input and push it on the stack.

For example,

    &,

...prints out "A" if the user types "65 ", and...

    ~.

...prints out "65 " if the user types "A".

### Output ###

The `.` command will pop a value off the stack and output it as a
decimal integer, followed by a space, somewhat like Forth. `,` will pop
a value, interpret it as the ASCII value of a character, and output that
character (not followed by a space.)

For example,

    665+*1-,

...prints out ASCII 65 ("A".), and...

    665+*1-.

...prints out "65 ".

### Special Commands ###

`#` is the 'bridge' command... it causes the next command which would
normally be executed to be skipped over, and not executed. For example,

    >123...@

would output "3 2 1 ", but

    >123#...@

would output "3 2 " with one of the '.''s being skipped. Judicious use
of `#` can make for very interesting code!

`:` is the duplication command. It makes a copy of the top element of
the stack. This is useful, as demonstrated in the following program:

    v.<
    >:|
      @

This program makes duplicates of each value on the stack, which is
checked, and if non-zero, printed.

`$` pops a value off the stack, but does nothing with it. So,

    123.$.@

results in "3 1 ".

`\` swaps the top two elements of the stack. So,

    123\...@

results in "2 3 1 ".

`` ` `` (back-quote) is the 'greater-than' command. It compares the top
two values on the stack, and returns '1' if the first is greater than
the second. For example,

    65`.

...outputs "1 " and...

    25`.

...outputs "0 ".

### Self-Modification ###

The last two commands that need to be explained are the ones that allow
you to examine and change the contents of the torus where the program is
stored. This 'playfield' can be used for auxiliary storage when the
stack alone will not suffice, but keep in mind that it also contains the
running program.

The `g` command examines the contents of the playfield. It pops a *y*
coordinate off the stack, then an *x* coordinate. It pushes the value
found at (*x*, *y*) onto the stack. If the thing at (*x*, *y*) is a
Befunge-93 instruction, the value pushed will be the ASCII value of that
character. From the point of view of the program text, *x* determines
the column and *y* determines the row; (0, 0) refers to the first
(leftmost) column and the first (topmost) row of the program source.

The `p` command alters the contents of the playfield. It pops a *y*
coordinate off the stack, then an *x* coordinate, and then a value. It
places the value into the torus at (*x*, *y*). If the program, at some
later point, executes the instruction at (*x*, *y*), it will be the
interpreted as the Befunge instruction in the ASCII character set with
the same value as was put there with the `p` instruction.

### Appendix A. Command Summary ###

    COMMAND         INITIAL STACK (bot->top)RESULT (STACK)
    -------         -------------           -----------------
    + (add)         <value1> <value2>       <value1 + value2>
    - (subtract)    <value1> <value2>       <value1 - value2>
    * (multiply)    <value1> <value2>       <value1 * value2>
    / (divide)      <value1> <value2>       <value1 / value2> (nb. integer)
    % (modulo)      <value1> <value2>       <value1 mod value2>
    ! (not)         <value>                 <0 if value non-zero, 1 otherwise>
    ` (greater)     <value1> <value2>       <1 if value1 > value2, 0 otherwise>
    > (right)                               PC -> right
    < (left)                                PC -> left
    ^ (up)                                  PC -> up
    v (down)                                PC -> down
    ? (random)                              PC -> right? left? up? down? ???
    _ (horizontal if) <boolean value>       PC->left if <value>, else PC->right
    | (vertical if)   <boolean value>       PC->up if <value>, else PC->down
    " (stringmode)                          Toggles 'stringmode'
    : (dup)         <value>                 <value> <value>
    \ (swap)        <value1> <value2>       <value2> <value1>
    $ (pop)         <value>                 pops <value> but does nothing
    . (output int)  <value>                 outputs <value> as integer
    , (output char) <value>                 outputs <value> as ASCII
    # (bridge)                              'jumps' PC one farther; skips
                                            over next command
    g (get)         <x> <y>                 <value at (x,y)>
    p (put)         <value> <x> <y>         puts <value> at (x,y)
    & (input int)                           <value user entered>
    ~ (input character)                     <character user entered>
    @ (end)                                 ends program

### The People Who Helped Make the Dream Reality ###

Special thanks to Curtis Coleman, Jason Goga, Kalyna Zazelenchuk, Shawn
Vincent, Mike Veroukis, Urban Müller, and Wouter van Oortmerssen.
