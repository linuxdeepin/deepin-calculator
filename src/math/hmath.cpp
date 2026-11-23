// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "hmath.h"
#include "floatconfig.h"
#include "../ddlog.h"

#include "rational.h"
#include "../3rdparty/core/settings.h"

#include <mpfr.h>
#include <gmp.h>

#include <QMap>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

// Precision constants
// DECPRECISION and MATHPRECISION come from floatconfig.h
#define HMATH_MAX_SHOWN 15
// Working precision: MATHPRECISION=159 decimal digits → ceil(159*log2(10)) ≈ 528 bits
#define MPFR_WORKING_PREC 528
#define MPFR_EVAL_PREC 560

/*------------------------   HNumberPrivate   --------------------*/

class HNumberPrivate
{
public:
    HNumberPrivate();
    HNumberPrivate(const HNumberPrivate &other);
    ~HNumberPrivate();

    mpfr_t fnum;
    Error error;

    HNumberPrivate &operator=(const HNumberPrivate &other);
};

HNumberPrivate::HNumberPrivate()
    : error(Success)
{
    mpfr_init2(fnum, MPFR_WORKING_PREC);
    mpfr_set_nan(fnum);
}

HNumberPrivate::HNumberPrivate(const HNumberPrivate &other)
    : error(other.error)
{
    mpfr_init2(fnum, MPFR_WORKING_PREC);
    mpfr_set(fnum, other.fnum, MPFR_RNDN);
}

HNumberPrivate::~HNumberPrivate()
{
    mpfr_clear(fnum);
}

HNumberPrivate &HNumberPrivate::operator=(const HNumberPrivate &other)
{
    if (this != &other) {
        error = other.error;
        mpfr_set(fnum, other.fnum, MPFR_RNDN);
    }
    return *this;
}

/*------------------------   Helper routines  -------------------------*/

static Error checkNaNParam(const HNumberPrivate &v1, const HNumberPrivate *v2 = nullptr)
{
    if (!mpfr_nan_p(v1.fnum) && (!v2 || !mpfr_nan_p(v2->fnum)))
        return Success;
    Error error = v1.error;
    if (error == Success && v2)
        error = v2->error;
    return error == 0 ? NoOperand : error;
}

static void roundResult(HNumberPrivate *dest)
{
    // Round to working precision (already at working prec, just check for errors)
    if (mpfr_nan_p(dest->fnum) && dest->error == Success)
        dest->error = NoOperand;
}

/*--------------------------   HNumber   -------------------*/

HNumber::HNumber()
    : d(new HNumberPrivate)
{
}

HNumber::HNumber(const HNumber &hn)
    : d(new HNumberPrivate)
{
    operator=(hn);
}

HNumber::HNumber(int i)
    : d(new HNumberPrivate)
{
    if (i >= 0)
        mpfr_set_ui(d->fnum, (unsigned long)i, MPFR_RNDN);
    else
        mpfr_set_si(d->fnum, (long)i, MPFR_RNDN);
}

HNumber::HNumber(const char *str)
    : d(new HNumberPrivate)
{
    if (!str || !*str) {
        return; // stays NaN
    }

    QString s = QString::fromLatin1(str).trimmed();
    if (s.isEmpty()) return;

    // Check for base prefixes
    bool useBase = false;
    int base = 10;
    QString numStr = s;

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        base = 16;
        numStr = s.mid(2);
        useBase = true;
    } else if (s.startsWith("0o", Qt::CaseInsensitive)) {
        base = 8;
        numStr = s.mid(2);
        useBase = true;
    } else if (s.startsWith("0b", Qt::CaseInsensitive)) {
        base = 2;
        numStr = s.mid(2);
        useBase = true;
    }

    // Try to parse
    numStr.replace(',', '.');

    if (useBase) {
        // For non-decimal bases, only integer part
        if (mpfr_set_str(d->fnum, numStr.toLatin1().constData(), base, MPFR_RNDN) != 0) {
            // parse error, stays NaN
            d->error = BadLiteral;
        }
    } else {
        if (mpfr_set_str(d->fnum, numStr.toLatin1().constData(), 10, MPFR_RNDN) != 0) {
            d->error = BadLiteral;
        }
    }
}

HNumber::HNumber(const char *str, bool b)
    : d(new HNumberPrivate)
{
    // The bool parameter indicates whether to allow non-decimal base prefixes
    // This is used for programmer mode where 0x/0o/0b prefixes should be parsed
    if (!str || !*str) {
        return;
    }

    QString s = QString::fromLatin1(str).trimmed();
    if (s.isEmpty()) return;

    int base = 10;
    QString numStr = s;

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        base = 16;
        numStr = s.mid(2);
    } else if (s.startsWith("0o", Qt::CaseInsensitive)) {
        base = 8;
        numStr = s.mid(2);
    } else if (s.startsWith("0b", Qt::CaseInsensitive)) {
        base = 2;
        numStr = s.mid(2);
    }

    numStr.replace(',', '.');

    if (mpfr_set_str(d->fnum, numStr.toLatin1().constData(), base, MPFR_RNDN) != 0) {
        d->error = BadLiteral;
    }
}

HNumber::HNumber(const QJsonObject &json)
    : d(new HNumberPrivate)
{
    *this = deSerialize(json);
}

HNumber::~HNumber()
{
    delete d;
}

Error HNumber::error() const
{
    return d->error;
}

bool HNumber::isNan() const
{
    return mpfr_nan_p(d->fnum) != 0;
}

bool HNumber::isZero() const
{
    return mpfr_zero_p(d->fnum) != 0;
}

bool HNumber::isNearZero() const
{
    if (mpfr_zero_p(d->fnum))
        return true;
    if (mpfr_nan_p(d->fnum))
        return false;
    mpfr_exp_t exp = mpfr_get_exp(d->fnum);
    return exp <= -80;
}

bool HNumber::isPositive() const
{
    if (mpfr_nan_p(d->fnum))
        return false;
    return mpfr_sgn(d->fnum) > 0;
}

bool HNumber::isNegative() const
{
    if (mpfr_nan_p(d->fnum))
        return false;
    return mpfr_sgn(d->fnum) < 0;
}

bool HNumber::isInteger() const
{
    if (mpfr_nan_p(d->fnum))
        return false;
    return mpfr_integer_p(d->fnum) != 0;
}

void HNumber::serialize(QJsonObject &json) const
{
    // Serialize as fixed-point string with full precision
    json["value"] = HMath::format(*this, Format::Fixed() + Format::Precision(DECPRECISION));
}

HNumber HNumber::deSerialize(const QJsonObject &json)
{
    HNumber result;
    if (json.contains("value")) {
        QString str = json["value"].toString();
        str.replace(",", ".");
        result = HNumber(str.toLatin1().constData());
    }
    return result;
}

int HNumber::toInt() const
{
    if (mpfr_nan_p(d->fnum))
        return 0;
    return (int)mpfr_get_si(d->fnum, MPFR_RNDZ);
}

HNumber &HNumber::operator=(const HNumber &hn)
{
    if (this != &hn) {
        d->error = hn.d->error;
        mpfr_set(d->fnum, hn.d->fnum, MPFR_RNDN);
    }
    return *this;
}

HNumber HNumber::operator+(const HNumber &num) const
{
    if (this->isZero() && !num.isNan())
        return num;
    if (num.isZero() && !this->isNan())
        return *this;

    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error == Success) {
        mpfr_add(result.d->fnum, d->fnum, num.d->fnum, MPFR_RNDN);
        roundResult(result.d);
    }
    return result;
}

