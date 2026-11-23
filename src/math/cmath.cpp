// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Complex number type and functions. CNumber wraps two HNumber values
// (real, imag). CMath delegates real-valued math to HMath and applies
// standard complex identities for the rest.

#include "cmath.h"

#include "cnumberparser.h"
#include "hmath.h"
#include "floatconfig.h"

#include <QString>
#include <QStringList>

#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// CNumber
// ---------------------------------------------------------------------------

CNumber::CNumber()
    : real(0), imag(0) {}

CNumber::CNumber(const HNumber &hn)
    : real(hn), imag(0) {}

CNumber::CNumber(const HNumber &x, const HNumber &y)
    : real(x), imag(y) {}

CNumber::CNumber(const CNumber &cn)
    : real(cn.real), imag(cn.imag) {}

CNumber::CNumber(int i)
    : real(i), imag(0) {}

CNumber::CNumber(const char *str)
{
    CNumberParser parser(str);
    parser.parse(this);
}

CNumber::CNumber(const QJsonObject &json)
{
    *this = deSerialize(json);
}

bool CNumber::isNan() const { return real.isNan() || imag.isNan(); }
bool CNumber::isZero() const { return real.isZero() && imag.isZero(); }
bool CNumber::isPositive() const { return real.isPositive() && imag.isZero(); }
bool CNumber::isNegative() const { return real.isNegative() && imag.isZero(); }
bool CNumber::isInteger() const { return real.isInteger() && imag.isZero(); }
bool CNumber::isGaussian() const { return real.isInteger() && imag.isInteger(); }
bool CNumber::isReal() const { return imag.isZero(); }
bool CNumber::isNearReal() const { return imag.isNearZero(); }

void CNumber::serialize(QJsonObject &json) const
{
    json["value"] = CMath::format(*this, CNumber::Format::Fixed() + CNumber::Format::Precision(DECPRECISION));
}

CNumber CNumber::deSerialize(const QJsonObject &json)
{
    CNumber result;
    if (json.contains("value")) {
        QString str = json["value"].toString();
        str.replace(",", ".");
        result = CNumber(str.toLatin1().constData());
    }
    return result;
}

Error CNumber::error() const { return real.error(); }

CNumber &CNumber::operator=(const CNumber &cn)
{
    real = cn.real;
    imag = cn.imag;
    return *this;
}

CNumber CNumber::operator+(const CNumber &num) const
{
    return CNumber(real + num.real, imag + num.imag);
}

CNumber &CNumber::operator+=(const CNumber &num) { return *this = *this + num; }

CNumber operator-(const CNumber &n1, const CNumber &n2)
{
    return CNumber(n1.real - n2.real, n1.imag - n2.imag);
}

CNumber &CNumber::operator-=(const CNumber &num) { return *this = *this - num; }

CNumber operator-(const CNumber &x)
{
    return CNumber(-x.real, -x.imag);
}

// (a+bi)(c+di) = (ac-bd) + (ad+bc)i
CNumber CNumber::operator*(const CNumber &num) const
{
    return CNumber(real * num.real - imag * num.imag,
                   imag * num.real + real * num.imag);
}

CNumber CNumber::operator*(const HNumber &num) const
{
    return CNumber(real * num, imag * num);
}

CNumber &CNumber::operator*=(const CNumber &num) { return *this = *this * num; }

// (a+bi)/(c+di) = ((ac+bd) + (bc-ad)i) / (c²+d²)
CNumber CNumber::operator/(const CNumber &num) const
{
    if (num.isZero())
        return CMath::nan(ZeroDivide);
    HNumber divider = num.real * num.real + num.imag * num.imag;
    return CNumber((real * num.real + imag * num.imag) / divider,
                   (imag * num.real - real * num.imag) / divider);
}

CNumber CNumber::operator/(const HNumber &num) const
{
    if (num.isZero())
        return CMath::nan(ZeroDivide);
    return CNumber(real / num, imag / num);
}

CNumber &CNumber::operator/=(const CNumber &num) { return *this = *this / num; }

