/* floatexp.h: exponential function and friends, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATEXP_H
# define FLOATEXP_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

char _coshminus1lt1(floatnum x, int digits);
void _sinhlt1(floatnum x, int digits);
void _expminus1lt1(floatnum x, int digits);
void _expltln10(floatnum x, int digits);
char _exp(floatnum x, int digits);
char _expminus1(floatnum x, int digits);
char _coshminus1(floatnum x, int digits);
void _tanhlt0_5(floatnum x, int digits);
char _tanhminus1gt0(floatnum x, int digits);
char _sinh(floatnum x, int digits);
void _tanhgt0_5(floatnum x, int digits);
char _power10(floatnum exponent, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATEXP_H */
