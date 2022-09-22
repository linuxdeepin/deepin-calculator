/* floaterf.h: normal distribution integrals erf and the like */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATERF_H
#define FLOATERF_H

#include "floatseries.h"

#ifdef __cplusplus
extern "C" {
#endif

#define erfnear0 erfseries
#define erfcbigx erfcasymptotic

char _erf(floatnum x, int digits);
char _erfc(floatnum x, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATERF_H */
