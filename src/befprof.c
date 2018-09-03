/********************************************************************

   befprof.c - Befunge-93 Profiler

   Copyright (c)1998-2018, Chris Pressey, Cat's Eye Technologies.
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

   befprof [-l] [-i] [-n count] [-r input-file] [-w report-file]
           <befunge-source>

      -i: use interactive I/O
      -r: redirect input from a specified file instead of stdin
          generally use these only if random input and logic
          does not well reflect reality, or causes a problem
          with your source code (e.g. anagram.bf)
      -l: randomize logic - makes | and _ execute randomly.
      -w: specify an output filename for report (if omitted,
          <input-file>.map is used)
      -n: repeat execution a given number of times (def. 1)
          use to generate data with a better resolution.
          although note that cells are stored as 2-byte words,
          so too many repetitions may cause them to wrap back to 0.

   Known to Compile Under :

        gcc 5.4.0 (Ubuntu 16.04)
        gcc 4.5.3 (NetBSD 6.1.5)
        DICE C 3.15 (AmigaDOS 1.3)
        DJGPP 2.05 gcc 8.1.0 (32-bit Protected-Mode, FreeDOS 1.1)
        Borland C++ v3.1 (16-bit, FreeDOS 1.1)
          (NOTE: use "compact" or "large" memory model!)
        Microsoft Visual C++ 14.15 (see NMakefile)

   Has, in the Past, been Known to Compile Under:

        Metrowerks CodeWarrior (MacOS)

   ******************************************************************

   History:

   v1.0: Sep 2018, Chris Pressey
          handle trampoline at leftmost/topmost edges
            (thanks to https://github.com/serprex for this fix!)
          exit with non-zero error code on error
          remove unimplemented -q option from usage help text
          don't load invalid (past-EOF) bytes into playfield

   v0.94: Sep 2004, Chris Pressey
          cleanup only, no functional changes

   v0.93: Jul 2000, Chris Pressey
          added defines for Metrowerks CodeWarrior
            so that befprof will build on MacOS
          relicensed under BSD license

   v0.92: Mar 1998, Chris Pressey
          original Befunge-93 Profiler 'befprof' distribution.

   ****************************************************************** */

/********************************************************* #INCLUDE'S */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/********************************************************** #DEFINE'S */

#define LINEWIDTH    80
#define PAGEHEIGHT   25

#define cur pg[y * LINEWIDTH + x]
#define curbuf pgbuf[y * LINEWIDTH + x]
#define incprof(z) prof[z][y * LINEWIDTH + x]++;
#define curprof(z) prof[z][y * LINEWIDTH + x]

/********************************************************* STRUCTURES */

struct stack                    /* stack structure, for values on stack */
{
  signed long val;
  struct stack *next;
} *head;                        /* head of stack */

/*************************************************** GLOBAL VARIABLES */

char * pg;                       /* befunge 'page' of source */
char * pgbuf;                    /* where we load befunge 'page' of source */
int x = 0, y = 0;                /* x and y of the PC */
int dx = 1, dy = 0;              /* direction of the PC */
int infile = 0, ia;              /* flag : use input file, and assoc arg? */
int stringmode = 0;              /* flag : are we in string mode? */
int reps = 1;
int rep = 1;
int interactive = 0;
int rand_logic = 0;
unsigned long int curiter = 0;
unsigned long int maxiter = 500000;
int putoob = 0, unsupp = 0;

#define INGRESS_NORTH 0
#define INGRESS_SOUTH 1
#define INGRESS_EAST 2
#define INGRESS_WEST 3
#define EGRESS_NORTH 4
#define EGRESS_SOUTH 5
#define EGRESS_EAST 6
#define EGRESS_WEST 7
#define PUT_CELL 8
#define GET_CELL 9

#define PROFILE_HANDLES 10

unsigned long int * prof[PROFILE_HANDLES];

/********************************************************* PROTOTYPES */

void push (signed long val);
signed long pop (void);
int main (int, char **);