int CNumber::compare(const CNumber &other) const
{
    // Complex numbers are not an ordered field; only compare reals.
    if (isReal() && other.isReal())
        return real.compare(other.real);
    return false;
}

bool operator>(const CNumber &l, const CNumber &r) { return l.compare(r) > 0; }
bool operator<(const CNumber &l, const CNumber &r) { return l.compare(r) < 0; }
bool operator>=(const CNumber &l, const CNumber &r) { return l.compare(r) >= 0; }
bool operator<=(const CNumber &l, const CNumber &r) { return l.compare(r) <= 0; }
bool operator==(const CNumber &l, const CNumber &r) { return l.compare(r) == 0; }
bool operator!=(const CNumber &l, const CNumber &r) { return l.compare(r) != 0; }

// ---------------------------------------------------------------------------
// CNumber::Format
// ---------------------------------------------------------------------------

CNumber::Format::Format()
    : HNumber::Format(), notation(Notation::Null) {}

CNumber::Format::Format(const Format &other)
    : HNumber::Format(static_cast<const HNumber::Format &>(other))
    , notation(other.notation) {}

CNumber::Format::Format(const HNumber::Format &other)
    : HNumber::Format(other), notation(Notation::Null) {}

CNumber::Format CNumber::Format::operator+(const CNumber::Format &other) const
{
    Format result(HNumber::Format::operator+(static_cast<const HNumber::Format &>(other)));
    result.notation = (this->notation != Notation::Null) ? this->notation : other.notation;
    return result;
}

CNumber::Format CNumber::Format::Polar()
{
    Format result;
    result.notation = Notation::Polar;
    return result;
}

CNumber::Format CNumber::Format::Cartesian()
{
    Format result;
    result.notation = Notation::Cartesian;
    return result;
}

// ---------------------------------------------------------------------------
// CMath
// ---------------------------------------------------------------------------

CNumber CMath::nan(Error error)
{
    // Keep both parts NaN with the same error.
    CNumber result;
    result.real = HMath::nan(error);
    result.imag = HMath::nan(error);
    return result;
}

CNumber CMath::e()  { return CNumber(HMath::e()); }
CNumber CMath::pi() { return CNumber(HMath::pi()); }
CNumber CMath::phi(){ return CNumber(HMath::phi()); }
CNumber CMath::i()  { return CNumber(0, 1); }

QString CMath::format(const CNumber &cn, CNumber::Format format)
{
    if (cn.isNan())
        return "NaN";
    if (cn.imag.isNearZero())
        return HMath::format(cn.real, format);

    if (format.notation == CNumber::Format::Notation::Polar) {
        QString strRadius = HMath::format(CMath::abs(cn).real, format);
        HNumber phase = CMath::phase(cn).real;
        if (phase.isZero())
            return strRadius;
        QString strPhase = HMath::format(phase, format);
        return QString("%1 * exp(j*%2)").arg(strRadius, strPhase);
    }

    // Cartesian: a+jb (omit real part when zero, use '-' for negative imag).
    QString real_part = cn.real.isZero() ? QString() : HMath::format(cn.real, format);
    QString imag_part;
    QString separator;
    const QString postfix = "j";

    if (cn.imag.isPositive()) {
        separator = cn.real.isZero() ? QString() : QString("+");
        imag_part = HMath::format(cn.imag, format);
    } else {
        separator = "-";
        imag_part = HMath::format(-cn.imag, format);
    }
    return real_part + separator + imag_part + postfix;
}

// |a+bi| = sqrt(a²+b²)
CNumber CMath::abs(const CNumber &n)
{
    return HMath::sqrt(n.real * n.real + n.imag * n.imag);
}

CNumber CMath::conj(const CNumber &n)
{
    return CNumber(n.real, -n.imag);
}

// sqrt via the identity on wikipedia: Square_root#Square_roots_of_negative_and_complex_numbers
CNumber CMath::sqrt(const CNumber &n)
{
    HNumber s = (n.imag.isPositive() || n.imag.isZero()) ? 1 : -1;
    return CNumber(HMath::sqrt((abs(n).real + n.real) / 2),
                   s * HMath::sqrt((abs(n).real - n.real) / 2));
}

