/* floatconvert.c: radix conversion, based on floatnum. */

// Copyright (C) 2007 - 2009 Wolf Lammen.
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "floatconvert.h"
#include "floatcommon.h"
#include "floatconst.h"
#include "floatipower.h"
#include <stdlib.h>

typedef struct {
    t_number_desc n;
    t_longint l;
    floatnum f;
    int bitlength;
    int trailing0;
    int lgbase;
} t_ext_number;
typedef t_ext_number *p_ext_number;

/************************   conversion to/from longint   *******************/

static unsigned
_digitblock(
    floatnum f,
    int ofs,
    int count)
{
    unsigned result;

    result = 0;
    for (; --count >= 0;)
        result = 10 * result + float_getdigit(f, ofs++);
    return result;
}

Error
_floatnum2longint(
    t_longint *longint,
    floatnum f)
{
    int digits;
    int i;
    unsigned factor;

    longint->length = 0;
    digits = float_getexponent(f) + 1;
    i = digits % 9;
    _longintadd(longint, _digitblock(f, 0, i));
    factor = 1000000000;
    while (i < digits) {
        if (_longintmul(longint, factor)
                || _longintadd(longint, _digitblock(f, i, 9)))
            return IOConversionOverflow;
        i += 9;
    }
    /* extra element for floatlong operations */
    *(longint->value + longint->length) = 0;
    return Success;
}

static void
_setunsigned(
    floatnum f,
    unsigned value)
{
    float_setinteger(f, value);
    if ((int)value < 0)
        float_add(f, f, &cUnsignedBound, EXACT);
}

void
_longint2floatnum(
    floatnum f,
    t_longint *longint)
{
    floatstruct tmp;
    int idx;

    float_setzero(f);
    if (longint->length == 0)
        return;
    float_create(&tmp);
    idx = longint->length - 1;
    for (; idx >= 0; --idx) {
        _setunsigned(&tmp, longint->value[idx]);
        float_mul(f, f, &cUnsignedBound, EXACT);
        float_add(f, f, &tmp, EXACT);
    }
    float_free(&tmp);
}

/**************************   io routines   **************************/

static int
_max(
    int a,
    int b)
{
    return a > b ? a : b;
}

static char
_validmode(
    char mode)
{
    return mode >= IO_MODE_SCIENTIFIC && mode <= IO_MODE_COMPLEMENT;
}

static int
lgbase(
    signed char base)
{
    switch (base) {
    case 2:
        return 1;
    case 8:
        return 3;
    case 16:
        return 4;
    }
    return 0;
}

static char
_getfnintdigit(
    int ofs,
    p_seq_desc n)
{
    if (ofs <= n->digits)
        return float_getdigit((floatnum)(n->param), ofs);
    return 0;
}

static char
_getfnfracdigit(
    int ofs,
    p_seq_desc n)
{
    floatnum x;
    int exp;

    x = (floatnum)(n->param);
    exp = float_getexponent(x);
    if (ofs >= 0)
        return float_getdigit(x, ofs + exp + 1);
    return 0;
}

static void
_setfndesc(
    p_number_desc n,
    floatnum x)
{
    int digits;

    n->intpart.seq.base = 10;
    digits = _max(float_getexponent(x) + 1, 0);
    n->intpart.seq.digits = digits;
    n->intpart.seq.trailing0 = _max(digits - float_getlength(x), 0);
    n->intpart.seq.param = x;
    n->intpart.getdigit = _getfnintdigit;
    n->fracpart.seq.base = 10;
    n->fracpart.seq.leadingSignDigits = _max(-float_getexponent(x) - 1, 0);
    n->fracpart.seq.digits = float_getlength(x) - digits
                             + n->fracpart.seq.leadingSignDigits;
    n->fracpart.seq.param = x;
    n->fracpart.getdigit = _getfnfracdigit;
}