HNumber &HNumber::operator+=(const HNumber &num)
{
    return operator=(*this + num);
}

HNumber operator-(const HNumber &n1, const HNumber &n2)
{
    HNumber result;
    result.d->error = checkNaNParam(*n1.d, n2.d);
    if (result.d->error == Success) {
        mpfr_sub(result.d->fnum, n1.d->fnum, n2.d->fnum, MPFR_RNDN);
        roundResult(result.d);
    }
    return result;
}

HNumber &HNumber::operator-=(const HNumber &num)
{
    return operator=(*this - num);
}

HNumber HNumber::operator*(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error == Success) {
        mpfr_mul(result.d->fnum, d->fnum, num.d->fnum, MPFR_RNDN);
        roundResult(result.d);
    }
    return result;
}

HNumber &HNumber::operator*=(const HNumber &num)
{
    return operator=(*this * num);
}

HNumber HNumber::operator/(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error == Success) {
        if (mpfr_zero_p(num.d->fnum)) {
            result.d->error = ZeroDivide;
        } else {
            mpfr_div(result.d->fnum, d->fnum, num.d->fnum, MPFR_RNDN);
            roundResult(result.d);
        }
    }
    return result;
}

HNumber &HNumber::operator/=(const HNumber &num)
{
    return operator=(*this / num);
}

HNumber HNumber::operator%(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error == Success) {
        if (mpfr_zero_p(num.d->fnum)) {
            result.d->error = ZeroDivide;
        } else {
            // Integer modulo: a % b = a - b * floor(a/b)
            mpfr_t q;
            mpfr_init2(q, MPFR_WORKING_PREC);
            mpfr_div(q, d->fnum, num.d->fnum, MPFR_RNDN);
            mpfr_floor(q, q);
            mpfr_mul(q, q, num.d->fnum, MPFR_RNDN);
            mpfr_sub(result.d->fnum, d->fnum, q, MPFR_RNDN);
            mpfr_clear(q);
            roundResult(result.d);
        }
    }
    return result;
}

int HNumber::compare(const HNumber &other) const
{
    if (mpfr_nan_p(d->fnum) || mpfr_nan_p(other.d->fnum))
        return 0; // NaN comparisons return false
    return mpfr_cmp(d->fnum, other.d->fnum);
}

bool operator>(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) > 0;
}

bool operator<(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) < 0;
}

bool operator>=(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) >= 0;
}

bool operator<=(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) <= 0;
}

bool operator==(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) == 0;
}

bool operator!=(const HNumber &l, const HNumber &r)
{
    if (l.isNan() || r.isNan())
        return false;
    return l.compare(r) != 0;
}

/*------------------------   Bitwise operations   -------------------------*/
// These operate on the integer part using mpz

HNumber HNumber::operator&(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error != Success) return result;
    mpz_t za, zb, zr;
    mpz_init(za); mpz_init(zb); mpz_init(zr);
    mpfr_get_z(za, d->fnum, MPFR_RNDZ);
    mpfr_get_z(zb, num.d->fnum, MPFR_RNDZ);
    mpz_and(zr, za, zb);
    mpfr_set_z(result.d->fnum, zr, MPFR_RNDN);
    mpz_clear(za); mpz_clear(zb); mpz_clear(zr);
    return result;
}

HNumber &HNumber::operator&=(const HNumber &num)
{
    return operator=(*this & num);
}

HNumber HNumber::operator|(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error != Success) return result;
    mpz_t za, zb, zr;
    mpz_init(za); mpz_init(zb); mpz_init(zr);
    mpfr_get_z(za, d->fnum, MPFR_RNDZ);
    mpfr_get_z(zb, num.d->fnum, MPFR_RNDZ);
    mpz_ior(zr, za, zb);
    mpfr_set_z(result.d->fnum, zr, MPFR_RNDN);
    mpz_clear(za); mpz_clear(zb); mpz_clear(zr);
    return result;
}

HNumber &HNumber::operator|=(const HNumber &num)
{
    return operator=(*this | num);
}

HNumber HNumber::operator^(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error != Success) return result;
    mpz_t za, zb, zr;
    mpz_init(za); mpz_init(zb); mpz_init(zr);
    mpfr_get_z(za, d->fnum, MPFR_RNDZ);
    mpfr_get_z(zb, num.d->fnum, MPFR_RNDZ);
    mpz_xor(zr, za, zb);
    mpfr_set_z(result.d->fnum, zr, MPFR_RNDN);
    mpz_clear(za); mpz_clear(zb); mpz_clear(zr);
    return result;
}

HNumber &HNumber::operator^=(const HNumber &num)
{
    return operator=(*this ^ num);
}


HNumber HNumber::operator~() const
{
    HNumber result;
    result.d->error = checkNaNParam(*d);
    if (result.d->error != Success)
        return result;

    mpz_t za, zr;
    mpz_init(za);
    mpz_init(zr);

    mpfr_get_z(za, d->fnum, MPFR_RNDZ);
    mpz_com(zr, za);
    mpfr_set_z(result.d->fnum, zr, MPFR_RNDN);

    mpz_clear(za);
    mpz_clear(zr);

    return result;
}

HNumber operator-(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error == Success) {
        mpfr_neg(result.d->fnum, x.d->fnum, MPFR_RNDN);
    }
    return result;
}

HNumber HNumber::operator<<(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error != Success)
        return result;

    if (!num.isInteger() || num.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }

    mpz_t za;
    mpz_init(za);
    mpfr_get_z(za, d->fnum, MPFR_RNDZ);

    unsigned long shift = (unsigned long)mpfr_get_ui(num.d->fnum, MPFR_RNDZ);
    mpz_mul_2exp(za, za, shift);
    mpfr_set_z(result.d->fnum, za, MPFR_RNDN);

    mpz_clear(za);
    return result;
}

HNumber HNumber::operator>>(const HNumber &num) const
{
    HNumber result;
    result.d->error = checkNaNParam(*d, num.d);
    if (result.d->error != Success)
        return result;

    if (!num.isInteger() || num.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }

    mpz_t za;
    mpz_init(za);
    mpfr_get_z(za, d->fnum, MPFR_RNDZ);

    unsigned long shift = (unsigned long)mpfr_get_ui(num.d->fnum, MPFR_RNDZ);
    // Arithmetic shift right: preserve sign
    int sign = mpz_sgn(za);
    mpz_fdiv_q_2exp(za, za, shift);
    if (sign < 0) {
        // For arithmetic shift, negative numbers should extend sign
        // mpz_fdiv_q_2exp already rounds toward -inf, which is arithmetic shift
    }
    mpfr_set_z(result.d->fnum, za, MPFR_RNDN);

    mpz_clear(za);
    return result;
}

int HNumber::realNumberlength() const
{
    if (mpfr_nan_p(d->fnum))
        return 0;
    // Count significant digits
    char *str = mpfr_get_str(nullptr, nullptr, 10, 0, d->fnum, MPFR_RNDN);
    if (!str)
        return 0;
    int len = strlen(str);
    mpfr_free_str(str);
    return len;
}

/*------------------------   Format   -------------------------*/

HNumber::Format::Format()
    : base(Base::Null)
    , radixChar(RadixChar::Null)
    , mode(Mode::Null)
    , cut(AutoCut::Null)
    , precision(PrecisionNull)
{
}

