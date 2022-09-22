/* floatseries.c: basic series, based on floatnum. */

// Copyright (C) 2007 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "floatseries.h"
#include "floatconst.h"
#include "floatcommon.h"
#include "floatexp.h"

/* Though all these serieses can be used with arguments |x| < 1 or
   even more, the computation time increases rapidly with x.
   Tests show, that for 100 digit results, it is best to limit x
   to |x| < 0.01..0.02, and use reduction formulas for greater x */

/* the Taylor series of arctan/arctanh x at x == 0. For small
   |x| < 0.01 this series converges very fast, yielding 4 or
   more digits of the result with every summand. The working
   precision is adjusted, so that the relative error for
   100-digit arguments is around 5.0e-100. This means, the error
   is 1 in the 100-th place (or less) */
void
arctanseries(
  floatnum x,
  int digits,
  char alternating)
{
  int expx;
  int expsqrx;
  int pwrsz;
  int addsz;
  int i;
  floatstruct xsqr;
  floatstruct pwr;
  floatstruct smd;
  floatstruct sum;

  /* upper limit of log(x) and log(result) */
  expx = float_getexponent(x)+1;

  /* the summands of the series from the second on are
     bounded by x^(2*i-1)/3. So the summation yields a
     result bounded by (x^3/(1-x*x))/3.
     For x < sqrt(1/3) approx.= 0.5, this is less than 0.5*x^3.
     We need to sum up only, if the first <digits> places
     of the result (roughly x) are touched. Ignoring the effect of
     a possile carry, this is only the case, if
     x*x >= 2*10^(-digits) > 10^(-digits)
     Example: for x = 9e-51, a 100-digits result covers
     the decimal places from 1e-51 to 1e-150. x^3/3
     is roughly 3e-151, and so is the sum of the series.
     So we can ignore the sum, but we couldn't for x = 9e-50 */
  if (float_iszero(x) || 2*expx < -digits)
    /* for very tiny arguments arctan/arctanh x is approx.== x */
    return;
  float_create(&xsqr);
  float_create(&pwr);
  float_create(&smd);
  float_create(&sum);

  /* we adapt the working precision to the decreasing
     summands, saving time when multiplying. Unfortunately,
     there is no error bound given for the operations of
     bc_num. Tests show, that the last digit in an incomplete
     multiplication is usually not correct up to 5 ULP's. */
  pwrsz = digits + 2*expx + 1;
  /* the precision of the addition must not decrease, of course */
  addsz = pwrsz;
  i = 3;
  float_mul(&xsqr, x, x, pwrsz);
  float_setsign(&xsqr, alternating? -1 : 1);
  expsqrx = float_getexponent(&xsqr);
  float_copy(&pwr, x, pwrsz);
  float_setzero(&sum);

  for(; pwrsz > 0; )
  {
    /* x^i */
    float_mul(&pwr, &pwr, &xsqr, pwrsz+1);
    /* x^i/i */
    float_divi(&smd, &pwr, i, pwrsz);
    /* The addition virtually does not introduce errors */
    float_add(&sum, &sum, &smd, addsz);
    /* reduce the working precision according to the decreasing powers */
    pwrsz = digits - expx + float_getexponent(&smd) + expsqrx + 3;
    i += 2;
  }
  /* add the first summand */
  float_add(x, x, &sum, digits+1);

  float_free(&xsqr);
  float_free(&pwr);
  float_free(&smd);
  float_free(&sum);
}

/* series expansion of cos/cosh - 1 used for small x,
   |x| <= 0.01.
   The function returns 0, if an underflow occurs.
   The relative error seems to be less than 5e-100 for
   a 100-digit calculation with |x| < 0.01 */
char
cosminus1series(
  floatnum x,
  int digits,
  char alternating)
{
  floatstruct sum, smd;
  int expsqrx, pwrsz, addsz, i;

  expsqrx = 2 * float_getexponent(x);
  float_setexponent(x, 0);
  float_mul(x, x, x, digits+1);
  float_mul(x, x, &c1Div2, digits+1);
  float_setsign(x, alternating? -1 : 1);
  expsqrx += float_getexponent(x);
  if (float_iszero(x) || expsqrx < EXPMIN)
  {
    /* underflow */
    float_setzero(x);
    return expsqrx == 0;
  }
  float_setexponent(x, expsqrx);
  pwrsz = digits + expsqrx + 2;
  if (pwrsz <= 0)
    /* for very small x, cos/cosh(x) - 1 = (-/+)0.5*x*x */
    return 1;
  addsz = pwrsz;
  float_create(&sum);
  float_create(&smd);
  float_copy(&smd, x, pwrsz);
  float_setzero(&sum);
  i = 2;
  while (pwrsz > 0)
  {
    float_mul(&smd, &smd, x, pwrsz+1);
    float_divi(&smd, &smd, i*(2*i-1), pwrsz);
    float_add(&sum, &sum, &smd, addsz);
    ++i;
    pwrsz = digits + float_getexponent(&smd);
  }
  float_add(x, x, &sum, digits+1);
  float_free(&sum);
  float_free(&smd);
  return 1;
}
