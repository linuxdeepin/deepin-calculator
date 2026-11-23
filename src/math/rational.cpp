// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Rational number arithmetic using a continued-fraction approximation
// for conversion from HNumber/double.

#include "rational.h"
#include "hmath.h"
#include "../../3rdparty/core/numberformatter.h"

#include <cmath>
#include <QString>
#include <QStringList>
#include <limits.h>

void Rational::normalize()
{
    if (m_denom == 0) {
        m_valid = false;
        return;
    }
    if (m_num == 0) {
        m_denom = 1;
        return;
    }
    int g = gcd(abs(m_num), abs(m_denom));
    m_num /= g;
    m_denom /= g;
    if (m_denom < 0) {
        m_num = -m_num;
        m_denom = -m_denom;
    }
}

int Rational::compare(const Rational &other) const
{
    return m_num * other.m_denom - m_denom * other.m_num;
}

// Continued-fraction approximation adapted from Python's Fraction module
// (Sjoerd Mullender, Jeffrey Yasskin).
Rational::Rational(const HNumber &num)
    : m_num(1), m_denom(1), m_valid(true)
{
    if (num.isZero()) {
        m_num = 0;
        return;
    }
    if (HMath::abs(num) > HNumber(INT_MAX)
            || HMath::abs(num) < HNumber(1) / HNumber(INT_MAX)) {
        m_valid = false;
        return;
    }
    if (num.isInteger()) {
        m_num = num.toInt();
        return;
    }
    const unsigned long long MAXD = INT_MAX / 2;
    unsigned long long p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    HNumber val(HMath::abs(num));
    while (true) {
        long a = HMath::floor(val).toInt();
        unsigned long long q2 = q0 + a * q1;
        if (q2 > MAXD)
            break;
        unsigned long long t1 = p0, t2 = p1, t3 = q1;
        p0 = t2;
        q0 = t3;
        p1 = t1 + a * t2;
        q1 = q2;
        if (HMath::frac(val).isZero())
            break;
        val = HNumber(1) / HMath::frac(val);
        if (val > HNumber(MAXD))
            break;
    }
    Rational bound(p1, q1);
    if (num < 0)
        bound.m_num = -bound.m_num;
    *this = bound;
}

Rational::Rational(const double &num)
    : m_num(1), m_denom(1), m_valid(true)
{
    if (num == 0) {
        m_num = 0;
        return;
    }
    if (std::abs(num) > INT_MAX || std::abs(1. / num) > INT_MAX) {
        m_valid = false;
        return;
    }
    const long long MAXD = INT_MAX / 2;
    long long p0 = 0, q0 = 1, p1 = 1, q1 = 0;
    double val = fabs(num);
    while (true) {
        unsigned long long a = static_cast<unsigned long long>(floor(val));
        unsigned long long q2 = q0 + a * q1;
        if (q2 > MAXD)
            break;
        unsigned long long t1 = p0, t2 = p1, t3 = q1;
        p0 = t2;
        q0 = t3;
        p1 = t1 + a * t2;
        q1 = q2;
        if (val == a)
            break;
        val = 1 / (val - a);
    }
    Rational bound(p1, q1);
    if (num < 0)
        bound.m_num = -bound.m_num;
    *this = bound;
}

Rational::Rational(const QString &str)
    : m_num(0), m_denom(1), m_valid(true)
{
    if (str.isEmpty()) {
        m_valid = false;
        return;
    }
    QStringList l = str.split('/');
    if (l.size() == 1) {
        bool ok;
        m_num = l.at(0).toInt(&ok);
        if (!ok)
            m_valid = false;
    } else if (l.size() == 2) {
        bool ok;
        m_num = l.at(0).toInt(&ok);
        if (!ok) {
            m_valid = false;
            return;
        }
        m_denom = l.at(1).toInt(&ok);
        if (!ok)
            m_valid = false;
    }
}

Rational Rational::operator*(const Rational &other) const
{
    return Rational(m_num * other.m_num, m_denom * other.m_denom);
}

Rational Rational::operator/(const Rational &other) const
{
    if (other.isZero())
        return Rational(1, 0); // sets m_valid=false via normalize
    return Rational(m_num / other.m_num, m_denom / other.m_denom);
}

Rational Rational::operator+(const Rational &other) const
{
    return Rational(m_num * other.m_denom + m_denom * other.m_num,
                    m_denom * other.m_denom);
}

Rational Rational::operator-(const Rational &other) const
{
    return Rational(m_num * other.m_denom - m_denom * other.m_num,
                    m_denom * other.m_denom);
}

Rational &Rational::operator=(const Rational &other)
{
    m_num = other.m_num;
    m_denom = other.m_denom;
    m_valid = other.m_valid;
    return *this;
}

Rational &Rational::operator+=(const Rational &other) { return *this = *this + other; }
Rational &Rational::operator-=(const Rational &other) { return *this = *this - other; }
Rational &Rational::operator*=(const Rational &other) { return *this = *this * other; }
Rational &Rational::operator/=(const Rational &other) { return *this = *this / other; }

bool Rational::operator<(const Rational &other) const { return compare(other) < 0; }
bool Rational::operator==(const Rational &other) const { return compare(other) == 0; }
bool Rational::operator>(const Rational &other) const { return compare(other) > 0; }

bool Rational::isZero() const { return m_num == 0; }
bool Rational::isValid() const { return m_valid; }

QString Rational::toString() const
{
    if (m_denom == 1)
        return QString::fromLatin1("%1").arg(m_num);
    return QString::fromLatin1("%1/%2").arg(m_num).arg(m_denom);
}

HNumber Rational::toHNumber() const
{
    return HNumber(m_num) / HNumber(m_denom);
}

double Rational::toDouble() const
{
    return double(m_num) / m_denom;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
unsigned int qHash(const Rational &key)
{
    return qHash(key.numerator()) ^ qHash(key.denominator());
}
#else
size_t qHash(const Rational &key, size_t seed) noexcept
{
    return qHash(key.numerator(), seed) ^ qHash(key.denominator(), seed);
}
#endif