HNumber::Format::Format(const HNumber::Format &other)
    : base(other.base)
    , radixChar(other.radixChar)
    , mode(other.mode)
    , cut(other.cut)
    , precision(other.precision)
{
}

HNumber::Format HNumber::Format::operator+(const HNumber::Format &other) const
{
    Format result;
    result.base = (this->base != Base::Null) ? this->base : other.base;
    result.radixChar = (this->radixChar != RadixChar::Null) ? this->radixChar : other.radixChar;
    result.mode = (this->mode != Mode::Null) ? this->mode : other.mode;
    result.cut = (this->cut != AutoCut::Null) ? this->cut : other.cut;
    result.precision = (this->precision != PrecisionNull) ? this->precision : other.precision;
    return result;
}

HNumber::Format HNumber::Format::Binary() { Format r; r.base = Base::Binary; return r; }
HNumber::Format HNumber::Format::Octal() { Format r; r.base = Base::Octal; return r; }
HNumber::Format HNumber::Format::Decimal() { Format r; r.base = Base::Decimal; return r; }
HNumber::Format HNumber::Format::Hexadecimal() { Format r; r.base = Base::Hexadecimal; return r; }
HNumber::Format HNumber::Format::Precision(int prec) { Format r; r.precision = prec; return r; }
HNumber::Format HNumber::Format::Point() { Format r; r.radixChar = RadixChar::Point; return r; }
HNumber::Format HNumber::Format::Comma() { Format r; r.radixChar = RadixChar::Comma; return r; }
HNumber::Format HNumber::Format::General() { Format r; r.mode = Mode::General; return r; }
HNumber::Format HNumber::Format::Fixed() { Format r; r.mode = Mode::Fixed; return r; }
HNumber::Format HNumber::Format::Scientific() { Format r; r.mode = Mode::Scientific; return r; }
HNumber::Format HNumber::Format::Engineering() { Format r; r.mode = Mode::Engineering; return r; }
HNumber::Format HNumber::Format::Complement() { Format r; r.mode = Mode::Complement; return r; }
HNumber::Format HNumber::Format::NCut() { Format r; r.cut = AutoCut::Ncut; return r; }

/*------------------------   Formatting helpers   -------------------------*/

