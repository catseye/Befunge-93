/* ******************************************************************

   bef.c - The Original Befunge-93 Interpreter/Debugger in ANSI C
   v2.23-and-then-some-TODO-describe-the-changes-then-bump-this

   Copyright (c)1993-2015, Chris Pressey, Cat's Eye Technologies.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     Redistributions of source code must retain the above copyright
     notices, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
     notices, this list of conditions, and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

     Neither the names of the copyright holders nor the names of their
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission. 

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
   COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

   ******************************************************************

   Usage :

   bef [-d] [-o] [-q] [-i] [-=] [-l] [-t]
       [-r input-file] [-w output-file]
       [-s stack-file] [-y delay] <befunge-source>

      -d: visual ANSI debugging display
      -o: do not fix off-by-one error (for old sources)
      -q: produce no output except Befunge program output ('quiet')
      -i: ignore unsupported instructions
      -=: use b97-ish = directives
      -l: wrap instead of truncating long program lines (compat w/2.21)
      -t: make # at edge of playfield wrap inconsistently (compat w/2.21)
      -r: redirect input from a specified file instead of stdin
      -w: redirect output to a specified file instead of stdout
      -s: write contents of stack to log file
      -y: specify debugging delay in milliseconds

   Compiles Under:

      Borland C++ v3.1   (16-bit MS-DOS)
      DJGPP v2.952       (32-bit Protected-Mode MS-DOS)

   ******************************************************************

   v2.23: Aug 2012, Chris Pressey
          delay given with -y now actually happens when compiled
            with compilers other than Borland C++; it's implemented
            with nanosleep() in C99, and sleep() (with one-second
            resolution) in ANSI C

   v2.22: Mar 2011, Chris Pressey
          don't insert bogus EOF/directive characters into
            playfield when loading otherwise blank source file
            (thanks to whoever sent me this patch 6 years ago)
          truncate long lines instead of wrapping them, so that
            programs that use the full 80 characters on adjacent
            lines (like mycology) can load correctly; -l disables
          make # at edge of playfield wrap and consistently skip
            a space, regardless of which edge it's on; -t disables
          unknown command-line options are reported as such
          use corrected verbiage in license (I am not a Regent)

   v2.21: Sep 2004, Chris Pressey
          display correct version number
          cleanup only, no functional changes

   v2.20, Jul 2000, Chris Pressey
          prettied up preprocessor directives a bit
          added defines for Metroworks CodeWarrior
            so that bef will build on MacOS
          relicensed under BSD

   v2.12, Mar 1998, Chris Pressey / compiles under Borland C++ v3.1
          added -i and -= options.  You must specify -= if
            you want to use b97-esque directives or the Un*x-ish
            # comment thing, or both.
          automatically appends '.bf' to filenames containing no periods.
          explicitly pops remaining stack elements at end of execution.
          compatibility messages are displayed when quiet is not specified.
          debug mode is much improved, especially I/O under BorlandC.
          bounds checking on 'p' and 'g' instructions.

   v2.11, Jan 1998, Chris Pressey / compiles under Borland C++ v3.1
          divide by zero now produces the correct result
          improved some minor aesthetic features (messages & debug)
          ANSI is used only if not compiling under Borland C.

   v2.10: Jul 1997, Chris Pressey
          added -q command line option.
          added primitive understanding of = directive from b97 spec.
          any file with a =l directive but without =l b93 is rejected.
          also understands # as a pre-directive line prefix for
            Un*x-ish systems.

   v2.02: Jun 1997, Chris Pressey
          pads playfield with space characters.  all unread
            locations remain spaces.

   v2.01: Jun 1997, Chris Pressey
          command line switches are not case-insensitive.
          fixes gcc Segmentation Fault error.

   v2.00: Jun 1997, Chris Pressey
          combines interpreter and debugger.
          fixes ANSI error in debugger.
   v1.02: Feb 1996, Chris Pressey
          @ now pushes '@' onto the stack in stringmode instead of quitting.

   v1.01: Feb 1996, Chris Pressey
          fixes off-by-one error.

   v1.00: Sept 1993, Chris Pressey
          original Befunge-93 distribution.

   ****************************************************************** */

