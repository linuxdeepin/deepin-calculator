/* floatconst.h: constants for higher math functions */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOATCONST_H
#define FLOATCONST_H

#include "floatnum.h"

#define MAXBERNOULLIIDX 68
#define MAXERFCIDX 80

#ifdef __cplusplus
extern "C" {
#endif

extern floatstruct c1;
extern floatstruct c2;
extern floatstruct c3;
extern floatstruct c12;
extern floatstruct cExp;
extern floatstruct cMinus1;
extern floatstruct cMinus20;
extern floatstruct c1Div2;
extern floatstruct cLn2;
extern floatstruct cLn3;
extern floatstruct cLn7;
extern floatstruct cLn10;
extern floatstruct cPhi;
extern floatstruct cPi;
extern floatstruct cPiDiv2;
extern floatstruct cPiDiv4;
extern floatstruct c2Pi;
extern floatstruct c1DivPi;
extern floatstruct cSqrtPi;
extern floatstruct c1DivSqrtPi;
extern floatstruct cLnSqrt2PiMinusHalf;
extern floatstruct c2DivSqrtPi;
extern floatstruct cMinus0_4;
extern floatstruct cBernoulliNum[68];
extern floatstruct cBernoulliDen[68];
extern floatstruct cUnsignedBound;

extern int erfcdigits;
extern floatstruct erfccoeff[MAXERFCIDX];
extern floatstruct erfcalpha;
extern floatstruct erfcalphasqr;
extern floatstruct erfct2;
extern floatstruct erfct3;

void floatmath_init();
void floatmath_exit();

#ifdef __cplusplus
}
#endif

#endif /* FLOATCONST_H */
