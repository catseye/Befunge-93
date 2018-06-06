#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <catseye.h>

#define cur pg[y].l[x]

struct stack			/* stack structure, for values on stack */
{
  signed long val;
  struct stack *next;
} *head;			/* head of stack */

struct line
{
  char l[80];
};

struct line pg[25];

int x=0, y=0;
int dx=1, dy=0;
int stringmode = 0;

void push(signed long val);
signed long pop();

main(argc, argv)
  int argc;
  char **argv;
{
  FILE *f;
  srand(time(0));
  
  if (argc<2)
  {
    printf("USAGE : %s <befunge-source>\n", argv[0]);
    exit(0);
  }
  if (f=fopen(argv[1], "r"))
  {
    int i=0;
    while(!feof(f))
    {
      fgets(pg[i].l, 79, f);
      shrink(pg[i].l);
      while(strlen(pg[i].l)<79)
        strcat(pg[i].l, " ");
      i++;
      if (i>24)
        break;
    }
    fclose(f);
  } else
  {
    printf("Error : couldn't open '%s' for input.\n", argv[1]);
    exit(0);
  }

  while(cur!='@')
  {
    if(stringmode&&(cur!='"'))
      push(cur); else
    if(isdigit(cur))
      push(cur-'0'); else
    switch(cur)
    {
      case '>' : dx = 1; dy = 0; break;
      case '<' : dx = -1; dy = 0; break;
      case '^' : dx = 0; dy = -1; break;
      case 'v' : dx = 0; dy = 1; break;
      case '|' : dx = 0; if(pop()) dy = -1; else dy = 1; break;
      case '_' : dy = 0; if(pop()) dx = -1; else dx = 1; break;
      case '+' : push(pop()+pop()); break;
      case '-' : push(-1*(pop()-pop())); break;
      case '*' : push(pop()*pop()); break;
      case '/' : { int a = pop(); int b = pop(); push(b / a); } break;
      case '%' : { int a = pop(); int b = pop(); push(b % a); } break;
      case '\\' : { int a = pop(); int b = pop(); push(a); push(b); } break;
      case '.' : printf("%ld ", pop()); fflush(stdout); break;
      case ',' : printf("%c", pop()); fflush(stdout); break;
      case '"' : stringmode = !stringmode; break;
      case ':' : { int a = pop(); push(a); push(a); } break;
      case '!' : push(!pop()); break;
      case '#' : x += dx; y += dy; break;
      case '$' : pop(); break;
      case '?' : switch((rand()/32)%4)
                 {
                   case 0 : dx = 1; dy = 0; break;
                   case 1 : dx = -1; dy = 0; break;
                   case 2 : dx = 0; dy = -1; break;
                   case 3 : dx = 0; dy = 1; break;
                 } break;
      case '&' : { signed long b; fscanf(stdin, "%ld", &b); push(b); } break;
      case '~' : { char c=fgetc(stdin); push(c); } break;
      case 'g' : { int y=pop(), x=pop(); push(cur); } break;
      case 'p' : { int y=pop(), x=pop(); cur=pop(); } break;
      default  : break;
    }
    x += dx;
    y += dy;
  }
  exit(0);
}

/*
 * pushes a value onto the stack.
 */
void push(signed long val)
{
  struct stack *s;
  s = (struct stack *) malloc(sizeof(struct stack));
  s->val = val;
  s->next = head;
  head = s;
}

/*
 * pops a value off the stack. returns 0 in case of underflow.
 */
signed long pop()
{
  signed long v;
  struct stack *s = head;
  if (s)
  {
    v = head->val;
    head = head->next;
    free(s);
    return v;
  } else
  {
    return 0;
  }
}