/********************************************************* #INCLUDE'S */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#if defined(__TURBOC__) || defined(__BORLANDC__) || defined(__DJGPP__)
  #define MSDOS 1
#endif
#ifdef MSDOS
#  include <dos.h>
#  include <conio.h>
#  define CONSOLE 1
#  define CURSORSHAPE 1
#endif /* MSDOS */
#ifdef __GNUC__
#  include <unistd.h>
#endif /* __GNUC__ */
#ifdef _POSIX_C_SOURCE
#  include <sys/time.h>
#endif /* _POSIX_C_SOURCE */
#ifdef __MWERKS__
#  include <console.h>
#  define CONSOLE 1
#endif /* __MWERKS__ */

/********************************************************** #DEFINE'S */

#define LINEWIDTH    80
#define PAGEHEIGHT   25

#define SCREENWIDTH  79
#define SCREENHEIGHT 22

#define DEBUGROW 24
#define cur pg[y * LINEWIDTH + x]

/********************************************************* STRUCTURES */

struct stack                    /* stack structure, for values on stack */
{
  signed long val;
  struct stack *next;
} * head;                       /* head of stack */

/*************************************************** GLOBAL VARIABLES */

char pg[LINEWIDTH * PAGEHEIGHT]; /* befunge 'page' of source */
int x = 0, y = 0;                /* x and y of the PC */
int dx = 1, dy = 0;              /* direction of the PC */
int debug = 0;                   /* flag : display ANSI debugging? */
int infile = 0, ia;              /* flag : use input file, and assoc arg? */
int outfile = 0, oa;             /* flag : use output file, and assoc arg? */
int stackfile = 0, sa;           /* flag : use stack log file, & assoc arg? */
int stringmode = 0;              /* flag : are we in string mode? */
int quiet = 0;                   /* flag : are we quiet? */
int v10err_compat = 0;           /* flag : emulate v1.0 off-by-one err? */
int deldur = 25;                 /* debugging delay in milliseconds */
int ignore_unsupported = 0;      /* flag : ignore unsupported instructions? */
int use_b97directives = 0;       /* flag : use b97-esque directives? */
int wrap_long_program_lines = 0; /* flag : long program lines wrap? */
int inconsistent_trampoline = 0; /* flag : should # wrap inconsistently? */

/********************************************************* PROTOTYPES */

void befsleep (int);
void push (signed long val);
signed long pop (void);
void usage (void);
int main (int, char **);

/******************************************************* MAIN PROGRAM */