CNumber CMath::raise(const CNumber &n1, int n)
{
    return CMath::exp(CMath::ln(n1) * n);
}

CNumber CMath::raise(const CNumber &n1, const CNumber &n2)
{
    if (n1.isZero() && (n2.real > 0))
        return CNumber(0);
    return CMath::exp(CMath::ln(n1) * n2);
}

// e^(a+bi) = e^a (cos b + i sin b)
CNumber CMath::exp(const CNumber &x)
{
    HNumber a = HMath::exp(x.real);
    return CNumber(a * HMath::cos(x.imag), a * HMath::sin(x.imag));
}

CNumber CMath::ln(const CNumber &x)
{
    CNumber result;
    if (x.imag.isZero()) {
        if (x.real.isZero())
            return CMath::nan(OutOfDomain);
        result.real = HMath::ln(HMath::abs(x.real));
        if (result.real.isNan()) {
            result.imag = result.real;
            return result;
        }
        result.imag = x.real.isPositive() ? HNumber(0) : HMath::pi();
    } else {
        HNumber a = CMath::abs(x).real;
        result.real = HMath::ln(a);
        // Principal value: arccos(re/|z|)
        auto imag = HMath::arccos(x.real / a);
        result.imag = (x.imag.isPositive() || x.imag.isZero()) ? imag : -imag;
    }
    return result;
}

CNumber CMath::lg(const CNumber &x) { return CMath::ln(x) / HMath::ln(10); }
CNumber CMath::lb(const CNumber &x) { return CMath::ln(x) / HMath::ln(2); }
CNumber CMath::log(const CNumber &base, const CNumber &x) { return CMath::ln(x) / CMath::ln(base); }

// sin(a+bi) = sin a cosh b + i cos a sinh b
CNumber CMath::sin(const CNumber &x)
{
    return CNumber(HMath::sin(x.real) * HMath::cosh(x.imag),
                   HMath::cos(x.real) * HMath::sinh(x.imag));
}

// cos(a+bi) = cos a cosh b - i sin a sinh b
CNumber CMath::cos(const CNumber &x)
{
    return CNumber(HMath::cos(x.real) * HMath::cosh(x.imag),
                   -HMath::sin(x.real) * HMath::sinh(x.imag));
}

CNumber CMath::tan(const CNumber &x) { return CMath::sin(x) / CMath::cos(x); }
CNumber CMath::cot(const CNumber &x) { return CMath::cos(x) / CMath::sin(x); }
CNumber CMath::sec(const CNumber &x) { return CNumber(1) / CMath::cos(x); }
CNumber CMath::csc(const CNumber &x) { return CNumber(1) / CMath::sin(x); }

CNumber CMath::sinh(const CNumber &x) { return (exp(x) - exp(-x)) / HNumber(2); }
CNumber CMath::cosh(const CNumber &x) { return (exp(x) + exp(-x)) / HNumber(2); }
CNumber CMath::tanh(const CNumber &x) { return sinh(x) / cosh(x); }

CNumber CMath::arsinh(const CNumber &x) { return CMath::ln(x + CMath::sqrt(x * x + CNumber(1))); }
CNumber CMath::arcosh(const CNumber &x)
{
    return CMath::ln(x + CMath::sqrt(x + CNumber(1)) * CMath::sqrt(x - CNumber(1)));
}
CNumber CMath::artanh(const CNumber &x)
{
    return (CNumber("0.5") * CMath::ln(CNumber(1) + x))
         - (CNumber("0.5") * CMath::ln(CNumber(1) - x));
}

CNumber CMath::phase(const CNumber &x) { return HMath::arctan2(x.real, x.imag); }

CNumber CMath::arctan(const CNumber &x)
{
    return CMath::i() * (CMath::ln(CNumber(1) - CMath::i() * x)
                         - CMath::ln(CNumber(1) + CMath::i() * x)) / 2;
}

