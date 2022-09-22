/* floattrig.h: trigonometry functions, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATTRIG_H
# define FLOATTRIG_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

void _arctanlt1(floatnum x, int digits);
void _arctan(floatnum x, int digits);
void _arccosxplus1lt0_5(floatnum x, int digits);
void _arcsinlt0_5(floatnum x, int digits);
void _arccos(floatnum x, int digits);
void _arccosxplus1(floatnum x, int digits);
void _arcsin(floatnum x, int digits);
char _cosminus1ltPiDiv4(floatnum x, int digits);
void _sinltPiDiv4(floatnum x, int digits);
void _tanltPiDiv4(floatnum x, int digits);
void _cos(floatnum x, int digits);
void _sin(floatnum x, int digits);
char _tan(floatnum x, int digits);
char _cosminus1(floatnum x, int digits);
char _trigreduce(floatnum x, int digits);
void _sinpix(floatnum x, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATTRIG_H */