namespace {

// Format a number in scientific notation: d.ddddeX
// Returns string like "3.14159E+0" or "1.234E+4"
QString formatScientificMPFR(const mpfr_t val, int prec, int base)
{
    if (mpfr_nan_p(val))
        return QStringLiteral("nan");

    // Determine sign
    bool isNeg = mpfr_sgn(val) < 0;
    mpfr_t absval;
    mpfr_init2(absval, mpfr_get_prec(val));
    mpfr_abs(absval, val, MPFR_RNDN);

    if (mpfr_zero_p(val)) {
        mpfr_clear(absval);
        if (prec <= 0 || prec == HMATH_MAX_SHOWN) {
            return QStringLiteral("0E+0");
        }
        QString result = "0";
        if (prec > 0) {
            result += ".";
            for (int i = 0; i < prec; i++)
                result += '0';
        }
        result += "E+0";
        return isNeg ? "-" + result : result;
    }

    // Get the exponent (base 10)
    mpfr_exp_t exp10 = 0;
    char *digits = nullptr;
    int numDigits = (prec < 0) ? HMATH_MAX_SHOWN : prec;

    if (base == 10) {
        // Use mpfr_get_str with base 10
        // We need numDigits+1 significant digits (1 before decimal point)
        digits = mpfr_get_str(nullptr, &exp10, 10, numDigits + 1, absval, MPFR_RNDN);
    } else {
        // For non-decimal bases, still use decimal for scientific
        digits = mpfr_get_str(nullptr, &exp10, 10, numDigits + 1, absval, MPFR_RNDN);
        base = 10; // force decimal for scientific
    }

    if (!digits) {
        mpfr_clear(absval);
        return QStringLiteral("nan");
    }

    // digits is like "314159265..." and exp10 is the position of decimal point
    // So the value is 0.digits * 10^exp10
    // We want format: d.ddddeX where X = exp10 - 1
    QString digitStr = QString::fromLatin1(digits);
    mpfr_free_str(digits);

    // Remove leading zeros (shouldn't be any with mpfr_get_str, but just in case)
    while (digitStr.length() > 1 && digitStr[0] == '0')
        digitStr.remove(0, 1);

    // The exponent for scientific notation
    int sciExp = (int)exp10 - 1;

    // Build the string: d.dddd
    QString result;
    if (isNeg)
        result = "-";
    result += digitStr[0];
    if (numDigits > 0 && digitStr.length() > 1) {
        // Add remaining digits, up to numDigits
        QString frac = digitStr.mid(1, numDigits);
        // Suppress trailing zeros if precision is 15 or 31 or 65 or negative
        if (prec == 15 || prec == 31 || prec == 65 || prec < 0) {
            while (frac.length() > 0 && frac[frac.length()-1] == '0')
                frac.chop(1);
        }
        if (!frac.isEmpty())
            result += "." + frac;
    }

    // Add exponent: E+X
    result += "E";
    if (sciExp >= 0)
        result += "+";
    else
        result += "-";
    result += QString::number(abs(sciExp));

    mpfr_clear(absval);
    return result;
}

// Format in fixed notation: ddddd.dddd
QString formatFixedMPFR(const mpfr_t val, int prec, int base)
{
    if (mpfr_nan_p(val))
        return QStringLiteral("nan");

    bool isNeg = mpfr_sgn(val) < 0;
    mpfr_t absval;
    mpfr_init2(absval, mpfr_get_prec(val));
    mpfr_abs(absval, val, MPFR_RNDN);

    if (mpfr_zero_p(val)) {
        mpfr_clear(absval);
        QString result = "0";
        if (prec > 0 && prec != 15 && prec != 31 && prec != 65) {
            result += ".";
            for (int i = 0; i < prec; i++)
                result += '0';
        }
        return isNeg ? "-" + result : result;
    }

    int numDigits = (prec < 0) ? HMATH_MAX_SHOWN : prec;

    if (base == 10) {
        mpfr_exp_t exp10 = 0;
        char *digits = mpfr_get_str(nullptr, &exp10, 10, numDigits + abs((int)exp10) + 10, absval, MPFR_RNDN);
        if (!digits) {
            mpfr_clear(absval);
            return QStringLiteral("nan");
        }

        QString digitStr = QString::fromLatin1(digits);
        mpfr_free_str(digits);

        // exp10 is the position of the decimal point from the start of digits
        QString intPart, fracPart;

        if (exp10 <= 0) {
            intPart = "0";
            fracPart = QString(-exp10, '0') + digitStr;
        } else {
            if (exp10 >= digitStr.length()) {
                intPart = digitStr + QString(exp10 - digitStr.length(), '0');
                fracPart = "";
            } else {
                intPart = digitStr.left(exp10);
                fracPart = digitStr.mid(exp10);
            }
        }

        // Truncate or pad fracPart to prec digits
        if (prec >= 0) {
            if (fracPart.length() > numDigits)
                fracPart = fracPart.left(numDigits);
            else if (fracPart.length() < numDigits && prec != 15 && prec != 31 && prec != 65)
                fracPart += QString(numDigits - fracPart.length(), '0');

            // Suppress trailing zeros for certain precision values
            if (prec == 15 || prec == 31 || prec == 65 || prec < 0) {
                while (fracPart.length() > 0 && fracPart[fracPart.length()-1] == '0')
                    fracPart.chop(1);
            }
        }

        QString result;
        if (isNeg)
            result = "-";
        result += intPart;
        if (!fracPart.isEmpty())
            result += "." + fracPart;

        mpfr_clear(absval);
        return result;
    }

    // Non-decimal base (hex, oct, bin) - convert integer part
    mpz_t zval;
    mpz_init(zval);
    mpfr_get_z(zval, absval, MPFR_RNDZ);

    char *baseStr = mpz_get_str(nullptr, base, zval);
    QString result;
    if (isNeg)
        result = "-";
    result += "0";
    if (base == 16) result += "x";
    else if (base == 8) result += "o";
    else if (base == 2) result += "b";
    result += QString::fromLatin1(baseStr).toUpper();

    free(baseStr);
    mpz_clear(zval);
    mpfr_clear(absval);
    return result;
}

// Format in engineering notation: dd.ddddeX where X is multiple of 3
QString formatEngineeringMPFR(const mpfr_t val, int prec, int base)
{
    if (mpfr_nan_p(val))
        return QStringLiteral("nan");

    // Get scientific first
    QString sci = formatScientificMPFR(val, prec, base);

    // Parse exponent
    int ePos = sci.indexOf('E');
    if (ePos < 0)
        return sci;

    int exp = sci.mid(ePos + 1).toInt();
    // Adjust to engineering: exponent should be multiple of 3
    int engExp = (exp / 3) * 3;
    if (exp < 0 && exp % 3 != 0)
        engExp -= 3;

    int shift = exp - engExp;
    // Shift decimal point in mantissa
    QString mantissa = sci.left(ePos);
    bool isNeg = mantissa.startsWith("-");
    if (isNeg)
        mantissa = mantissa.mid(1);

    int dotPos = mantissa.indexOf('.');
    if (dotPos < 0) {
        mantissa += ".";
        dotPos = mantissa.length() - 1;
    }

    // Move decimal point right by 'shift' positions
    QString intPart = mantissa.left(dotPos);
    QString fracPart = mantissa.mid(dotPos + 1);

    for (int i = 0; i < shift; i++) {
        if (fracPart.isEmpty())
            intPart += "0";
        else {
            intPart += fracPart[0];
            fracPart.remove(0, 1);
        }
    }

    QString result;
    if (isNeg)
        result = "-";
    result += intPart;
    if (!fracPart.isEmpty())
        result += "." + fracPart;
    result += "E";
    if (engExp >= 0)
        result += "+";
    else
        result += "-";
    result += QString::number(abs(engExp));

    return result;
}

// Format in complement mode (for programmer mode)
QString formatComplementMPFR(const mpfr_t val, int prec, int base, bool autocut)
{
    if (mpfr_nan_p(val))
        return QStringLiteral("nan");

    bool isNeg = mpfr_sgn(val) < 0;

    if (base == 2 || base == 8 || base == 16) {
        // For non-decimal complement, convert absolute value to the base
        // Negative numbers use two's complement representation
        mpz_t zval;
        mpz_init(zval);

        if (isNeg) {
            // Two's complement: need bit length from Settings
            int bitLength = 64; // default
            if (Settings::instance())
                bitLength = Settings::instance()->proBitLength;

            mpz_t mask;
            mpz_init(mask);
            mpz_ui_pow_ui(mask, 2, bitLength);
            mpz_sub_ui(mask, mask, 1);

            mpfr_get_z(zval, val, MPFR_RNDZ);
            mpz_and(zval, zval, mask);
            mpz_clear(mask);
        } else {
            mpfr_get_z(zval, val, MPFR_RNDZ);
        }

        char *baseStr = mpz_get_str(nullptr, base, zval);
        QString result = "0";
        if (base == 16) result += "x";
        else if (base == 8) result += "o";
        else if (base == 2) result += "b";
        result += QString::fromLatin1(baseStr).toUpper();

        free(baseStr);
        mpz_clear(zval);
        return result;
    }

    // Decimal complement - just format as fixed
    return formatFixedMPFR(val, prec, base);
}

// General format: choose between fixed and scientific based on magnitude
// Reproduces the old formatGeneral heuristic including n_scale checks
QString formatGeneralMPFR(const mpfr_t val, int prec, int base)
{
    if (mpfr_nan_p(val))
        return QStringLiteral("nan");

    if (mpfr_zero_p(val)) {
        if (prec < 0)
            return QStringLiteral("0E+0");
        return formatFixedMPFR(val, prec, base);
    }

    // Get the exponent and total digit count (n_scale equivalent)
    mpfr_t absval;
    mpfr_init2(absval, mpfr_get_prec(val));
    mpfr_abs(absval, val, MPFR_RNDN);

    mpfr_exp_t exp10 = 0;
    // Get all digits to determine n_scale (number of fractional digits)
    char *digits = mpfr_get_str(nullptr, &exp10, 10, 0, absval, MPFR_RNDN);
    int n_scale = 0;
    if (digits) {
        // n_scale = number of digits after decimal point
        // exp10 is position of decimal point from start of digits
        // If exp10 <= 0, all digits are fractional, n_scale = -exp10 + strlen
        // If exp10 > 0, n_scale = strlen - exp10
        int digitLen = strlen(digits);
        if (exp10 <= 0)
            n_scale = -exp10 + digitLen;
        else
            n_scale = (exp10 >= digitLen) ? 0 : digitLen - exp10;
        mpfr_free_str(digits);
    }
    mpfr_clear(absval);

    int expd = (int)exp10 - 1; // 0-based exponent

    int usePrec = (prec < 0) ? HMATH_MAX_SHOWN : prec;

    // Decision logic matching old formatGeneral exactly:
    if (expd > prec)
        return formatScientificMPFR(val, usePrec, base);
    if (expd < -prec)
        return formatScientificMPFR(val, usePrec, base);
    if (expd < 0 && prec >= 0 && expd < -prec)
        return formatScientificMPFR(val, usePrec, base);
    // n_scale heuristic: if many fractional digits relative to precision, use scientific
    if (expd < 0 && (n_scale + 1) <= usePrec - 2 && (n_scale - expd) > usePrec)
        return formatScientificMPFR(val, usePrec, base);
    if (expd < -3 && (n_scale + 1) > usePrec - 2)
        return formatScientificMPFR(val, usePrec, base);

    // Otherwise fixed
    QString fixed = formatFixedMPFR(val, usePrec, base);

    // Additional check from old code: verify scientific format doesn't round to 1e0
    QString strnew = fixed;
    strnew.remove(QRegularExpression("[^0-9E]"));
    int e = strnew.indexOf("E");
    if (e > 0) {
        if (strnew.left(e).length() + strnew.right(strnew.length() - e - 1).toInt() < usePrec + 2)
            fixed = formatFixedMPFR(val, usePrec, base);
    }

    return fixed;
}

} // unnamed namespace

/*------------------------   HMath::format   -------------------------*/