/******************************************************* MAIN PROGRAM */

int main (argc, argv)
    int argc;
    char **argv;
{
  FILE *f=NULL;
  FILE *fi=NULL;
  FILE *fp=NULL;
  int i;
  int flag_egress;
  char filename[128];
  char mapfilename[128];

#ifdef __MWERKS__
  argc = ccommand(&argv);
#endif /* __MWERKS__ */

  srand (time (0));

  pg = malloc(LINEWIDTH * PAGEHEIGHT);
  pgbuf = malloc(LINEWIDTH * PAGEHEIGHT);
  if ((pg == NULL) || (pgbuf == NULL))
  {
    printf ("Error: can't allocate %d bytes of memory.\n", LINEWIDTH * PAGEHEIGHT);
    exit (1);
  }
  memset(pg, ' ', LINEWIDTH * PAGEHEIGHT);
  memset(pgbuf, ' ', LINEWIDTH * PAGEHEIGHT);

  for(i=0;i<PROFILE_HANDLES;i++)
  {
    prof[i] = malloc(LINEWIDTH * PAGEHEIGHT * sizeof(long int));
    if (prof[i] == NULL)
    {
      printf ("Error: can't allocate %lu bytes of memory.\n", (long)(LINEWIDTH * PAGEHEIGHT * sizeof(long int)));
      exit (1);
    }
    memset(prof[i], 0, LINEWIDTH * PAGEHEIGHT * sizeof(long int));
  }

  if (argc < 2)
  {
    printf ("USAGE: befprof [-l] [-i] [-n count] [-r input] [-w foo.map] foo.bf\n");
    exit (1);
  }

  strcpy(filename, argv[argc - 1]);
  if (strchr(filename, '.') == NULL)
  {
    strcpy(mapfilename, filename);
    strcat(mapfilename, ".map");
    strcat(filename, ".bf");
  } else
  {
    char *xs;
    strcpy(mapfilename, filename);
    xs = strchr(mapfilename, '.');
    xs[0] = 0;
    strcat(mapfilename, ".map");
  }

  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-r")) { infile = 1; ia = i + 1; }
    if (!strcmp(argv[i], "-i")) { interactive = 1; }
    if (!strcmp(argv[i], "-l")) { rand_logic = 1; }
    if (!strcmp(argv[i], "-n")) { reps = atoi(argv[i + 1]); }
    if (!strcmp(argv[i], "-w")) { strcpy(mapfilename, argv[i + 1]); }
  }

  printf ("Befunge-93 Profiler v1.0\n");

  if ((f = fopen (filename, "r")) != NULL)             /*** Input Phase */
  {
    int x = 0, y = 0;

    while (!feof (f))
    {
      curbuf = fgetc (f);
      if (feof (f))
      {
        curbuf = ' ';
        break;
      }
      if (curbuf == '\n')
      {
        curbuf = ' ';
        x = 0;
        y++;
        if (y >= PAGEHEIGHT) break;
      } else
      {
        x++;
        if (x >= LINEWIDTH)
        {
          x = 0;
          y++;
          if (y >= PAGEHEIGHT) break;
        }
      }
    }
    fclose (f);
  } else
  {
    printf ("Error: couldn't open '%s' for input.\n", filename);
    exit (1);
  }

  if (!(fp = fopen (mapfilename, "w")))
  {
    printf ("Error : couldn't open '%s' for output.\n", mapfilename);
    exit (1);
  }

  while (rep <= reps)
  {
    x = 0;
    y = 0;
    dx = 1;
    dy = 0;
    stringmode = 0;
    curiter = 0;

    memcpy(pg, pgbuf, LINEWIDTH * PAGEHEIGHT);

    if (infile)
    {
      if (!(fi = fopen (argv[ia], "r")))
      {
        printf ("Error : couldn't open '%s' for input.\n", argv[ia]);
        exit (1);
      }
    }

    while ((cur != '@') || (stringmode))          /*** Intepreting Phase */
    {
      flag_egress = 0;
      if ((dx == 1) && (dy == 0)) incprof(INGRESS_WEST);
      if ((dx == -1) && (dy == 0)) incprof(INGRESS_EAST);
      if ((dx == 0) && (dy == 1)) incprof(INGRESS_NORTH);
      if ((dx == 0) && (dy == -1)) incprof(INGRESS_SOUTH);
      if (stringmode && (cur != '"'))
      {
        push (cur);
      } else
      if (isdigit ((int)cur))
      {
        push (cur - '0');
      } else
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
        {
          long int pp = pop();
          dx = 0;
          if (rand_logic) pp = ((rand () >> 3) % 2);
          if (pp)
          {
            dy = -1;
          } else
          {
            dy = 1;
          }
          break;
        }
        case '_':            /* Horizontal 'If' */
        {
          long int pp = pop();
          dy = 0;
          if (rand_logic) pp = ((rand () >> 3) % 2);
          if (pp)
          {
            dx = -1;
          } else
          {
            dx = 1;
          }
          break;
        }
        case '+':            /* Add */
          push (pop () + pop ());
          break;
        case '-':            /* Subtract */
        {
          long a = pop();
          long b = pop();
          push(b - a);
          break;
        }
        case '*':            /* Multiply */
          push (pop () * pop ());
          break;
        case '/':            /* Integer Divide */
        {
          signed long a = pop ();
          signed long b = pop ();
          if (a == 0)
          {
            if (infile)
            {
              fscanf (fi, "%ld", &b);
              push (b);
            } else
            if (interactive)
            {
              printf("What do you want %ld/0 to be? ", b);
              scanf ("%ld", &b);
              push (b);
            } else
            {
              push (rand() - 32767);
            }
          } else
          {
            push (b / a);
          }
          break;
        }
        case '%':            /* Modulo */
        {
          signed long a = pop ();
          signed long b = pop ();
          push (b % a);
          break;
        }
        case '\\':           /* Swap */
        {
          signed long a = pop ();
          signed long b = pop ();
          push (a);
          push (b);
          break;
        }
        case '.':            /* Pop Out Integer */
        {
          if (interactive)
          {
            printf ("%ld ", pop ());
            fflush (stdout);
          } else pop();
          break;
        }
        case ',':            /* Pop Out ASCII */
        {
          if (interactive)
          {
            printf ("%c", (char)pop ());
            fflush (stdout);
          } else pop();
          break;
        }
        case '"':               /* Toggle String Mode */
          stringmode = !stringmode;
          break;
        case ':':            /* Duplicate */
        {
          signed long a = pop ();
          push (a);
          push (a);
          break;
        }
        case '!':            /* Negate */
        {
          if (pop())
          {
            push(0);
          } else
          {
            push(1);
          }
          break;
        }
        case '`':
        {
          signed long b = pop ();
          signed long a = pop ();
          if (a > b)
          {
            push(1);
          } else
          {
            push(0);
          }
          break;
        }
        case '#':            /* Bridge */
        {
          if ((dx == 1) && (dy == 0)) incprof(EGRESS_EAST);
          if ((dx == -1) && (dy == 0)) incprof(EGRESS_WEST);
          if ((dx == 0) && (dy == 1)) incprof(EGRESS_SOUTH);
          if ((dx == 0) && (dy == -1)) incprof(EGRESS_NORTH);
          x += dx;
          y += dy;
          flag_egress = 1;
          break;
        }
        case '$':            /* Pop and Discard */
        {
          pop ();
          break;
        }
        case '?':            /* Random Redirect */
        {
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
        }
        case '&':            /* Input Integer */
        {
          signed long b;
          if (infile)
          {
            fscanf (fi, "%ld", &b);
            push (b);
          } else
          if (interactive)
          {
            scanf ("%ld", &b);
            push (b);
          } else
          {
            push (rand() - 32767);
          }
          break;
        }
        case '~':            /* Input ASCII */
        {
          char c;
          if (infile)
          {
            c = fgetc (fi);
            push (c);
          } else
          if (interactive)
          {
            c = fgetc (stdin);
            push (c);
          } else
          {
            c = ((rand() / 32) % 128);
            while (c < 32)
            {
              c = ((rand() / 32) % 128);
            }
            push (c);
          }
          break;
        }
        case 'g':            /* Get Value */
        {
          signed long y = pop (), x = pop ();
          incprof(GET_CELL);
          push (cur);
          break;
        }
        case 'p':            /* Put Value */
        {
          signed long y = pop (), x = pop ();
          if ((y < PAGEHEIGHT) && (y >= 0) && (x < LINEWIDTH) && (x >= 0))
          {
            incprof(PUT_CELL);
            cur = pop ();
          } else
          {
            /* fprintf(stderr, "p 'Put' instruction out of bounds (%d,%d)\n", x, y); */
            putoob++;
          }
          break;
        }
        case ' ':
          break;
        default:
        {
          unsupp++;
          break;
        }
      }
      if (!flag_egress)
      {
        if ((dx == 1) && (dy == 0)) incprof(EGRESS_EAST);
        if ((dx == -1) && (dy == 0)) incprof(EGRESS_WEST);
        if ((dx == 0) && (dy == 1)) incprof(EGRESS_SOUTH);
        if ((dx == 0) && (dy == -1)) incprof(EGRESS_NORTH);
      }
      x += dx;
      y += dy;
      if (x < 0)
      {
        x += LINEWIDTH;
      } else
      {
        x = x % LINEWIDTH;
      }
      if (y < 0)
      {
        y += PAGEHEIGHT;
      } else
      {
        y = y % PAGEHEIGHT;
      }

      curiter++;
      if (curiter > maxiter)
      {
        printf("more than %ld iterations\n", maxiter);
        break;
      }
    }

    while (head != NULL) { pop(); }
    if (fi != NULL)
    {
      fclose (fi);
      fi = NULL;
    }
    printf("Finished executing %d/%d (%ld ticks)\n", rep, reps, curiter-1);
    rep++;
  }

  if (fp != NULL)
  {
    /* print out results */
    int x, y, z; char c;
    unsigned long int maxprof;
    fprintf(fp, "BEGIN map 0; bf93prof - source file\n");
    for (y=0;y<PAGEHEIGHT;y++)
    {
      for (x=0;x<LINEWIDTH;x++)
      {
        fprintf(fp, "%c", cur);
      }
      fprintf(fp, "\n");
    }

    for(z=0;z<PROFILE_HANDLES;z++)
    {
      fprintf(fp, "BEGIN map %d; bf93prof %s ", z + 1, filename);
      switch(z)
      {
        case INGRESS_NORTH: fprintf(fp, "ip enters from north"); break;
        case INGRESS_SOUTH: fprintf(fp, "ip enters from south"); break;
        case INGRESS_EAST: fprintf(fp, "ip enters from east"); break;
        case INGRESS_WEST: fprintf(fp, "ip enters from west"); break;
        case EGRESS_NORTH: fprintf(fp, "ip exits to north"); break;
        case EGRESS_SOUTH: fprintf(fp, "ip exits to south"); break;
        case EGRESS_EAST: fprintf(fp, "ip exits to east"); break;
        case EGRESS_WEST: fprintf(fp, "ip exits to west"); break;
        case PUT_CELL: fprintf(fp, "g (get) addresses cell"); break;
        case GET_CELL: fprintf(fp, "p (put) addresses cell"); break;
      }
      fprintf(fp, "\n");
      maxprof = 0;
      {
        for (y=0;y<PAGEHEIGHT;y++)
        {
          for (x=0;x<LINEWIDTH;x++)
          {
            if (curprof(z) > maxprof) maxprof = curprof(z);
          }
        }
        for (y=0;y<PAGEHEIGHT;y++)
        {
          for (x=0;x<LINEWIDTH;x++)
          {
            if (maxprof > 0)
            {
              c = '0' + ((curprof(z) * 10) / maxprof);
              if (curprof(z) == 0) c = '.';
              if (curprof(z) == maxprof) c = '*';
              if ((c > '9') && (c != '*') && (c != '.'))
              {
                c = '*';
              }
              fprintf(fp, "%c", c);
            } else
            {
              fprintf(fp, ".");
            }
          }
          fprintf(fp, "\n");
        }
      }
      fprintf(fp, "END map %d\n", z + 1);
    }

    fprintf(fp, "BEGIN map %d; bf93prof %s ", PROFILE_HANDLES + 1, filename);
    fprintf(fp, "all possible ip ingresses (N8S4E2W1)\n");

    for (y=0;y<PAGEHEIGHT;y++)
    {
      for (x=0;x<LINEWIDTH;x++)
      {
        int b=0;
        if (curprof(INGRESS_NORTH) > 0) { b |= 0x08; }
        if (curprof(INGRESS_SOUTH) > 0) { b |= 0x04; }
        if (curprof(INGRESS_EAST) > 0) { b |= 0x02; }
        if (curprof(INGRESS_WEST) > 0) { b |= 0x01; }
        if (b)
        {
          fprintf(fp, "%x", b);
        } else
        {
          fprintf(fp, ".");
        }
      }
      fprintf(fp, "\n");
    }
    fprintf(fp, "END map %d\n", PROFILE_HANDLES + 1);

    fprintf(fp, "BEGIN map %d; bf93prof %s ", PROFILE_HANDLES + 2, filename);
    fprintf(fp, "all possible ip egresses (N8S4E2W1)\n");

    for (y=0;y<PAGEHEIGHT;y++)
    {
      for (x=0;x<LINEWIDTH;x++)
      {
        int b=0;
        if (curprof(EGRESS_NORTH) > 0) { b |= 0x08; }
        if (curprof(EGRESS_SOUTH) > 0) { b |= 0x04; }
        if (curprof(EGRESS_EAST) > 0) { b |= 0x02; }
        if (curprof(EGRESS_WEST) > 0) { b |= 0x01; }
        if (b)
        {
          fprintf(fp, "%x", b);
        } else
        {
          fprintf(fp, ".");
        }
      }
      fprintf(fp, "\n");
    }
    fprintf(fp, "END map %d\n", PROFILE_HANDLES + 2);

    fprintf(fp, "BEGIN map %d; bf93prof %s ", PROFILE_HANDLES + 3, filename);
    fprintf(fp, "cells by: get (4) put (2) execute (1)\n");

    for (y=0;y<PAGEHEIGHT;y++)
    {
      for (x=0;x<LINEWIDTH;x++)
      {
        int b=0;
        if ((curprof(EGRESS_NORTH) > 0) ||
            (curprof(EGRESS_SOUTH) > 0) ||
            (curprof(EGRESS_EAST) > 0) ||
            (curprof(EGRESS_WEST) > 0) ||
            (curprof(INGRESS_NORTH) > 0) ||
            (curprof(INGRESS_SOUTH) > 0) ||
            (curprof(INGRESS_EAST) > 0) ||
            (curprof(INGRESS_WEST) > 0))
        {
          b |= 0x01;
        }
        if (curprof(PUT_CELL) > 0)
        {
          b |= 0x02;
        }
        if (curprof(GET_CELL) > 0)
        {
          b |= 0x04;
        }
        if (b)
        {
          fprintf(fp, "%x", b);
        } else
        {
          fprintf(fp, ".");
        }
      }
      fprintf(fp, "\n");
    }
    fprintf(fp, "END map %d\n", PROFILE_HANDLES + 3);

    fclose (fp);
  }

  free (pg);
  free (pgbuf);
  for(i=0;i<PROFILE_HANDLES;i++)
  {
    free (prof[i]);
  }

  exit (0);
  return 0;
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

/* END of befprof.c */