static Error
_pack2longint(
    t_longint *l,
    p_ext_seq_desc n)
{
    int bitofs;
    int ofs;
    int logbase;

    logbase = lgbase(n->seq.base);
    ofs = n->seq.leadingSignDigits;
    if (_significantdigits(&n->seq) == 0)
        /* can be true in complement case: 0xFF00 */
        --ofs;
    bitofs = (n->seq.digits - ofs) * logbase;
    if (!_longintsetsize(l, bitofs))
        return IOBufferOverflow;
    for (; bitofs > 0;) {
        bitofs -= logbase;
        _orsubstr(l->value, bitofs, n->getdigit(ofs++, &n->seq));
    }
    return Success;
}

static char
_getlongintdigit(
    int ofs,
    p_seq_desc n)
{
    if (ofs < 0 || ofs >= n->digits)
        return 0;
    return _bitsubstr(((t_longint *)(n->param))->value,
                      (n->digits - ofs - 1) * lgbase(n->base))
           & (n->base - 1);
}

static char
_getlongintofsdigit(
    int ofs,
    p_seq_desc n)
{
    p_number_desc nmb;
    int digits;

    nmb = (p_number_desc)(n->param);
    digits = n->digits;
    if (ofs < 0 || ofs >= digits)
        return 0;
    digits += nmb->fracpart.seq.digits;
    return _bitsubstr(((t_longint *)(nmb->fracpart.seq.param))->value,
                      (digits - ofs - 1) * lgbase(n->base))
           & (n->base - 1);
}

static void
_setlongintdesc(
    p_ext_seq_desc n,
    t_longint *l,
    signed char base)
{
    int lg;

    n->seq.base = base;
    lg = lgbase(base);
    n->seq.digits = (_bitlength(l) + lg - 1) / lg;
    n->seq.leadingSignDigits = 0;
    n->seq.trailing0 = _lastnonzerobit(l) / lg;
    n->seq.param = l;
    n->getdigit = _getlongintdigit;
}

static Error
_packdec2int(
    floatnum x,
    p_ext_seq_desc n)
{
    int ofs;
    int exp;
    int bufsz;
    int i;
    char buf[DECPRECISION];

    float_setnan(x);
    ofs = n->seq.leadingSignDigits;
    exp = n->seq.trailing0;
    bufsz = n->seq.digits - ofs - exp;
    if (bufsz > DECPRECISION)
        return IOBufferOverflow;
    if (bufsz == 0)
        float_setzero(x);
    else
        for (i = -1; ++i < bufsz;)
            buf[i] = n->getdigit(ofs++, &n->seq) + '0';
    float_setsignificand(x, NULL, buf, bufsz);
    float_setexponent(x, exp + bufsz - 1);
    return Success;
}

static Error
_packbin2int(
    floatnum x,
    p_ext_seq_desc n)
{
    t_longint l;
    Error result;

    float_setnan(x);
    if ((result = _pack2longint(&l, n)) != Success)
        return result;
    _longint2floatnum(x, &l);
    return Success;
}

static char
_signextendbin(
  t_longint* longint,
        int bitlength)
{
  unsigned mask;
  signed char sign;
  int maxidx =  ((bitlength-1) / BITS_IN_UNSIGNED);
  int signbit = (bitlength - 1 - maxidx * BITS_IN_UNSIGNED);

  sign = (longint->value[maxidx] & (1 << signbit)) != 0? -1:1;
  mask = (~0) << signbit;
  if (sign < 0)
    longint->value[maxidx] |= mask;
  else
    longint->value[maxidx] &= ~mask;
  return sign;
}

static void
_negbin(
  t_longint* longint,
        int bitlength)
{
  int idx = -1;
  const int maxidx = ((bitlength-1) / BITS_IN_UNSIGNED);
  while (++idx <= maxidx && longint->value[idx] == 0);
  if (idx <= maxidx)
    longint->value[idx] = - longint->value[idx];
  while (++idx <= maxidx)
    longint->value[idx] = ~longint->value[idx];
}