CNumber CMath::arcsin(const CNumber &x)
{
    return -CMath::i() * CMath::ln(CMath::i() * x + sqrt(CNumber(1) - x * x));
}

CNumber CMath::arccos(const CNumber &x)
{
    return -CMath::i() * CMath::ln(x + sqrt(x * x - CNumber(1)));
}

CNumber CMath::rad2deg(const CNumber &x)
{
    return CNumber(HMath::rad2deg(x.real), HMath::rad2deg(x.imag));
}
CNumber CMath::deg2rad(const CNumber &x)
{
    return CNumber(HMath::deg2rad(x.real), HMath::deg2rad(x.imag));
}
CNumber CMath::rad2gon(const CNumber &x)
{
    return CNumber(HMath::rad2gon(x.real), HMath::rad2gon(x.imag));
}
CNumber CMath::gon2rad(const CNumber &x)
{
    return CNumber(HMath::gon2rad(x.real), HMath::gon2rad(x.imag));
}

// ---------------------------------------------------------------------------
// Real-only wrappers
//
// The following functions are only defined for real numbers. When given a
// complex argument they return NaN with the specified error. Each delegates
// to the corresponding HNumber method after extracting the real part.
// ---------------------------------------------------------------------------

#define ENSURE_REAL(number, error) \
    if ((number).isNan() || !(number).isNearReal()) \
        return CMath::nan(error);

#define REAL_WRAPPER_CNUMBER_1(fct, error) \
    CNumber CNumber::fct() const \
    { \
        ENSURE_REAL(*this, error); \
        return CNumber(this->real.fct()); \
    }

#define REAL_WRAPPER_CNUMBER_2(fct, error) \
    CNumber CNumber::fct(const CNumber &x) const \
    { \
        ENSURE_REAL(*this, error); \
        ENSURE_REAL(x, error); \
        return CNumber(this->real.fct(x.real)); \
    }

#define REAL_WRAPPER_CNUMBER_3(fct, error) \
    CNumber &CNumber::fct(const CNumber &x) \
    { \
        if (!this->isReal() || !x.isReal()) { \
            *this = CMath::nan(error); \
            return *this; \
        } \
        this->real.fct(x.real); \
        return *this; \
    }

#define REAL_WRAPPER_CNUMBER_4(fct, error) \
    int CNumber::fct() const \
    { \
        if (!this->isNearReal()) \
            return 0; \
        return this->real.fct(); \
    }

#define REAL_WRAPPER_CMATH_NUM(fct, error) \
    CNumber CMath::fct(const CNumber &x) \
    { \
        ENSURE_REAL(x, error); \
        return CNumber(HMath::fct(x.real)); \
    }

#define REAL_WRAPPER_CMATH_NUM_NUM(fct, error) \
    CNumber CMath::fct(const CNumber &x1, const CNumber &x2) \
    { \
        ENSURE_REAL(x1, error); \
        ENSURE_REAL(x2, error); \
        return CNumber(HMath::fct(x1.real, x2.real)); \
    }

#define REAL_WRAPPER_CMATH_NUM_INT(fct, error) \
    CNumber CMath::fct(const CNumber &x1, int n) \
    { \
        ENSURE_REAL(x1, error); \
        return CNumber(HMath::fct(x1.real, n)); \
    }

#define REAL_WRAPPER_CMATH_NUM_NUM_NUM(fct, error) \
    CNumber CMath::fct(const CNumber &x1, const CNumber &x2, const CNumber &x3) \
    { \
        ENSURE_REAL(x1, error); \
        ENSURE_REAL(x2, error); \
        ENSURE_REAL(x3, error); \
        return CNumber(HMath::fct(x1.real, x2.real, x3.real)); \
    }

