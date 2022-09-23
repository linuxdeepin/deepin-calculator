/* floatincgamma.h: incomplete gamma function */

// Copyright (C) 2009 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATINCGAMMA_H
# define FLOATINCGAMMA_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

void testincgamma(floatnum x, cfloatnum a, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATINCGAMMA_H */