static Error
_packsignedbin2int(
    floatnum x,
    p_ext_seq_desc n,
        int bitlength)
{
    t_longint l;
    Error result;

    for (int i = 0;i<5;i++) {
        l.value[i] = 0;
    }

    float_setnan(x);
    if ((result = _pack2longint(&l, n)) != Success)
        return result;
    int idx;
    signed char sign;

    sign = _signextendbin(&l,bitlength);
    if (sign < 0)
      _negbin(&l,bitlength);
    idx = (bitlength-1) / BITS_IN_UNSIGNED;
    while (idx >= 0 && l.value[idx] == 0)
      --idx;
    if (idx < 0)
      l.length = 0;
    else
      l.length = idx + 1;

    _longint2floatnum(x, &l);
    float_setsign(x, sign);
    return Success;
}

static Error
_pack2int(
    floatnum x,
    p_ext_seq_desc n,
    int bitlength,
    signed char b)
{
    switch (n->seq.base) {
    case IO_BASE_NAN:
        float_setnan(x);
        break;
    case IO_BASE_ZERO:
        float_setzero(x);
        break;
    case 10:
        return _packdec2int(x, n);
    default:
        if(b > 0)
        {
            return _packsignedbin2int(x, n, bitlength);
        }
        return _packbin2int(x, n);
    }
    return Success;
}

static Error
_pack2frac(
    floatnum x,
    p_ext_seq_desc n,
    int digits,
    int bitlength,
    signed char b)
{
    floatstruct tmp;
    int exp;
    Error result;

    n->seq.digits -= n->seq.trailing0;
    n->seq.trailing0 = 0;
    switch (n->seq.base) {
    case IO_BASE_NAN:
        float_setnan(x);
        break;
    case IO_BASE_ZERO:
        float_setzero(x);
        break;
    default:
        if ((result = _pack2int(x, n, bitlength, b)) != Success)
            return result;
        float_create(&tmp);
        float_setinteger(&tmp, n->seq.base);
        _raiseposi(&tmp, &exp, n->seq.digits, digits + 2);
        float_div(x, x, &tmp, digits + 2);
        float_setexponent(x, float_getexponent(x) - exp);
        float_free(&tmp);
    }
    n->seq.digits += n->seq.trailing0;
    return Success;
}

Error
pack2floatnum(
    floatnum x,
    p_number_desc n,
    int bitlength,
    signed char b)
{
    floatstruct tmp;
    int digits;
    int saveerr;
    int saverange;
    Error result;
    signed char base;

    if ((result = _pack2int(x, &n->intpart, bitlength, b)) != Success)
        return result;
    if (float_isnan(x))
        return Success;
    saveerr = float_geterror();
    saverange = float_setrange(MAXEXP);
    float_create(&tmp);
    float_move(&tmp, x);
    float_setzero(x);
    digits = DECPRECISION - float_getexponent(&tmp);
    if (digits <= 0
            || (result = _pack2frac(x, &n->fracpart, digits, bitlength, b)) == Success)
        float_add(x, x, &tmp, DECPRECISION);
    if (result != Success)
        return result;
    if ((!float_getlength(x)) == 0) { /* no zero, no NaN? */
        base = n->prefix.base;
        float_setinteger(&tmp, base);
        if (n->exp >= 0) {
            _raiseposi_(&tmp, n->exp, DECPRECISION + 2);
            float_mul(x, x, &tmp, DECPRECISION + 2);
        } else {
            _raiseposi_(&tmp, -n->exp, DECPRECISION + 2);
            float_div(x, x, &tmp, DECPRECISION + 2);
        }
    }
    float_free(&tmp);
    float_setsign(x, n->prefix.sign == IO_SIGN_COMPLEMENT ? -1 : n->prefix.sign);
    float_geterror();
    float_seterror(saveerr);
    float_setrange(saverange);
    if (!float_isvalidexp(float_getexponent(x)))
        float_setnan(x);
    return float_isnan(x) ? IOExpOverflow : Success;
}

static Error
_outscidec(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    float_checkedround(x, scale + 1);
    n->exp = float_getexponent(x);
    float_setexponent(x, 0);
    _setfndesc(n, x);
    return desc2str(tokens, n, scale);
}

