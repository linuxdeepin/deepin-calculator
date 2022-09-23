/* floatgamma.h: Gamma function, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATGAMMA_H
# define FLOATGAMMA_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

char _lngamma(floatnum x, int digits);
char _gamma(floatnum x, int digits);
char _gammaint(floatnum integer, int digits);
char _gamma0_5(floatnum x, int digits);
char _pochhammer(floatnum x, cfloatnum n, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATGAMMA_H */
