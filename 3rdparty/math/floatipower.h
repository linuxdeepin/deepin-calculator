/* floatpower.h: power operation, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATIPOWER_H
# define FLOATIPOWER_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

char _raiseposi_(floatnum x, unsigned exponent, int digits);
char _raiseposi(floatnum x, int* expx, unsigned exponent, int digits);
char _raisei(floatnum x, int exponent, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATIPOWER_H */
