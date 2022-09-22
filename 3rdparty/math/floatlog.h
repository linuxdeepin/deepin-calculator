/* floatlog.h: logarithms and friends, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATLOG_H
# define FLOATLOG_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

void _lnxplus1near0(floatnum x, int digits);
void _lnreduce(floatnum x, floatnum lnguess, int digits);
void _lnxplus1lt1(floatnum x, int digits);
void _ln(floatnum x, int digits);
void _lnxplus1(floatnum x, int digits);
void _artanh1minusx(floatnum x, int digits);
void _artanhlt0_5(floatnum x, int digits);
void _artanh(floatnum x, int digits);
void _arsinh(floatnum x, int digits);
void _arcoshxplus1(floatnum x, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATLOG_H */