QString HMath::format(const HNumber &hn, HNumber::Format format)
{
    if (hn.isNan())
        return QStringLiteral("nan");

    if (format.precision < 0 && format.precision != HNumber::Format::PrecisionNull)
        format.precision = -1;
    if (format.precision == HNumber::Format::PrecisionNull)
        format.precision = -1;

    int base;
    switch (format.base) {
    case HNumber::Format::Base::Binary: base = 2; break;
    case HNumber::Format::Base::Octal: base = 8; break;
    case HNumber::Format::Base::Hexadecimal: base = 16; break;
    case HNumber::Format::Base::Decimal:
    case HNumber::Format::Base::Null:
    default: base = 10; break;
    }

    bool isautocut = true;
    if (format.cut == HNumber::Format::AutoCut::Ncut)
        isautocut = false;

    QString result;

    switch (format.mode) {
    case HNumber::Format::Mode::Fixed:
        result = formatFixedMPFR(hn.d->fnum, format.precision, base);
        break;
    case HNumber::Format::Mode::Scientific:
        result = formatScientificMPFR(hn.d->fnum, format.precision, base);
        break;
    case HNumber::Format::Mode::Engineering:
        result = formatEngineeringMPFR(hn.d->fnum, format.precision, base);
        break;
    case HNumber::Format::Mode::Complement: {
        if (base == 8 || base == 16) {
            result = formatComplementMPFR(hn.d->fnum, format.precision, 2, isautocut);
            // For oct/hex complement, first convert to binary, then to target base
            // Actually, the old code converts to binary complement then formats in target base
            result = formatComplementMPFR(hn.d->fnum, format.precision, base, isautocut);
        } else if (base == 10) {
            result = formatComplementMPFR(hn.d->fnum, format.precision, base, isautocut);
        } else {
            result = formatComplementMPFR(hn.d->fnum, format.precision, base, isautocut);
        }
        break;
    }
    case HNumber::Format::Mode::General:
    default:
        result = formatGeneralMPFR(hn.d->fnum, format.precision, base);
        break;
    }

    return result;
}

/*------------------------   HMath functions   -------------------------*/

HNumber HMath::rad2deg(const HNumber &angle) { return angle * (HNumber(180) / pi()); }
HNumber HMath::deg2rad(const HNumber &angle) { return angle * (pi() / HNumber(180)); }
HNumber HMath::rad2gon(const HNumber &angle) { return angle * (HNumber(200) / pi()); }
HNumber HMath::gon2rad(const HNumber &angle) { return angle * (pi() / HNumber(200)); }

HNumber HMath::e()
{
    HNumber value;
    mpfr_t one;
    mpfr_init2(one, MPFR_WORKING_PREC);
    mpfr_set_ui(one, 1, MPFR_RNDN);
    mpfr_exp(value.d->fnum, one, MPFR_RNDN);
    mpfr_clear(one);
    return value;
}

HNumber HMath::pi()
{
    HNumber value;
    mpfr_const_pi(value.d->fnum, MPFR_RNDN);
    return value;
}

HNumber HMath::phi()
{
    // phi = (1 + sqrt(5)) / 2
    HNumber five(5);
    HNumber sqrt5;
    mpfr_sqrt(sqrt5.d->fnum, five.d->fnum, MPFR_RNDN);
    return (HNumber(1) + sqrt5) / HNumber(2);
}

HNumber HMath::nan(Error error)
{
    HNumber result;
    result.d->error = error;
    return result;
}

HNumber HMath::max(const HNumber &n1, const HNumber &n2)
{
    if (n1.isNan() || n2.isNan())
        return HMath::nan(checkNaNParam(*n1.d, n2.d));
    return (n1 > n2) ? n1 : n2;
}

HNumber HMath::min(const HNumber &n1, const HNumber &n2)
{
    if (n1.isNan() || n2.isNan())
        return HMath::nan(checkNaNParam(*n1.d, n2.d));
    return (n1 < n2) ? n1 : n2;
}

HNumber HMath::abs(const HNumber &n)
{
    HNumber result;
    result.d->error = checkNaNParam(*n.d);
    if (result.d->error == Success) {
        mpfr_abs(result.d->fnum, n.d->fnum, MPFR_RNDN);
    }
    return result;
}

HNumber HMath::round(const HNumber &n, int prec)
{
    if (n.isNan())
        return HMath::nan(checkNaNParam(*n.d));
    HNumber result(n);
    if (prec == 0) {
        mpfr_round(result.d->fnum, n.d->fnum);
    } else {
        // Multiply by 10^prec, round, divide by 10^prec
        mpfr_t factor;
        mpfr_init2(factor, MPFR_WORKING_PREC);
        mpfr_ui_pow_ui(factor, 10, prec, MPFR_RNDN);
        mpfr_mul(result.d->fnum, n.d->fnum, factor, MPFR_RNDN);
        mpfr_round(result.d->fnum, result.d->fnum);
        mpfr_div(result.d->fnum, result.d->fnum, factor, MPFR_RNDN);
        mpfr_clear(factor);
    }
    return result;
}

HNumber HMath::trunc(const HNumber &n, int prec)
{
    if (n.isNan())
        return HMath::nan(checkNaNParam(*n.d));
    HNumber result(n);
    if (prec == 0) {
        mpfr_trunc(result.d->fnum, n.d->fnum);
    } else {
        mpfr_t factor;
        mpfr_init2(factor, MPFR_WORKING_PREC);
        mpfr_ui_pow_ui(factor, 10, prec, MPFR_RNDN);
        mpfr_mul(result.d->fnum, n.d->fnum, factor, MPFR_RNDN);
        mpfr_trunc(result.d->fnum, result.d->fnum);
        mpfr_div(result.d->fnum, result.d->fnum, factor, MPFR_RNDN);
        mpfr_clear(factor);
    }
    return result;
}

HNumber HMath::integer(const HNumber &n)
{
    HNumber result;
    result.d->error = checkNaNParam(*n.d);
    if (result.d->error == Success) {
        mpfr_trunc(result.d->fnum, n.d->fnum);
    }
    return result;
}

HNumber HMath::frac(const HNumber &n)
{
    HNumber result;
    result.d->error = checkNaNParam(*n.d);
    if (result.d->error == Success) {
        mpfr_t intpart;
        mpfr_init2(intpart, MPFR_WORKING_PREC);
        mpfr_trunc(intpart, n.d->fnum);
        mpfr_sub(result.d->fnum, n.d->fnum, intpart, MPFR_RNDN);
        mpfr_clear(intpart);
    }
    return result;
}

HNumber HMath::floor(const HNumber &n)
{
    if (n.isNan())
        return HMath::nan(checkNaNParam(*n.d));
    HNumber result;
    mpfr_floor(result.d->fnum, n.d->fnum);
    return result;
}

HNumber HMath::ceil(const HNumber &n)
{
    if (n.isNan())
        return HMath::nan(checkNaNParam(*n.d));
    HNumber result;
    mpfr_ceil(result.d->fnum, n.d->fnum);
    return result;
}

HNumber HMath::gcd(const HNumber &n1, const HNumber &n2)
{
    if (!n1.isInteger() || !n2.isInteger()) {
        Error error = checkNaNParam(*n1.d, n2.d);
        if (error != Success)
            return HMath::nan(error);
        return HMath::nan(TypeMismatch);
    }

    HNumber a = abs(n1);
    HNumber b = abs(n2);

    if (a == 0) return b;
    if (b == 0) return a;

    while (true) {
        a = a % b;
        if (a == 0) return b;
        b = b % a;
        if (b == 0) return a;
    }
}

HNumber HMath::idiv(const HNumber &dividend, const HNumber &divisor)
{
    HNumber result;
    result.d->error = checkNaNParam(*dividend.d, divisor.d);
    if (result.d->error != Success)
        return result;
    if (divisor.isZero()) {
        result.d->error = ZeroDivide;
        return result;
    }
    // Integer division = trunc(a/b)
    mpfr_div(result.d->fnum, dividend.d->fnum, divisor.d->fnum, MPFR_RNDN);
    mpfr_trunc(result.d->fnum, result.d->fnum);
    return result;
}