int main (argc, argv)
     int argc;
     char **argv;
{
  FILE *f=NULL;
  FILE *fi=NULL;
  FILE *fo=NULL;
  FILE *fs=NULL;
  int i;
  char filename[128];

#ifdef __MWERKS__
  argc = ccommand(&argv);
#endif /* __MWERKS__ */

  srand((unsigned)time(0));

  if (argc < 2)
  {
    usage();
  }
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-') {
      if (!strcmp(argv[i], "-o")) { v10err_compat = 1; }
      else if (!strcmp(argv[i], "-d")) { debug = 1; }
      else if (!strcmp(argv[i], "-r")) { infile = 1; ia = i + 1; }
      else if (!strcmp(argv[i], "-w")) { outfile = 1; oa = i + 1; }
      else if (!strcmp(argv[i], "-s")) { stackfile = 1; sa = i + 1; }
      else if (!strcmp(argv[i], "-y")) { deldur = atoi(argv[++i]); }
      else if (!strcmp(argv[i], "-q")) { quiet = 1; }
      else if (!strcmp(argv[i], "-i")) { ignore_unsupported = 1; }
      else if (!strcmp(argv[i], "-=")) { use_b97directives = 1; }
      else if (!strcmp(argv[i], "-l")) { wrap_long_program_lines = 1; }
      else if (!strcmp(argv[i], "-t")) { inconsistent_trampoline = 1; }
      else
      {
        printf("Unknown option: %s\n", argv[i]);
        usage();
      }
    }
  }
  if (!quiet)
  {
    printf ("Befunge-93 Interpreter/Debugger v2.23\n");
  }

  memset(pg, ' ', LINEWIDTH * PAGEHEIGHT);

  strcpy(filename, argv[argc - 1]);
  if (strchr(filename, '.') == NULL)
  {
    strcat(filename, ".bf");
  }

  if ((f = fopen (filename, "r")) != NULL)             /*** Input Phase */
  {
    int x = 0, y = 0;
    char dc = '=', tc = ' ';
    int tt = 0; char s[80];
    int accept_pound = 1;

    while (!feof (f))
    {
      tc = fgetc (f);
      if (feof (f))
        break;
      if (use_b97directives && (x == 0) &&
          ((tc == dc) || (accept_pound && (tc == '#'))))
      {
        if (tc != '#') accept_pound = 0;
        tc = fgetc (f);
        if (tc == 'l')
        {
          while (tc != ' ')
          {
            tc = fgetc (f);
          }
          while (tc != '\n')
          {
            tc = fgetc (f);
            if (tc != '\n') { s[tt++] = tc; s[tt] = (char)0; }
          }
          if (strcmp(s, "b93"))
          {
            fprintf(stderr, "Error: only Befunge-93 (not %s) sources are supported by bef.\n", s);
            exit(10);
          }
        }
        while (tc != '\n')
        {
          tc = fgetc (f);
        }
      } else
      {
        accept_pound = 0;
        if (tc == '\n')
        {
          x = 0;
          y++;
          if (y >= PAGEHEIGHT) break;
        } else
        {
          cur = tc;
          x++;
          if (x >= LINEWIDTH)
          {
            if (!wrap_long_program_lines)
            {
              while (tc != '\n')
              {
                tc = fgetc (f);
                if (feof(f)) { y = PAGEHEIGHT; break; }
              }
            }
            x = 0;
            y++;
            if (y >= PAGEHEIGHT) break;
          }
        }
      }
    }
    fclose (f);
  } else
  {
    printf ("Error: couldn't open '%s' for input.\n", filename);
    exit (0);
  }

  if (infile)
  {
    if (!(fi = fopen (argv[ia], "r")))
    {
      printf ("Error : couldn't open '%s' for input.\n", argv[ia]);
      exit (0);
    }
  }

  if (outfile)
  {
    if (!(fo = fopen (argv[oa], "w")))
    {
      printf ("Error : couldn't open '%s' for output.\n", argv[oa]);
      exit (0);
    }
  }

  if (stackfile)
  {
    if (!(fs = fopen (argv[sa], "w")))
    {
      printf ("Error : couldn't open '%s' for output.\n", argv[sa]);
      exit (0);
    }
  }

  if (debug)
  {

#ifdef CONSOLE
    clrscr();
#  ifdef CURSORSHAPE
    _setcursortype(_NOCURSOR);
#  endif /* CURSORSHAPE */
#else
    printf ("%c[1;1H", 27);
    printf ("%c[2J", 27);
#endif /* CONSOLE */

    for(y = 0; y < SCREENHEIGHT; y++)
    {
      for(x = 0; x < SCREENWIDTH; x++)
      {
        if (isprint((int)cur))
        {
          printf("%c", cur);
        }
      }
      printf("\n");
    }

    x = y = 0;

#ifdef CUSRORSHAPE
    _setcursortype(_SOLIDCURSOR);
#endif /* CUSRORSHAPE */

  }

  while ((cur != '@') || (stringmode))          /*** Intepreting Phase */
  {
    if (debug)
    {
      if ((y < SCREENHEIGHT) && (x < SCREENWIDTH))
      {
#ifdef CONSOLE
        gotoxy(x+1, y+1);
        if (kbhit())
        {
          char c;
          /* ideally, pop up a debugging tool. for now, exit. */
          c = getch();
          if (c == 0)
          {
            c = getch();
          } else
          {
            if (c == 27)
            {
              /* pause */
              c = getch();
              if (c == 0)
              {
                c = getch();
              } else
              {
                if (c == 27)
                {
                  goto the_end;
                }
              }
            }
          }
        }
#else
        printf ("%c[%d;%dH", 27, y+1, x+1);
        fflush (stdout);
#endif /* CONSOLE */
      }
      befsleep (deldur);
    }
    if (stringmode && (cur != '"'))
      push (cur);
      else if (isdigit ((int)cur))
        push (cur - '0');
      else
        switch (cur)
        {
           case '>':            /* PC Right */
             dx = 1;
             dy = 0;
             break;
           case '<':            /* PC Left */
             dx = -1;
             dy = 0;
             break;
           case '^':            /* PC Up */
             dx = 0;
             dy = -1;
             break;
           case 'v':            /* PC Down */
             dx = 0;
             dy = 1;
             break;
           case '|':            /* Vertical 'If' */
             dx = 0;
             if (pop ())
               dy = -1;
             else
               dy = 1;
             break;
           case '_':            /* Horizontal 'If' */
             dy = 0;
             if (pop ())
               dx = -1;
             else
               dx = 1;
             break;
           case '+':            /* Add */
             push (pop () + pop ());
             break;
           case '-':            /* Subtract */
             {
               long a = pop();
               long b = pop();
               push(b - a);
             }
             break;
           case '*':            /* Multiply */
             push (pop () * pop ());
             break;
           case '/':            /* Integer Divide */
             {
               signed long a = pop ();
               signed long b = pop ();
               if (a == 0)
               {
                 if (!outfile)
                 {
                   printf("What do you want %ld/0 to be? ", b);
                 } else
                 {
                   fprintf(fo, "What do you want %ld/0 to be? ", b);
                 }
                 if (infile)
                 {
                   fscanf (fi, "%ld", &b);
                   push (b);
                 } else
                 {
                   if (!debug)
                   {
                     fscanf (stdin, "%ld", &b);
                     push (b);
                   }
                 }
               } else
               {
                 push (b / a);
               }
             }
             break;
           case '%':            /* Modulo */
             {
               signed long a = pop ();
               signed long b = pop ();
               push (b % a);
             }
             break;
           case '\\':           /* Swap */
             {
               signed long a = pop ();
               signed long b = pop ();
               push (a);
               push (b);
             }
             break;
           case '.':            /* Pop Out Integer */
             {
               if (outfile)
               {
                 fprintf (fo, "%ld ", pop ());
                 fflush (fo);
               } else
               {
                 if (!debug)
                 {
                   fprintf (stdout, "%ld ", pop ());
                   fflush (stdout);
                 } else
                 {
#ifdef CONSOLE
                   int x, y;
                   char s[172], t[172];
                   x = wherex();
                   y = wherey();
                   sprintf(s, "%ld ", pop());
                   gettext(1+strlen(s), DEBUGROW, 80, DEBUGROW, t);
                   puttext(1, DEBUGROW, 80-strlen(s), DEBUGROW, t);
                   gotoxy(81-strlen(s), DEBUGROW);
                   cputs(s);
                   gotoxy(x, y);
#endif /* CONSOLE */
                 }
               }
             }
             break;
           case ',':            /* Pop Out ASCII */
             {
               if (outfile)
               {
                 fprintf (fo, "%c", (char)pop ());
                 fflush (fo);
               } else
               {
                 if (!debug)
                 {
                   fprintf (stdout, "%c", (char)pop ());
                   fflush (stdout);
                 } else
                 {
#ifdef CONSOLE
                   int x, y;
                   long int p = pop();
                   char t[172];
                   x = wherex();
                   y = wherey();
                   gettext(2, DEBUGROW, 80, DEBUGROW, t);
                   puttext(1, DEBUGROW, 79, DEBUGROW, t);
                   gotoxy(80, DEBUGROW);
                   if ((p >= 32) && (p <= 255))
                   {
                     putch((int)p);
                   } else
                   {
                     if (p == 10)
                     {
                       putch(179);
                     } else
                     {
                       putch(168);
                     }
                   }
                   gotoxy(x, y);
#endif /* CONSOLE */
                 }
               }
             }
             break;
           case '"':            /* Toggle String Mode */
             stringmode = !stringmode;
             break;
           case ':':            /* Duplicate */
             {
               signed long a = pop ();
               push (a);
               push (a);
             }
             break;
           case '!':            /* Negate */
          if (pop())
            push(0);
          else
            push(1);
             break;
           case '`':
             {
               signed long b = pop ();
               signed long a = pop ();
               push (a > b);
             }
             break;
           case '#':            /* Bridge */
             x += dx;
             y += dy;
             break;
           case '$':            /* Pop and Discard */
             pop ();
             break;
           case '?':            /* Random Redirect */
             switch ((rand () / 32) % 4)
                {
                case 0:
                  dx = 1;
                  dy = 0;
                  break;
                case 1:
                  dx = -1;
                  dy = 0;
                  break;
                case 2:
                  dx = 0;
                  dy = -1;
                  break;
                case 3:
                  dx = 0;
                  dy = 1;
                  break;
                }
             break;
           case '&':            /* Input Integer */
             {
               signed long b;
               if (infile)
               {
                 fscanf (fi, "%ld", &b);
                 push (b);
               } else
               {
                 if (!debug)
                 {
                   fscanf (stdin, "%ld", &b);
                   push (b);
                 } else
                 {
#ifdef CONSOLE
                   int x, y;
                   long int p;
                   char t[172];
                   x = wherex();
                   y = wherey();
                   gettext(10, DEBUGROW, 80, DEBUGROW, t);
                   puttext(1, DEBUGROW, 71, DEBUGROW, t);
                   gotoxy(72, DEBUGROW);
                   clreol();
                   cscanf("%ld", &p);
                   push(p);
                   gotoxy(x, y);
#endif /* CONSOLE */
                 }
               }
             }
             break;
           case '~':            /* Input ASCII */
             {
               char c;
               if (infile)
               {
                 c = fgetc (fi);
                 push (c);
               } else
               {
                 if (!debug)
                 {
                   c = fgetc (stdin);
                   push (c);
                 } else
                 {
#ifdef CONSOLE
                   int x, y;
                   long int p;
                   char t[172];
                   x = wherex();
                   y = wherey();
                   gettext(2, DEBUGROW, 80, DEBUGROW, t);
                   puttext(1, DEBUGROW, 79, DEBUGROW, t);
                   gotoxy(80, DEBUGROW);
                   clreol();
                   p = getche();
                   if (p == '\r')
                   {
                     p = '\n';
                     gotoxy(80, DEBUGROW);
                     putch(179);
                   }
                   push(p);
                   gotoxy(x, y);
#endif /* CONSOLE */
                 }
               }
             }
             break;
           case 'g':            /* Get Value */
             {
               signed long y = pop (), x = pop ();
               if ((y < PAGEHEIGHT) && (y >= 0) && (x < LINEWIDTH) && (x >= 0))
               {
                 push (cur);
               } else
               {
                 if (!debug)
                 {
                   if (!quiet)
                   {
                     fprintf(stderr, "g 'Get' instruction out of bounds (%ld,%ld)\n", x, y);
                   }
                 }
                 push (0);
               }
             }
             break;
           case 'p':            /* Put Value */
             {
               signed long y = pop (), x = pop ();
               if ((y < PAGEHEIGHT) && (y >= 0) && (x < LINEWIDTH) && (x >= 0))
               {
                 cur = pop ();
               } else
               {
                 if (!debug)
                 {
                   if (!quiet)
                   {
                     fprintf(stderr, "p 'Put' instruction out of bounds (%ld,%ld)\n", x, y);
                   }
                 }
                 pop();
               }
               if ((debug) && (y < SCREENHEIGHT) && (x < SCREENWIDTH))
               {
#ifdef CONSOLE
                 gotoxy(x+1,y+1);
#else
                 printf ("%c[%d;%dH", 27, (int)(y+1), (int)(x+1));
#endif /* CONSOLE */
                 if (isprint ((int)cur)) printf ("%c", cur); else printf(".");
               }
             }
             break;
           case ' ':
             break;
           default:
             if ((!debug) && (!ignore_unsupported) && (!quiet))
             {
               fprintf(stderr, "Unsupported instruction '%c' (0x%02x) (maybe not Befunge-93?)\n", cur, cur);
             }
             break;
        }
      x += dx;
      y += dy;
      if (x < 0)
        if (v10err_compat)
        {
          x = LINEWIDTH;
        } else if (inconsistent_trampoline)
        {
          x = LINEWIDTH - 1;
        } else
        {
          x += LINEWIDTH;
        }
      else
        x = x % LINEWIDTH;
      if (y < 0)
        if (v10err_compat)
        {
          y = PAGEHEIGHT;
        } else if (inconsistent_trampoline)
        {
          y = PAGEHEIGHT - 1;
        } else
        {
          y += PAGEHEIGHT;
        }
      else
        y = y % PAGEHEIGHT;
      if (stackfile)
      {
        struct stack *s;
        for (s = head; s; s = s->next)
          fprintf(fs, "%ld ", s->val);
        fprintf(fs, "\n");
        fflush(fs);
      }
    }

