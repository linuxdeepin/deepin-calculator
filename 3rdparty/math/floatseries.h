/* floatseries.h: header file for basic series, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATSERIES_H
# define FLOATSERIES_H

#include "floatnum.h"

#define artanhnear0(x, digits) arctanseries(x, digits, 0)
#define arctannear0(x, digits) arctanseries(x, digits, 1)
#define coshminus1near0(x, digits) cosminus1series(x, digits, 0)
#define cosminus1near0(x, digits) cosminus1series(x, digits, 1)

#ifdef __cplusplus
extern "C" {
#endif

void arctanseries(floatnum x, int digits, char alternating);
char cosminus1series(floatnum x, int digits, char alternating);

#ifdef __cplusplus
}
#endif

#endif /* FLOATSERIES_H */