HNumber HMath::sqrt(const HNumber &n)
{
    HNumber result;
    result.d->error = checkNaNParam(*n.d);
    if (result.d->error != Success)
        return result;
    if (n.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }
    mpfr_sqrt(result.d->fnum, n.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::cbrt(const HNumber &n)
{
    HNumber result;
    result.d->error = checkNaNParam(*n.d);
    if (result.d->error != Success)
        return result;
    if (n.isZero())
        return n;
    mpfr_cbrt(result.d->fnum, n.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::raise(const HNumber &n1, int n)
{
    HNumber r;
    r.d->error = checkNaNParam(*n1.d);
    if (r.d->error != Success)
        return r;
    if (n >= 0)
        mpfr_pow_ui(r.d->fnum, n1.d->fnum, (unsigned long)n, MPFR_RNDN);
    else {
        mpfr_pow_si(r.d->fnum, n1.d->fnum, (long)n, MPFR_RNDN);
    }
    return r;
}

HNumber HMath::raise(const HNumber &n1, const HNumber &n2)
{
    HNumber result;
    result.d->error = checkNaNParam(*n1.d, n2.d);
    if (result.d->error != Success)
        return result;

    // Handle negative base with non-integer exponent
    if (n1.isNegative() && !n2.isInteger()) {
        Rational exp(n2);
        if (abs(exp.toHNumber() - n2) >= HNumber("1e-20") ||
            (n1.isNegative() && exp.denominator() % 2 == 0))
            return HMath::nan(OutOfDomain);

        HNumber temp = -n1;
        mpfr_pow(result.d->fnum, temp.d->fnum, n2.d->fnum, MPFR_RNDN);
        if (exp.numerator() % 2 != 0)
            result = -result;
        return result;
    }

    mpfr_pow(result.d->fnum, n1.d->fnum, n2.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::exp(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_exp(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::ln(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    if (x.isZero() || x.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }
    mpfr_log(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::lg(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    if (x.isZero() || x.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }
    mpfr_log10(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::lb(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    if (x.isZero() || x.isNegative()) {
        result.d->error = OutOfDomain;
        return result;
    }
    mpfr_log2(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::log(const HNumber &base, const HNumber &x)
{
    return lg(x) / lg(base);
}

HNumber HMath::sin(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_sin(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::cos(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_cos(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::tan(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_tan(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::cot(const HNumber &x) { return cos(x) / sin(x); }
HNumber HMath::sec(const HNumber &x) { return HNumber(1) / cos(x); }
HNumber HMath::csc(const HNumber &x) { return HNumber(1) / sin(x); }

HNumber HMath::arctan(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_atan(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::arctan2(const HNumber &x, const HNumber &y)
{
    if (y.isZero()) {
        if (x.isNegative()) return HMath::pi();
        if (x.isZero()) return HMath::nan(OutOfDomain);
        return HNumber(0);
    }
    HNumber phi = HMath::arctan(HMath::abs(y / x));
    if (x.isNegative())
        return (HMath::pi() - phi) * HMath::sgn(y);
    if (x.isZero())
        return HMath::pi() / HNumber(2) * HMath::sgn(y);
    return phi * HMath::sgn(y);
}

HNumber HMath::arcsin(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_asin(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::arccos(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_acos(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::sinh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_sinh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::cosh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_cosh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::tanh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_tanh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::arsinh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_asinh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::arcosh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_acosh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::artanh(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_atanh(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::gamma(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_gamma(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::lnGamma(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    mpfr_lngamma(result.d->fnum, x.d->fnum, MPFR_RNDN);
    return result;
}

HNumber HMath::sgn(const HNumber &x)
{
    if (x.isNan())
        return HMath::nan(checkNaNParam(*x.d));
    int s = mpfr_sgn(x.d->fnum);
    HNumber result;
    if (s > 0) result = HNumber(1);
    else if (s < 0) result = HNumber(-1);
    else result = HNumber(0);
    return result;
}

HNumber HMath::factorial(const HNumber &x, const HNumber &base)
{
    if (x.isNan())
        return HMath::nan(checkNaNParam(*x.d));

    if (base == HNumber(1)) {
        // Standard factorial: x! = Gamma(x+1)
        HNumber result;
        HNumber xplus1 = x + HNumber(1);
        result.d->error = checkNaNParam(*xplus1.d);
        if (result.d->error != Success)
            return result;
        mpfr_gamma(result.d->fnum, xplus1.d->fnum, MPFR_RNDN);
        return result;
    }

    HNumber numer = x - base + HNumber(1);
    HNumber r(base);
    // Gamma(x+1)/Gamma(base) = Gamma(base + numer) / Gamma(base)
    HNumber g1, g2;
    mpfr_gamma(g1.d->fnum, (base + numer).d->fnum, MPFR_RNDN);
    mpfr_gamma(g2.d->fnum, base.d->fnum, MPFR_RNDN);
    r = g1 / g2;
    return r;
}

HNumber HMath::nCr(const HNumber &n, const HNumber &r)
{
    Error error = checkNaNParam(*n.d, r.d);
    if (error != Success)
        return HMath::nan(error);

    if (r.isInteger() && r < 0)
        return HNumber(0);

    HNumber r1 = (r + r > n) ? n - r : r;
    HNumber r2 = n - r1;

    if (r1 >= 0) {
        if (n.isInteger() && r1.isInteger() && n <= 1000 && r1 <= 50)
            return factorial(n, r2 + 1) / factorial(r1, 1);
        // Use Gamma: nCr = exp(lngamma(n+1) - lngamma(r+1) - lngamma(n-r+1))
        HNumber result;
        HNumber ln1, ln2, ln3;
        mpfr_lngamma(ln1.d->fnum, (n + HNumber(1)).d->fnum, MPFR_RNDN);
        mpfr_lngamma(ln2.d->fnum, (r1 + HNumber(1)).d->fnum, MPFR_RNDN);
        mpfr_lngamma(ln3.d->fnum, (r2 + HNumber(1)).d->fnum, MPFR_RNDN);
        mpfr_sub(result.d->fnum, ln1.d->fnum, ln2.d->fnum, MPFR_RNDN);
        mpfr_sub(result.d->fnum, result.d->fnum, ln3.d->fnum, MPFR_RNDN);
        mpfr_exp(result.d->fnum, result.d->fnum, MPFR_RNDN);
        return result;
    }
    if (r2 >= 0 || !r2.isInteger())
        return factorial(n, r1 + 1) / factorial(r2, 1);
    return 0;
}

HNumber HMath::nPr(const HNumber &n, const HNumber &r)
{
    return factorial(n, (n - r + 1));
}

// Probability functions - delegate to combinations of basic ops
static bool checkpn(const HNumber &p, const HNumber &n)
{
    return n.isInteger() && !n.isNegative() && !p.isNan() && !p.isNegative() && p <= 1;
}

HNumber HMath::binomialPmf(const HNumber &k, const HNumber &n, const HNumber &p)
{
    if (!k.isInteger() || !checkpn(p, n))
        return HMath::nan(InvalidParam);
    HNumber result = nCr(n, k);
    if (result.isZero()) return result;
    if (p.isInteger())
        return (int)(p.isZero() ? k.isZero() : n == k);
    return result * raise(p, k) * raise(HNumber(1) - p, n - k);
}

HNumber HMath::binomialCdf(const HNumber &k, const HNumber &n, const HNumber &p)
{
    if (!k.isInteger() || n.isNan())
        return HMath::nan();
    HNumber summand = binomialPmf(0, n, p);
    if (summand.isNan()) return summand;
    HNumber one(1);
    if (k.isNegative()) return 0;
    if (k >= n) return one;
    HNumber pcompl = one - p;
    if (p.isInteger()) return pcompl;
    if (k + k > n && k + one >= p * (n + one))
        return one - binomialCdf(n - k - one, n, pcompl);
    HNumber result(summand);
    for (HNumber i(0); i < k;) {
        summand *= p * (n - i);
        i += one;
        summand /= pcompl * i;
        result += summand;
    }
    return result;
}

HNumber HMath::binomialMean(const HNumber &n, const HNumber &p)
{
    if (!checkpn(p, n)) return HMath::nan();
    return n * p;
}

HNumber HMath::binomialVariance(const HNumber &n, const HNumber &p)
{
    return binomialMean(n, p) * (HNumber(1) - p);
}

static bool checkNMn(const HNumber &N, const HNumber &M, const HNumber &n)
{
    return N.isInteger() && !N.isNegative() && M.isInteger() && !M.isNegative() &&
           n.isInteger() && !n.isNegative() && HMath::max(M, n) <= N;
}

HNumber HMath::hypergeometricPmf(const HNumber &k, const HNumber &N, const HNumber &M, const HNumber &n)
{
    if (!k.isInteger() || !checkNMn(N, M, n)) return HMath::nan();
    return nCr(M, k) * nCr(N - M, n - k) / nCr(N, n);
}

HNumber HMath::hypergeometricCdf(const HNumber &k, const HNumber &N, const HNumber &M, const HNumber &n)
{
    HNumber c = M + n - N;
    HNumber i = max(c, 0);
    HNumber summand = hypergeometricPmf(i, N, M, n);
    if (!k.isInteger() || summand.isNan()) return HMath::nan();
    HNumber one(1);
    if (k >= M || k >= n) return one;
    if (i > k) return 0;
    HNumber result = summand;
    for (; i < k;) {
        summand *= (M - i) * (n - i);
        i += one;
        summand /= i * (i - c);
        result += summand;
    }
    return result;
}

HNumber HMath::hypergeometricMean(const HNumber &N, const HNumber &M, const HNumber &n)
{
    if (!checkNMn(N, M, n)) return HMath::nan();
    return n * M / N;
}

HNumber HMath::hypergeometricVariance(const HNumber &N, const HNumber &M, const HNumber &n)
{
    return (hypergeometricMean(N, M, n) * (HNumber(1) - M / N) * (N - n)) / (N - HNumber(1));
}

HNumber HMath::poissonPmf(const HNumber &k, const HNumber &l)
{
    if (!k.isInteger() || l.isNan() || l.isNegative()) return HMath::nan();
    if (k.isNegative()) return 0;
    if (l.isZero()) return int(k.isZero());
    return exp(-l) * raise(l, k) / factorial(k);
}

HNumber HMath::poissonCdf(const HNumber &k, const HNumber &l)
{
    if (!k.isInteger() || l.isNan() || l.isNegative()) return HMath::nan();
    if (k.isNegative()) return 0;
    HNumber one(1);
    if (l.isZero()) return one;
    HNumber summand = one;
    HNumber result = one;
    for (HNumber i = one; i <= k; i += one) {
        summand *= l / i;
        result += summand;
    }
    result = exp(-l) * result;
    return result;
}

HNumber HMath::poissonMean(const HNumber &l)
{
    if (l.isNan() || l.isNegative()) return HMath::nan();
    return l;
}

HNumber HMath::poissonVariance(const HNumber &l)
{
    return poissonMean(l);
}

// Error function - mpfr doesn't have built-in erf, implement using series
HNumber HMath::erf(const HNumber &x)
{
    HNumber result;
    result.d->error = checkNaNParam(*x.d);
    if (result.d->error != Success)
        return result;
    if (x.isZero())
        return HNumber(0);

    // For small |x|, use Taylor series: erf(x) = (2/sqrt(pi)) * sum[ (-1)^n * x^(2n+1) / (n! * (2n+1)) ]
    // For larger |x|, use erf(x) = 1 - erfc(x) and compute erfc via continued fraction
    // For simplicity and correctness, use the series for |x| < 2, and complementary for larger

    HNumber absx = abs(x);
    if (absx > HNumber("6")) {
        // erf(x) ≈ ±1 for large |x|
        result = x.isNegative() ? HNumber(-1) : HNumber(1);
        return result;
    }

    // Use mpfr's internal erf if available (MPFR 4.2+ may not have it)
    // Implement using the series expansion
    // erf(x) = (2/sqrt(pi)) * x * sum[ (-1)^n * x^(2n) / ((2n+1) * n!) ]
    HNumber sqrtPi = sqrt(pi());
    HNumber coeff = HNumber(2) / sqrtPi;

    HNumber sum;
    HNumber term;
    HNumber x2 = x * x;
    int n = 0;
    int maxIter = 200;

    // Series: sum_{n=0}^{inf} (-1)^n * x^(2n+1) / (n! * (2n+1))
    HNumber term0 = x; // n=0: x / 1
    sum = term0;
    HNumber prevTerm = term0;

    for (n = 1; n < maxIter; n++) {
        // term_n = term_{n-1} * (-x^2) / (n * (2n+1) / (2n-1))
        // Actually: term_n = (-1)^n * x^(2n+1) / (n! * (2n+1))
        // term_n / term_{n-1} = -x^2 * (2n-1) / (n * (2n+1))
        HNumber ratio = -x2 * HNumber(2*n - 1) / (HNumber(n) * HNumber(2*n + 1));
        prevTerm = prevTerm * ratio;
        if (prevTerm.isZero() || abs(prevTerm) < HNumber("1e-80"))
            break;
        sum += prevTerm;
    }

    result = coeff * sum;
    return result;
}

HNumber HMath::erfc(const HNumber &x)
{
    return HNumber(1) - erf(x);
}

// Logic functions
HNumber HMath::mask(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || bits == 0 || bits >= 256 || !bits.isInteger())
        return HMath::nan();
    return val & ~(HNumber(-1) << HNumber(bits));
}

HNumber HMath::sgnext(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || bits == 0 || bits >= 256 || !bits.isInteger())
        return HMath::nan();
    HNumber ofs = HNumber(256) - bits;
    return (val << ofs) >> ofs;
}

HNumber HMath::ashr(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || !bits.isInteger())
        return HMath::nan();
    int bitLen = 64;
    if (Settings::instance())
        bitLen = Settings::instance()->proBitLength;
    if (bits <= -bitLen || bits >= bitLen)
        return HMath::nan();
    if (bits >= 0)
        return val >> bits;
    return val << -bits;
}

HNumber HMath::lshr(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || !bits.isInteger())
        return HMath::nan();
    int bitLen = 64;
    if (Settings::instance())
        bitLen = Settings::instance()->proBitLength;
    if (bits <= -bitLen || bits >= bitLen)
        return HMath::nan();
    if (bits >= 0) {
        return (val >> bits) & ~(((HNumber(-1) << HNumber(bitLen - 1)) >> bits) << HNumber(1));
    }
    return val << -bits;
}

HNumber HMath::rosh(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || !bits.isInteger())
        return HMath::nan();
    int bitLen = 64;
    if (Settings::instance())
        bitLen = Settings::instance()->proBitLength;
    HNumber bitlen(bitLen);
    HNumber shift = bits % bitlen;
    if (shift == 0) return val;
    if (shift < 0) {
        shift = -shift;
        return lshr(val, bitlen - shift) | (val << shift);
    }
    return (val << (bitlen - shift)) | lshr(val, shift);
}

HNumber HMath::rcsh(const HNumber &val, const HNumber &bits)
{
    if (val.isNan() || !bits.isInteger())
        return HMath::nan();
    int shift = 0;
    int bitLen = 64;
    if (Settings::instance())
        bitLen = Settings::instance()->proBitLength;
    HNumber bitlen(bitLen + 1);
    QString str = format(val, HNumber::Format::Complement() + HNumber::Format::Precision(65) + HNumber::Format::Binary()).remove("0b");
    shift = (bits % bitlen).toInt();
    while (str.length() < bitLen)
        str.push_front("0");
    if (Settings::instance() && !Settings::instance()->proRotateCarry.isEmpty())
        str.push_front(Settings::instance()->proRotateCarry.front());
    else
        str.push_front("0");
    if (shift <= 0) {
        shift = -shift;
        str = str.right(str.length() - shift) + str.left(shift);
        if (Settings::instance() && !Settings::instance()->proRotateCarry.isEmpty())
            Settings::instance()->proRotateCarry.front() = str.front();
        str = "0b" + str.right(bitLen);
        HNumber result(str.toLatin1().data());
        return result;
    } else {
        while (shift-- > 0) {
            str.push_front(str.back());
            str.chop(1);
        }
        if (Settings::instance() && !Settings::instance()->proRotateCarry.isEmpty())
            Settings::instance()->proRotateCarry.front() = str.front();
        str = "0b" + str.right(bitLen);
        HNumber result(str.toLatin1().data());
        return result;
    }
}

// IEEE-754 functions - same logic as old, delegated to basic operations
HNumber HMath::decodeIeee754(const HNumber &val, const HNumber &exp_bits, const HNumber &significand_bits)
{
    return decodeIeee754(val, exp_bits, significand_bits, raise(2, exp_bits - 1) - 1);
}

HNumber HMath::decodeIeee754(const HNumber &val, const HNumber &exp_bits, const HNumber &significand_bits, const HNumber &exp_bias)
{
    if (val.isNan() || exp_bits <= 0 || exp_bits >= 256 || !exp_bits.isInteger() ||
        significand_bits <= 0 || significand_bits >= 256 || !significand_bits.isInteger() ||
        !exp_bias.isInteger())
        return HMath::nan();

    HNumber sign(mask(val >> (exp_bits + significand_bits), 1).isZero() ? 1 : -1);
    HNumber exp = mask(val >> significand_bits, exp_bits);
    HNumber significand = mask(val, significand_bits) * raise(2, -significand_bits);

    if (exp.isZero())
        return sign * significand * raise(2, exp - exp_bias + 1);
    if (exp - raise(2, exp_bits) == -1) {
        if (significand.isZero())
            return HNumber();
        return HMath::nan();
    }
    return sign * (significand + 1) * raise(2, exp - exp_bias);
}

HNumber HMath::encodeIeee754(const HNumber &val, const HNumber &exp_bits, const HNumber &significand_bits)
{
    return encodeIeee754(val, exp_bits, significand_bits, raise(2, exp_bits - 1) - 1);
}

HNumber HMath::encodeIeee754(const HNumber &val, const HNumber &exp_bits, const HNumber &significand_bits, const HNumber &exp_bias)
{
    if (exp_bits <= 0 || exp_bits >= 256 || !exp_bits.isInteger() || significand_bits <= 0 ||
        significand_bits >= 256 || !significand_bits.isInteger() || !exp_bias.isInteger())
        return HMath::nan();

    HNumber sign_bit, significand, exponent;
    HNumber min_exp(1 - exp_bias);
    HNumber max_exp(raise(2, exp_bits) - 2 - exp_bias);

    if (val.isNan()) {
        sign_bit = 0;
        significand = raise(2, significand_bits) - 1;
        exponent = raise(2, exp_bits) - 1;
    } else if (val.isZero()) {
        sign_bit = 0; significand = 0; exponent = 0;
    } else {
        sign_bit = val.isNegative() ? 1 : 0;
        HNumber search_min = min_exp;
        HNumber search_max = max_exp;
        exponent = ceil((search_max - search_min) / 2) + search_min;
        significand = abs(val) * raise(2, -exponent);
        do {
            if (significand >= 1 && significand < 2) break;
            else if (significand >= 2) search_min = exponent + 1;
            else if (significand < 1) search_max = exponent - 1;
            exponent = ceil((search_max - search_min) / 2) + search_min;
            significand = abs(val) * raise(2, -exponent);
        } while (exponent != min_exp && exponent != max_exp);

        HNumber rounded = round(significand * raise(2, significand_bits));
        HNumber intpart = integer(rounded * raise(2, -significand_bits));
        significand = mask(rounded, significand_bits);
        if (intpart.isZero()) exponent = 0;
        else if (intpart > 1) { exponent = raise(2, exp_bits) - 1; significand = 0; }
        else exponent = exponent + exp_bias;
    }

    return sign_bit << (exp_bits + significand_bits) | exponent << significand_bits | significand;
}

std::ostream &operator<<(std::ostream &s, const HNumber &n)
{
    QString str = HMath::format(n);
    s << str.toLatin1().constData();
    return s;
}

HNumber HMath::parse_str(const char *str_in, const char **str_out)
{
    const char *str = str_in;
    HNumber x;

    // Try to parse a number from the beginning of str
    QString input = QString::fromLatin1(str);
    QString trimmed = input.trimmed();

    // Find where the number ends
    int i = 0;
    bool hasDigit = false;
    bool hasDot = false;
    bool hasExp = false;

    // Skip leading whitespace
    while (i < trimmed.length() && trimmed[i].isSpace())
        i++;

    // Skip optional sign
    if (i < trimmed.length() && (trimmed[i] == '+' || trimmed[i] == '-'))
        i++;

    // Check for base prefix
    if (i + 1 < trimmed.length() && trimmed[i] == '0') {
        QChar next = trimmed[i+1].toLower();
        if (next == 'x' || next == 'o' || next == 'b') {
            i += 2;
        }
    }

    // Read digits
    while (i < trimmed.length()) {
        QChar c = trimmed[i];
        if (c.isDigit()) { hasDigit = true; i++; }
        else if (c == '.' && !hasDot && !hasExp) { hasDot = true; i++; }
        else if ((c == 'e' || c == 'E') && !hasExp && hasDigit) {
            hasExp = true;
            i++;
            if (i < trimmed.length() && (trimmed[i] == '+' || trimmed[i] == '-'))
                i++;
        }
        else break;
    }

    if (hasDigit) {
        QString numStr = trimmed.left(i);
        x = HNumber(numStr.toLatin1().constData());
        // Calculate how many chars were consumed from the original string
        int consumed = 0;
        while (consumed < input.length() && consumed < i && input[consumed].isSpace())
            consumed++;
        consumed += i;
        str = str_in + consumed;
    }

    if (str_out)
        *str_out = str;

    return x;
}
