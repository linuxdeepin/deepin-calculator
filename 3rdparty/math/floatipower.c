/* floatpower.c: power operation, based on floatnum. */

// Copyright (C) 2007, 2008 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "floatipower.h"
#include "floatconst.h"
#include "floatcommon.h"
#include "floatlong.h"

/* for radix conversion, we need to get a result,
   even though it might slightly overflow or underflow.
   That is why we keep the exponent separate.
   For limited exponents (< 1024) the relative
   error for a 100 digit calculation is < 1e-99
   x != 0 */
char
_raiseposi(
  floatnum x,
  int* expx,
  unsigned exponent,
  int digits)
{
  int exppwr, extra;
  floatstruct pwr;

  float_create(&pwr);
  float_move(&pwr, x);
  exppwr = float_getexponent(&pwr);
  float_setexponent(&pwr, 0);
  if ((exponent & 1) != 0)
  {
    float_copy(x, &pwr, digits+1);
    *expx = exppwr;
  }
  else
  {
    float_copy(x, &c1, EXACT);
    *expx = 0;
  }
  extra = _findfirstbit(exponent)/3+1;
  while((exponent >>= 1) != 0)
  {
    float_mul(&pwr, &pwr, &pwr, digits+extra);
    if (!_checkadd(&exppwr, exppwr))
      break;
    exppwr += float_getexponent(&pwr);
    float_setexponent(&pwr, 0);
    if((exponent & 1) != 0)
    {
      float_mul(x, x, &pwr, digits+extra);
      *expx += exppwr + float_getexponent(x);
      float_setexponent(x, 0);
    }
  }
  float_free(&pwr);
  return exponent == 0;
}

char
_raiseposi_(
  floatnum x,
  unsigned exponent,
  int digits)
{
  int exp;
  char result;

  result = _raiseposi(x, &exp, exponent, digits);
  float_setexponent(x, exp);
  return result;
}

/* raises a non-zero x to the exponent-th power */
char
_raisei(
  floatnum x,
  int exponent,
  int digits)
{
  int expx;
  signed char sgn;
  char negexp;

  switch (exponent)
  {
  case 0:
    float_copy(x, &c1, EXACT); /* fall through */
  case 1:
    return 1;
  }
  if (float_getlength(x) == 1 && float_getdigit(x, 0) == 1)
  {
    /* power of ten */
    if (!_checkmul(&exponent, float_getexponent(x))
        || exponent < EXPMIN || exponent > EXPMAX)
      return 0;
    sgn = float_getsign(x) < 0? -(exponent & 1) : 1;
    float_setexponent(x, exponent);
    float_setsign(x, sgn);
    return 1;
  }
  negexp = exponent < 0;
  if (negexp)
    exponent = -exponent;
  if (!_raiseposi(x, &expx, exponent, digits)
      || expx < EXPMIN || expx > EXPMAX)
    return 0;
  float_setexponent(x, expx);
  return negexp? float_reciprocal(x, digits) : 1;
}
