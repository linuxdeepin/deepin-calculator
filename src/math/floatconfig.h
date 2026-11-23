// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef _FLOATCONFIG_H
#define _FLOATCONFIG_H

/* The decimal precision of the math library.
   This controls how many decimal digits are computed/stored.
   The old floatnum library used 78; the new mpfr-based implementation
   uses this same value for formatting and rounding decisions. */
#define DECPRECISION 78

/* The internal math precision (binary digits).
   Old: 2*DECPRECISION + 3 = 159 decimal digits.
   New: mapped to mpfr precision ~528 bits. */
#define MATHPRECISION (2*DECPRECISION + 3)

/* Maximum number of digits for internal storage (no longer used by mpfr,
   but kept for compatibility). */
#define MAXDIGITS 250

/* The integer domain of logical functions. */
#define LOGICRANGE 256

#endif /* _FLOATCONFIG_H */