#define REAL_WRAPPER_CMATH_NUM_NUM_NUM_NUM(fct, error) \
    CNumber CMath::fct(const CNumber &x1, const CNumber &x2, const CNumber &x3, const CNumber &x4) \
    { \
        ENSURE_REAL(x1, error); \
        ENSURE_REAL(x2, error); \
        ENSURE_REAL(x3, error); \
        ENSURE_REAL(x4, error); \
        return CNumber(HMath::fct(x1.real, x2.real, x3.real, x4.real)); \
    }

// CNumber member operators that require real operands.
REAL_WRAPPER_CNUMBER_4(toInt, OutOfDomain)
REAL_WRAPPER_CNUMBER_2(operator%, OutOfDomain)
REAL_WRAPPER_CNUMBER_2(operator&, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_3(operator&=, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_2(operator|, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_3(operator|=, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_2(operator^, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_3(operator^=, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_1(operator~, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_2(operator>>, OutOfLogicRange)
REAL_WRAPPER_CNUMBER_2(operator<<, OutOfLogicRange)

// CMath general math (real-only).
REAL_WRAPPER_CMATH_NUM(integer, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(frac, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(floor, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(ceil, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(gcd, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(idiv, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_INT(round, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_INT(trunc, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(cbrt, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(sgn, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(arctan2, OutOfDomain)

// CMath higher math (real-only).
REAL_WRAPPER_CMATH_NUM_NUM(factorial, NotImplemented)
REAL_WRAPPER_CMATH_NUM(erf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(erfc, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(gamma, NotImplemented)
REAL_WRAPPER_CMATH_NUM(lnGamma, NotImplemented)

// CMath probability (real-only).
REAL_WRAPPER_CMATH_NUM_NUM(nCr, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(nPr, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM(binomialPmf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM(binomialCdf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(binomialMean, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(binomialVariance, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM_NUM(hypergeometricPmf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM_NUM(hypergeometricCdf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM(hypergeometricMean, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM(hypergeometricVariance, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(poissonPmf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM(poissonCdf, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(poissonMean, OutOfDomain)
REAL_WRAPPER_CMATH_NUM(poissonVariance, OutOfDomain)

// CMath logic (real-only).
REAL_WRAPPER_CMATH_NUM_NUM(mask, OutOfLogicRange)
REAL_WRAPPER_CMATH_NUM_NUM(sgnext, OutOfLogicRange)
REAL_WRAPPER_CMATH_NUM_NUM(ashr, OutOfLogicRange)
REAL_WRAPPER_CMATH_NUM_NUM(lshr, OutOfLogicRange)
REAL_WRAPPER_CMATH_NUM_NUM(rosh, OutOfLogicRange)
REAL_WRAPPER_CMATH_NUM_NUM(rcsh, OutOfLogicRange)

// CMath IEEE-754 (real-only).
REAL_WRAPPER_CMATH_NUM_NUM_NUM(decodeIeee754, OutOfDomain)
REAL_WRAPPER_CMATH_NUM_NUM_NUM_NUM(decodeIeee754, OutOfDomain)

CNumber CMath::encodeIeee754(const CNumber &val, const CNumber &exp_bits,
                             const CNumber &significand_bits)
{
    ENSURE_REAL(exp_bits, OutOfDomain);
    ENSURE_REAL(significand_bits, OutOfDomain);
    if (!val.isNan() && !val.isNearReal())
        return CMath::nan(OutOfDomain);
    return CNumber(HMath::encodeIeee754(val.real, exp_bits.real, significand_bits.real));
}

CNumber CMath::encodeIeee754(const CNumber &val, const CNumber &exp_bits,
                             const CNumber &significand_bits, const CNumber &exp_bias)
{
    ENSURE_REAL(exp_bits, OutOfDomain);
    ENSURE_REAL(significand_bits, OutOfDomain);
    ENSURE_REAL(exp_bias, OutOfDomain);
    if (!val.isNan() && !val.isNearReal())
        return CMath::nan(OutOfDomain);
    return CNumber(HMath::encodeIeee754(val.real, exp_bits.real, significand_bits.real, exp_bias.real));
}

std::ostream &operator<<(std::ostream &os, const CNumber &cn)
{
    os << CMath::format(cn).toStdString();
    return os;
}