#ifdef CONSOLE
the_end:
#endif /* CONSOLE */

  while (head != NULL) { pop(); }
  if (fi != NULL) fclose (fi);
  if (fo != NULL) fclose (fo);
  if (fs != NULL) fclose (fs);

  if (debug)
  {
#ifdef CONSOLE
#  ifdef CURSORSHAPE
    _setcursortype(_NORMALCURSOR);
#  endif /* CURSORSHAPE */
    gotoxy(1,22);
#else
    printf ("%c[22;1H", 27);
#endif /* CONSOLE */
  }

  exit (0);
  return 0;
}

/*
 * sleep for the given number of milliseconds, so far as we are able
 */
void befsleep (dur)
    int dur;
{
#if MSDOS
      delay (dur);
#else
  #ifdef _POSIX_C_SOURCE
        struct timespec tv;

        tv.tv_sec = dur / 1000;
        tv.tv_nsec = (dur % 1000) * 1000000;

        nanosleep(&tv, NULL);
  #else
        sleep (dur / 1000);
  #endif
#endif
}

/*
 * pushes a value onto the stack.
 */
void push (val)
    signed long val;
{
  struct stack *s;
  s = (struct stack *) malloc (sizeof (struct stack));
  s->val = val;
  s->next = head;
  head = s;
}

/*
 * pops a value off the stack. returns 0 in case of underflow.
 */
signed long pop ()
{
  signed long v;
  struct stack *s = head;
  if (s)
  {
    v = head->val;
    head = head->next;
    free (s);
    return v;
  } else
  {
    return 0;
  }
}

void usage ()
{
  printf ("USAGE: bef [-d] [-o] [-q] [-i] [-=] [-l] [-t]\n");
  printf ("           [-r input] [-w output] [-s stack] [-y delay] foo.bf\n");
  exit (1);
}