static int
_checkbounds(
    floatnum x,
    int digits,
    signed char base)
{
    if (float_getexponent(x) < 0) {
        float_muli(x, x, base, digits);
        return -1;
    } else if (float_asinteger(x) >= base) {
        float_divi(x, x, base, digits);
        return 1;
    }
    return 0;
}

static void
_scale2int(
    floatnum x,
    int scale,
    signed char base)
{
    floatstruct pwr;
    int pwrexp;

    (void)scale;

    if (scale != 0) {
        float_create(&pwr);
        float_setinteger(&pwr, base);
        _raiseposi(&pwr, &pwrexp, scale, DECPRECISION + 4);
        float_mul(x, x, &pwr, DECPRECISION + 4);
        float_addexp(x, pwrexp);
        float_free(&pwr);
    }
    float_roundtoint(x, TONEAREST);
}

static Error
_fixp2longint(
    p_number_desc n,
    t_longint *l,
    floatnum x,
    int scale)
{
    Error result;
    _scale2int(x, scale, n->prefix.base);
    result = _floatnum2longint(l, x);
    if (result != Success)
        return result;
    _setlongintdesc(&n->fracpart, l, n->prefix.base);
    return Success;
}

static int
_extractexp(
    floatnum x,
    int scale,
    signed char base)
{
    floatstruct pwr;
    floatstruct fbase;
    int decprec;
    int pwrexp;
    int exp;
    int logbase;

    (void)scale;

    logbase = lgbase(base);
    decprec = DECPRECISION + 3;
    exp = (int)(aprxlog10fn(x) * 3.321928095f);
    if (float_getexponent(x) < 0)
        exp -= 3;
    exp /= logbase;
    if (exp != 0) {
        float_create(&fbase);
        float_setinteger(&fbase, base);
        float_create(&pwr);
        float_copy(&pwr, &fbase, EXACT);
        _raiseposi(&pwr, &pwrexp, exp < 0 ? -exp : exp, decprec);
        if (float_getexponent(x) < 0) {
            float_addexp(x, pwrexp);
            float_mul(x, x, &pwr, decprec);
        } else {
            float_addexp(x, -pwrexp);
            float_div(x, x, &pwr, decprec);
        }
        float_free(&pwr);
        float_free(&fbase);
    }
    exp += _checkbounds(x, decprec, base);
    return exp;
}

static void
_setscale(
    p_number_desc n,
    t_longint *l,
    int scale)
{
    (void)l;

    n->intpart.seq.leadingSignDigits = 0;
    n->intpart.seq.trailing0 = n->fracpart.seq.trailing0 - scale;
    if (n->intpart.seq.trailing0 < 0)
        n->intpart.seq.trailing0 = 0;
    n->intpart.seq.base = n->fracpart.seq.base;
    n->intpart.seq.digits = n->fracpart.seq.digits - scale;
    n->intpart.getdigit = _getlongintofsdigit;
    n->intpart.seq.param = n;

    n->fracpart.seq.digits = scale;
    if (n->fracpart.seq.trailing0 >= scale) {
        n->fracpart.seq.base = IO_BASE_ZERO;
        n->fracpart.seq.trailing0 = scale;
    }
}

static Error
_outscihex(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    t_longint l;
    Error result;

    n->exp = _extractexp(x, scale, n->prefix.base);
    result = _fixp2longint(n, &l, x, scale);
    if (result != Success)
        return result;
    /* rounding in _fixp2longint may have increased the exponent */
    n->exp += n->fracpart.seq.digits - 1 - scale;
    _setscale(n, &l, n->fracpart.seq.digits - 1);
    return desc2str(tokens, n, scale);
}

static char
_isvalidbase(
    signed char base)
{
    return base == 10 || lgbase(base) != 0;
}

static Error
_outsci(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    if (n->prefix.base == 10)
        return _outscidec(tokens, x, n, scale);
    return _outscihex(tokens, x, n, scale);
}

