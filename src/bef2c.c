/********************************************************************

   bef2c.c - Befunge-93 to ANSI C Compiler in ANSI C

   Copyright (c)1997-2018, Chris Pressey, Cat's Eye Technologies.
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

   bef2c [-O] [-w width] [-h height] <befunge-source> <c-destination>

     -O : enable [buggy] post-optimization phase
     -w : explicit width
     -h : explicit height

   Known to Compile Under :

        gcc 5.4.0 (Ubuntu 16.04)
        gcc 4.5.3 (NetBSD 6.1.5)
        DJGPP 2.05 gcc 8.1.0 (32-bit Protected-Mode, FreeDOS 1.1)
        Borland C++ v3.1 (16-bit, FreeDOS 1.1)
        Microsoft Visual C++ 14.15 (see NMakefile)

   Has, in the Past, been Known to Compile Under:

        Metrowerks CodeWarrior (MacOS)

   ******************************************************************

   History:

     v1.0: Sep 2018, Chris Pressey
        fixes submitted by https://github.com/serprex (thanks!):
          - avoid double free due to double fclose
          - handle trampoline at leftmost/topmost edges
          - don't load NULs into playfield
          - use %% instead of %c with '%' being passed in
          - avoid freeing fo/fi on failure to open
        removed -p flag as it is equivalent to
          `-w 80 -h 25`, just use that instead
        replaced -o (disable post-optimization) flag
          with -O (enable post-optimization) because buggy
        show usage and exit if unrecognized command-line
          options are given
        exit with a non-zero exit code if an error occurs
        when detecing pageheight, add one on the assumption
          that the source does not end with a newline
          (this lets some existing example programs compile)

     v0.94: Sep 2004, Chris Pressey
        display correct version number
        cleanup only, no functional changes

     v0.93, Jul 19 2000, Chris Pressey
        added defines for Metrowerks CodeWarrior
          so that bef2c will build on MacOS
        relicensed under BSD license

     v0.92, Jun 21 1997, Chris Pressey
        original release of bef2c.c

 */

/********************************************************* #INCLUDE'S */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/********************************************************** #DEFINE'S */

int linewidth = 1;       /* 80 */
int pageheight = 1;      /* 25 */

#define cur pg[y * 80 + x]

#define RIGHT "_%2.2d_%2.2d_R"
#define LEFT  "_%2.2d_%2.2d_L"
#define UP    "_%2.2d_%2.2d_U"
#define DOWN  "_%2.2d_%2.2d_D"

#define y_down   (y+1)%pageheight
#define y_up     (y ? y-1 : pageheight-1)
#define x_right  (x+1)%linewidth
#define x_left   (x ? x-1 : linewidth-1)

#define y_2down   (y+2)%pageheight
#define y_2up     ((y>1) ? y-2 : ((pageheight-2)<0 ? 0 : (y-2+pageheight)))
#define x_2right  (x+2)%linewidth
#define x_2left   ((x>1) ? x-2 : ((linewidth-2)<0 ? 0 : (x-2+linewidth)))

#define ALL RIGHT ": " LEFT ": " UP ": " DOWN ":\n"

#define ECHO(s)   fprintf(fo, RIGHT ": " s " goto " RIGHT ";\n", x, y, x_right, y); \
          fprintf(fo, LEFT ": " s " goto " LEFT ";\n", x, y, x_left, y);           \
          fprintf(fo, UP ": " s " goto " UP ";\n", x, y, x, y_up);               \
          fprintf(fo, DOWN ": " s " goto " DOWN ";\n", x, y, x, y_down);

/*************************************************** GLOBAL VARIABLES */

char in[255];
char pg[2000];                   /* befunge 'page' of source */
int x = 0, y = 0, d = 0;         /* loopers */
int post_optimize = 0;           /* flag: optimize after compile? */

int labelrefs[8000];             /* postoptimization table */
char s[255];
char t[255];

/********************************************************* PROTOTYPES */

void usage(char *);
int main (int, char **);

/********************************************************** FUNCTIONS */

