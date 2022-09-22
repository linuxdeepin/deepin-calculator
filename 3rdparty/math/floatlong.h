/* floatlong.h: portable double size integer arithmetic */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATLONG_H
# define FLOATLONG_H

#include "floatconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BITS_IN_UNSIGNED (sizeof(unsigned)*8)

/* one unsigned extra, so that _bitsubstr() does not access parts
   outside of t_uarray */
#define UARRAYLG ((8305*(MATHPRECISION+5) + 1)/20000/sizeof(unsigned) + 2)

typedef unsigned t_uarray[UARRAYLG];

typedef struct{
  int length;
  t_uarray value;
} t_longint;

int _findfirstbit(unsigned value);
char _longadd(unsigned* s1, unsigned* s2);
char _longmul(unsigned* f1, unsigned* f2);
char _checkadd(int* s1, int s2);
char _checkmul(int* f1, int f2);
unsigned _longshr(unsigned low, unsigned high, char shift);
unsigned _longshl(unsigned low, unsigned high, char shift);
unsigned _longarrayadd(unsigned* uarray, int lg, unsigned incr);
unsigned _longarraymul(unsigned* uarray, int lg, unsigned factor);
void _orsubstr(unsigned* uarray, int bitofs, unsigned value);
unsigned _bitsubstr(unsigned* uarray, int ofs);
unsigned _bitlength(t_longint* l);
unsigned _lastnonzerobit(t_longint* l);
char _longintsetsize(t_longint* l, unsigned bitlength);
unsigned _longintadd(t_longint* l, unsigned summand);
unsigned _longintmul(t_longint* l, unsigned factor);

#ifdef __cplusplus
}
#endif

#endif /* FLOATLONG_H */
