/*

   bef.c
   Befunge Interpreter - v1.0, Sept 1993, Chris Pressey

   usage :

   bef <befunge-source>

   compilation :

   Amiga : dcc bef.c -o bef
   Unix  : acc bef.c -o bef

 */

/********************************************************* #INCLUDE'S */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <catseye.h>

/********************************************************** #DEFINE'S */

#define LINEWIDTH  80		/* 0 .. 79 */
#define PAGEHEIGHT 25		/* 0 .. 24 */

#define cur pg[y * LINEWIDTH + x]

/********************************************************* STRUCTURES */

struct stack			/* stack structure, for values on stack */
  {
    signed long val;
    struct stack *next;
  }
 *head;				/* head of stack */

/*************************************************** GLOBAL VARIABLES */

char pg[LINEWIDTH * PAGEHEIGHT];	/* befunge 'page' of source */
int x = 0, y = 0;		/* x and y of the PC */
int dx = 1, dy = 0;		/* direction of the PC */
int stringmode = 0;		/* flag : are we in string mode? */

/********************************************************* PROTOTYPES */

void push (signed long val);
signed long pop ();

/******************************************************* MAIN PROGRAM */

main (argc, argv)
     int argc;
     char **argv;
{
  FILE *f;
  srand (time (0));

  if (argc < 2)
    {
      printf ("USAGE : %s <befunge-source>\n", argv[0]);
      exit (0);
    }
  if (f = fopen (argv[1], "r"))			      /*** Input Phase */
    {
      int x = 0, y = 0;
      while (!feof (f))
	{
	  cur = fgetc (f);
	  if (cur == '\n')
	    {
	      cur = ' ';
	      x = 0;
	      y++;
	      if (y >= PAGEHEIGHT)
		break;
	    }
	  else
	    {
	      x++;
	      if (x >= LINEWIDTH)
		{
		  x = 0;
		  y++;
		  if (y >= PAGEHEIGHT)
		    break;
		}
	    }
	}
      fclose (f);
    }
  else
    {
      printf ("Error : couldn't open '%s' for input.\n", argv[1]);
      exit (0);
    }

  while (cur != '@')				/*** Intepreting Phase */
    {
      if (stringmode && (cur != '"'))
	push (cur);
      else if (isdigit (cur))
	push (cur - '0');
      else
	switch (cur)
	   {
	   case '>':		/* PC Right */
	     dx = 1;
	     dy = 0;
	     break;
	   case '<':		/* PC Left */
	     dx = -1;
	     dy = 0;
	     break;
	   case '^':		/* PC Up */
	     dx = 0;
	     dy = -1;
	     break;
	   case 'v':		/* PC Down */
	     dx = 0;
	     dy = 1;
	     break;
	   case '|':		/* Vertical 'If' */
	     dx = 0;
	     if (pop ())
	       dy = -1;
	     else
	       dy = 1;
	     break;
	   case '_':		/* Horizontal 'If' */
	     dy = 0;
	     if (pop ())
	       dx = -1;
	     else
	       dx = 1;
	     break;
	   case '+':		/* Add */
	     push (pop () + pop ());
	     break;
	   case '-':		/* Subtract */
	     push (-1 * (pop () - pop ()));
	     break;
	   case '*':		/* Multiply */
	     push (pop () * pop ());
	     break;
	   case '/':		/* Integer Divide */
	     {
	       int a = pop ();
	       int b = pop ();
	       push (b / a);
	     }
	     break;
	   case '%':		/* Modulo */
	     {
	       int a = pop ();
	       int b = pop ();
	       push (b % a);
	     }
	     break;
	   case '\\':		/* Swap */
	     {
	       int a = pop ();
	       int b = pop ();
	       push (a);
	       push (b);
	     }
	     break;
	   case '.':		/* Pop Out Integer */
	     printf ("%ld ", pop ());
	     fflush (stdout);
	     break;
	   case ',':		/* Pop Out ASCII */
	     printf ("%c", pop ());
	     fflush (stdout);
	     break;
	   case '"':		/* Toggle String Mode */
	     stringmode = !stringmode;
	     break;
	   case ':':		/* Duplicate */
	     {
	       int a = pop ();
	       push (a);
	       push (a);
	     }
	     break;
	   case '!':		/* Negate */
	     push (!pop ());
	     break;
	   case '#':		/* Bridge */
	     x += dx;
	     y += dy;
	     break;
	   case '$':		/* Pop and Discard */
	     pop ();
	     break;
	   case '?':		/* Random Redirect */
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
	   case '&':		/* Input Integer */
	     {
	       signed long b;
	       fscanf (stdin, "%ld", &b);
	       push (b);
	     }
	     break;
	   case '~':		/* Input ASCII */
	     {
	       char c = fgetc (stdin);
	       push (c);
	     }
	     break;
	   case 'g':		/* Get Value */
	     {
	       int y = pop (), x = pop ();
	       push (cur);
	     }
	     break;
	   case 'p':		/* Put Value */
	     {
	       int y = pop (), x = pop ();
	       cur = pop ();
	     }
	     break;
	   default:
	     break;
	   }
      x += dx;
      y += dy;
    }
  exit (0);
}

/*
 * pushes a value onto the stack.
 */
void
  push (signed long val)
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
signed long
  pop ()
{
  signed long v;
  struct stack *s = head;
  if (s)
    {
      v = head->val;
      head = head->next;
      free (s);
      return v;
    }
  else
    {
      return 0;
    }
}