void usage(char *e)
{
  printf ("USAGE : %s [-O] [-w width] [-h height] <befunge-source> <c-destination>\n", e);
  exit (1);
}

/******************************************************* MAIN PROGRAM */

int main (argc, argv)
     int argc;
     char **argv;
{
  FILE *fi;
  FILE *fo;

  int i;

#ifdef __MWERKS__
  argc = ccommand(&argv);
#endif /* __MWERKS__ */

  srand (time (0));

  printf ("Befunge-93 to ANSI C Compiler v1.0\n");

  if (argc < 3) usage(argv[0]);
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      if (!strcmp(argv[i], "-O")) { post_optimize = 1; }
      else if (!strcmp(argv[i], "-w")) { linewidth = atoi(argv[i+1]); }
      else if (!strcmp(argv[i], "-h")) { pageheight = atoi(argv[i+1]); }
      else usage(argv[0]);
    }
  }
  if ((fi = fopen (argv[argc - 2], "r")) != NULL)             /*** Input Phase */
  {
    int x = 0, y = 0;
    while (!feof (fi))
    {
      int ch = fgetc (fi);
      if (ch == -1) break;
      if (ch == '\n')
      {
        x = 0;
        y++;
        if (y >= 25) break;
        else if (y > pageheight) pageheight=y;
      } else
      {
        cur = ch;
        x++;
        if (x >= 80)
        {
          x = 0;
          y++;
          if (y >= 25) break;
          else if (y > pageheight) pageheight=y;
        } else if (x > linewidth)
        {
          linewidth = x;
        }
      }
    }
  } else
  {
    printf ("Error : couldn't open '%s' for input.\n", argv[argc - 1]);
    exit (1);
  }

  /* Most Befunge-93 sources do not end with a newline.  Therefore: */
  if (pageheight < 25) pageheight++;

  if (!(fo = fopen (argv[argc - 1], "w")))             /*** Output */
  {
    printf ("Error : couldn't open '%s' for output.\n", argv[argc - 1]);
    exit (1);
  }

  printf ("Loaded %d columns by %d rows.\n", linewidth, pageheight);
  printf ("Compiling");

  fprintf (fo, "/* %s converted to ANSI C from %s by bef2c */\n",
               argv[argc-1], argv[argc-2]);
  fprintf (fo, "#include <stdio.h>\n");
  fprintf (fo, "#include <stdlib.h>\n");
  fprintf (fo, "#include <time.h>\n\n");

  fprintf (fo, "char pg[%d];\n\n", 2000);

  fprintf (fo, "struct stack\n{\n  signed long val;\n  struct stack *next;\n} *head;\n\n");

  fprintf (fo, "void push (signed long val);\nsigned long pop (void);\n\n");

  fprintf (fo, "void main ()\n{\n  signed long a; signed long b; char c; srand (time (0));\n\n");

  for(y = 0; y < pageheight; y++)
  {
    for(x = 0; x < linewidth; x++)
    {
      if (cur) fprintf (fo, "  pg[%d]=%d;\n", y * 80 + x, cur);
    }
  }

  printf (".");

  for(y = 0; y < pageheight; y++)
  {
    for(x = 0; x < linewidth; x++)
    {
      switch (cur)
      {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          fprintf(fo, RIGHT ": push(%c); goto " RIGHT ";\n", x, y, cur, x_right, y);
          fprintf(fo, LEFT ": push(%c); goto " LEFT ";\n", x, y, cur, x_left, y);
          fprintf(fo, UP ": push(%c); goto " UP ";\n", x, y, cur, x, y_up);
          fprintf(fo, DOWN ": push(%c); goto " DOWN ";\n", x, y, cur, x, y_down);
          break;
        case '>':            /* PC Right */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  goto " RIGHT ";\n", x_right, y);
          break;
        case '<':            /* PC Left */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  goto " LEFT ";\n", x_left, y);
          break;
        case '^':            /* PC Up */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  goto " UP ";\n", x, y_up);
          break;
        case 'v':            /* PC Down */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  goto " DOWN ";\n", x, y_down);
          break;
        case '|':            /* Vert If */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  if (pop ()) goto " UP "; else goto " DOWN ";\n", x, y_up, x, y_down);
          break;
        case '_':            /* Horiz If */
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  if (pop ()) goto " LEFT "; else goto " RIGHT ";\n", x_left, y, x_right, y);
          break;
        case '+':
          ECHO("push(pop()+pop());");
          break;
        case '-':
          ECHO("a=pop();b=pop();push(b-a);");
          break;
        case '*':
          ECHO("push(pop()*pop());");
          break;
        case '/':
          ECHO("a=pop();b=pop();push(b/a);");
          break;
        case '%':
          sprintf(t, "a=pop();b=pop();push(b%%a);");
          fprintf(fo, RIGHT ": %s goto " RIGHT ";\n", x, y, t, x_right, y);
          fprintf(fo, LEFT ": %s goto " LEFT ";\n", x, y, t, x_left, y);
          fprintf(fo, UP ": %s goto " UP ";\n", x, y, t, x, y_up);
          fprintf(fo, DOWN ": %s goto " DOWN ";\n", x, y, t, x, y_down);
          break;
        case '\\':
          ECHO("a=pop();b=pop();push(a);push(b);");
          break;
        case ':':
          ECHO("a=pop();push(a);push(a);");
          break;
        case '.':
          fprintf(fo, RIGHT ": fprintf(stdout,\"%%ld \",pop());fflush(stdout); goto " RIGHT ";\n", x, y, x_right, y);
          fprintf(fo, LEFT ": fprintf(stdout,\"%%ld \",pop());fflush(stdout); goto " LEFT ";\n", x, y, x_left, y);
          fprintf(fo, UP ": fprintf(stdout,\"%%ld \",pop());fflush(stdout); goto " UP ";\n", x, y, x, y_up);
          fprintf(fo, DOWN ": fprintf(stdout,\"%%ld \",pop());fflush(stdout); goto " DOWN ";\n", x, y, x, y_down);
          break;
        case ',':
          fprintf(fo, RIGHT ": fprintf(stdout,\"%%c\",pop());fflush(stdout); goto " RIGHT ";\n", x, y, x_right, y);
          fprintf(fo, LEFT ": fprintf(stdout,\"%%c\",pop());fflush(stdout); goto " LEFT ";\n", x, y, x_left, y);
          fprintf(fo, UP ": fprintf(stdout,\"%%c\",pop());fflush(stdout); goto " UP ";\n", x, y, x, y_up);
          fprintf(fo, DOWN ": fprintf(stdout,\"%%c\",pop());fflush(stdout); goto " DOWN ";\n", x, y, x, y_down);
          break;
        case '&':
          fprintf(fo, RIGHT ": fscanf(stdin,\"%%ld\",&b);push(b); goto " RIGHT ";\n", x, y, x_right, y);
          fprintf(fo, LEFT ": fscanf(stdin,\"%%ld\",&b);push(b); goto " LEFT ";\n", x, y, x_left, y);
          fprintf(fo, UP ": fscanf(stdin,\"%%ld\",&b);push(b); goto " UP ";\n", x, y, x, y_up);
          fprintf(fo, DOWN ": fscanf(stdin,\"%%ld\",&b);push(b); goto " DOWN ";\n", x, y, x, y_down);
          break;
        case '~':
          fprintf(fo, RIGHT ": c=fgetc(stdin);push(c); goto " RIGHT ";\n", x, y, x_right, y);
          fprintf(fo, LEFT ": c=fgetc(stdin);push(c); goto " LEFT ";\n", x, y, x_left, y);
          fprintf(fo, UP ": c=fgetc(stdin);push(c); goto " UP ";\n", x, y, x, y_up);
          fprintf(fo, DOWN ": c=fgetc(stdin);push(c); goto " DOWN ";\n", x, y, x, y_down);
          break;
        case '"':  /* ha! */
          ECHO("puts(\"Error: compiled Befunge does not support stringmode\\n\");");
          break;
        case '!':
          ECHO("if(pop()) push(0); else push(1);");
          break;
        case '`':
          ECHO("b=pop();a=pop();push(a>b);");
          break;
        case '$':
          ECHO("pop();");
          break;
        case '?':
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, " switch ((rand () / 32) %% 4) \n"
                      " { case 0: goto " RIGHT ";\n"
                      "   case 1: goto " LEFT ";\n"
                      "   case 2: goto " UP ";\n"
                      "   case 3: goto " DOWN "; }\n",
                  x_right, y, x_left, y, x, y_up, x, y_down);
          break;
        case '#':
          fprintf(fo, RIGHT ": goto " RIGHT ";\n", x, y, x_2right, y);
          fprintf(fo, LEFT ": goto " LEFT ";\n", x, y, x_2left, y);
          fprintf(fo, UP ": goto " UP ";\n", x, y, x, y_2up);
          fprintf(fo, DOWN ": goto " DOWN ";\n", x, y, x, y_2down);
          break;
        case 'g':
          ECHO("a=pop();b=pop();push(pg[a*80+b]);");
          break;
        case 'p':
          ECHO("a=pop();b=pop();pg[a*80+b]=pop();");
          break;
        case '@':
          fprintf(fo, ALL, x, y, x, y, x, y, x, y);
          fprintf(fo, "  exit(0);");
          break;
        default:
          ECHO("");
          break;
      }
    }
    printf (".");
  }

  fprintf (fo, "\n}\n\n");

  fprintf (fo, "void push (signed long val)\n{\n  struct stack *s;  s = (struct stack *) malloc (sizeof (struct stack));  s->val = val;\ns->next = head;  head = s;\n}\n\n");

  fprintf (fo, "signed long pop ()\n{\nsigned long v;struct stack *s = head;\nif(s){v=head->val;head=head->next;free(s);return v;\n}else{return 0;}\n}\n\n");

  if (fi) fclose (fi);
  if (fo) fclose (fo);

  printf ("done.\n");
  if (post_optimize)
  {
    int x0 = 0; int y0 = 0; int x1 = 0; int y1 = 0; int i = 0;
    printf ("Optimizing");

    for (i=0; i < (linewidth*pageheight); i++)
    {
      for(y = 0; y < 25; y++)
      {
        for(x = 0; x < 80; x++)
        {
          labelrefs[y * 80 + x] = 0;
        }
      }
      if ((fi = fopen (argv[argc - 1], "r")) != NULL)
      {
        while (!feof(fi))
        {
          fscanf(fi, "%s", in);
          if (!strcmp(in, "goto"))
          {
            fscanf(fi, "%d_%d_%s", &x, &y, s);
            switch (s[0]) { case 'L': d = 0; break;
                          case 'R': d = 1; break;
                          case 'U': d = 2; break;
                          case 'D': d = 3; break; }
            labelrefs[d * 2000 + y * 80 + x]++;
          }
        }
        rewind(fi);
      }
      printf (".");
      if ((fo = fopen ("temp.c", "w")) != NULL)
      {
        while (!feof(fi))
        {
          fgets(in, 255, fi);
          if (sscanf(in, "_%d_%d_%s:  goto _%d_%d_%s;\n", &x0, &y0, t, &x1, &y1, s))
          {
            switch (s[0]) { case 'L': d = 0; break;
                          case 'R': d = 1; break;
                          case 'U': d = 2; break;
                          case 'D': d = 3; break; }
            if (labelrefs[d * 2000 + y1 * 80 + x1])
            {
              fprintf(fo, "_%2.2d_%2.2d_%s ", x0, y0, t);
              fprintf(fo, "goto _%2.2d_%2.2d_%s;\n", x1, y1, s);
            }
          } else
          {
            fprintf(fo, "%s", in);
          }
        }
      }
      if (fi) fclose (fi);
      if (fo) fclose (fo);
      if ((fi = fopen ("temp.c", "r")) != NULL)
      {
        if ((fo = fopen (argv[argc - 1], "w")) != NULL)
        {
          while (!feof(fi))
          {
            fgets(in, 255, fi);
            fprintf(fo, "%s", in);
          }
        }
      }
      printf (".");
    }
    printf ("done.\n");
  }
  return 0;
}

/* END of bef2c.c */
