
/*
 *  catseye.h
 *
 *  not Copyright 1993, Chris Pressey
 */

#ifndef CATSEYE_H
#define CATSEYE_H

#define shrink(s) s[strlen(s)-1]=0
#define space2us(s) {int i=0;while(s[i++]) if(s[i]==' ') s[i]='_';}
#define colon2dash(s) {int i=0;while(s[i++]) if(s[i]==':') s[i]='-';}

#endif

