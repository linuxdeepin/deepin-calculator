/* floatpower.h: power operation, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATPOWER_H
# define FLOATPOWER_H

#include "floatnum.h"

#ifdef __cplusplus
extern "C" {
#endif

char _raise(floatnum x, cfloatnum exponent, int digits);

#ifdef __cplusplus
}
#endif

#endif /* FLOATPOWER_H */