static Error
_outfixpdec(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    //edit for bug-19653
    float_checkedround(x, scale + 1);
    int digits;

    digits = float_getexponent(x) + scale + 1;
    if (digits <= 0)
        /* underflow */
        return IOConversionUnderflow;
    if (float_round(x, x, digits, TONEAREST) != TRUE)
        /* float_round() can err if the number contains too many digits */
        return float_geterror();
    _setfndesc(n, x);
    return desc2str(tokens, n, scale);
}

static Error
_outfixphex(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    t_longint l;
    Error result;

    float_copy(x, x, DECPRECISION + 1);
    result = _fixp2longint(n, &l, x, scale);
    if (result != Success)
        return result;
    if (l.length == 0) {
        result = float_geterror();
        return result != Success ? result : IOConversionUnderflow;
    }
    _setscale(n, &l, scale);
    return desc2str(tokens, n, scale);
}

static Error
_outfixp(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    if (n->prefix.base == 10)
        return _outfixpdec(tokens, x, n, scale);
    return _outfixphex(tokens, x, n, scale);
}

static Error
_outengdec(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    int shift;

    float_checkedround(x, scale + 1);
    n->exp = float_getexponent(x);
    if (n->exp < 0)
        shift = 2 - (-n->exp - 1) % 3;
    else
        shift = n->exp % 3;
    float_setexponent(x, shift);
    n->exp -= shift;
    _setfndesc(n, x);
    return desc2str(tokens, n, scale - shift);
}

static Error
_outeng(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    if (n->prefix.base != 10 || scale < 2)
        return InvalidParam;
    return _outengdec(tokens, x, n, scale);
}

static Error
_outcompl(
    p_otokens tokens,
    floatnum x,
    p_number_desc n,
    int scale)
{
    (void)scale;

    if (!float_isinteger(x))
        return IOInvalidComplement;
    if (n->prefix.sign == IO_SIGN_MINUS)
        n->prefix.sign = IO_SIGN_COMPLEMENT;
    else
        n->prefix.sign = IO_SIGN_NONE;
    return _outfixphex(tokens, x, n, 0);
}

static void
_emptybuffer(
    p_buffer token)
{
    if (token->sz > 0)
        *(token->buf) = '\0';
    else
        token->buf = NULL;
}

static void
_emptytokens(
    p_otokens tokens)
{
    _emptybuffer(&tokens->intpart);
    _emptybuffer(&tokens->fracpart);
    tokens->sign = IO_SIGN_NONE;
    tokens->base = IO_BASE_NAN;
    tokens->exp = 0;
}

Error float_out(
    p_otokens tokens,
    floatnum x,
    int scale,
    signed char base,
    char outmode)
{
    t_number_desc n;

    _emptytokens(tokens);
    /* do some sanity checks first */
    if (!_validmode(outmode) || scale < 0 || !_isvalidbase(base))
        return InvalidParam;
    _clearnumber(&n);
    if (float_iszero(x))
        n.prefix.base = IO_BASE_ZERO;
    else if (!float_isnan(x))
        n.prefix.base = base;
    if (!_isvalidbase(n.prefix.base))
        /* NaN and 0 are handled here */
        return desc2str(tokens, &n, 0);
    n.prefix.sign = float_getsign(x);
    float_abs(x);
    switch (outmode) {
    case IO_MODE_FIXPOINT:
        return _outfixp(tokens, x, &n, scale);
    case IO_MODE_ENG:
        return _outeng(tokens, x, &n, scale);
    case IO_MODE_COMPLEMENT:
        return _outcompl(tokens, x, &n, 0);
    default:
        return _outsci(tokens, x, &n, scale);
    }
}

/**
 * 新增入参 bitlength:数据长度 8,16,32,64
 * 新增入参 b：当2进制转换8,16进制时不考虑符号位
 */
Error
float_in(floatnum x,
    p_itokens tokens,
         int bitlength,
         signed char b)
{
    t_number_desc n;
    Error result;

    if ((result = str2desc(&n, tokens)) == Success)
        result = pack2floatnum(x, &n, bitlength, b);
    if (result != Success) {
        _seterror(x, BadLiteral);
        float_setnan(x);
    }
    return result;
}
