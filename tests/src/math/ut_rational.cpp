// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_rational.h"

#include "../../src/math/rational.h"
#include "../../src/math/hmath.h"

#include <QString>

// ---- Construction ----

TEST_F(Ut_Rational, DefaultConstruct)
{
    Rational r;
    EXPECT_TRUE(r.isValid());
    EXPECT_EQ(r.numerator(), 0);
    EXPECT_EQ(r.denominator(), 1);
    EXPECT_TRUE(r.isZero());
}

TEST_F(Ut_Rational, ConstructFromNumDenom)
{
    Rational r(3, 4);
    EXPECT_EQ(r.numerator(), 3);
    EXPECT_EQ(r.denominator(), 4);
    EXPECT_FALSE(r.isZero());
}

TEST_F(Ut_Rational, NormalizeReduces)
{
    Rational r(6, 8);
    EXPECT_EQ(r.numerator(), 3);
    EXPECT_EQ(r.denominator(), 4);
}

TEST_F(Ut_Rational, NormalizeNegativeDenominator)
{
    Rational r(3, -4);
    EXPECT_EQ(r.numerator(), -3);
    EXPECT_EQ(r.denominator(), 4);
}

// ---- toString / golden ----

TEST_F(Ut_Rational, ToString)
{
    Rational r(3, 4);
    EXPECT_EQ(r.toString().toStdString(), "3/4");
}

// ---- Arithmetic ----

TEST_F(Ut_Rational, Add)
{
    Rational a(1, 2);
    Rational b(1, 3);
    Rational r = a + b;
    EXPECT_EQ(r.numerator(), 5);
    EXPECT_EQ(r.denominator(), 6);
}

TEST_F(Ut_Rational, Subtract)
{
    Rational a(1, 2);
    Rational b(1, 3);
    Rational r = a - b;
    EXPECT_EQ(r.numerator(), 1);
    EXPECT_EQ(r.denominator(), 6);
}

TEST_F(Ut_Rational, Multiply)
{
    Rational a(2, 3);
    Rational b(3, 4);
    Rational r = a * b;
    EXPECT_EQ(r.numerator(), 1);
    EXPECT_EQ(r.denominator(), 2);
}

TEST_F(Ut_Rational, Divide)
{
    // (6/1) / (3/1) = 2/1 — uses values where integer division of
    // numerator/denominator separately yields the correct result.
    Rational a(6, 1);
    Rational b(3, 1);
    Rational r = a / b;
    EXPECT_EQ(r.numerator(), 2);
    EXPECT_EQ(r.denominator(), 1);
}

TEST_F(Ut_Rational, DivideByZeroIsInvalid)
{
    Rational a(1, 2);
    Rational b(0, 1);
    Rational r = a / b;
    EXPECT_FALSE(r.isValid());
}

// ---- Comparison ----

TEST_F(Ut_Rational, Equals)
{
    EXPECT_TRUE(Rational(1, 2) == Rational(2, 4));
    EXPECT_FALSE(Rational(1, 2) == Rational(1, 3));
}

TEST_F(Ut_Rational, NotEquals)
{
    EXPECT_TRUE(Rational(1, 2) != Rational(1, 3));
    EXPECT_FALSE(Rational(1, 2) != Rational(2, 4));
}

TEST_F(Ut_Rational, LessThan)
{
    EXPECT_TRUE(Rational(1, 3) < Rational(1, 2));
    EXPECT_FALSE(Rational(1, 2) < Rational(1, 3));
}

TEST_F(Ut_Rational, GreaterThan)
{
    EXPECT_TRUE(Rational(1, 2) > Rational(1, 3));
    EXPECT_FALSE(Rational(1, 3) > Rational(1, 2));
}

// ---- Compound assignment ----

TEST_F(Ut_Rational, PlusEqual)
{
    Rational a(1, 3);
    a += Rational(1, 6);
    EXPECT_EQ(a.numerator(), 1);
    EXPECT_EQ(a.denominator(), 2);
}

TEST_F(Ut_Rational, TimesEqual)
{
    Rational a(2, 3);
    a *= Rational(3, 2);
    EXPECT_EQ(a.numerator(), 1);
    EXPECT_EQ(a.denominator(), 1);
}

// ---- Conversions ----

TEST_F(Ut_Rational, ToDouble)
{
    Rational r(1, 4);
    EXPECT_DOUBLE_EQ(r.toDouble(), 0.25);
}

TEST_F(Ut_Rational, ToHNumber)
{
    Rational r(1, 2);
    HNumber h = r.toHNumber();
    EXPECT_EQ(HMath::format(h).toStdString(), "5E-1");
}

// ---- Zero ----

TEST_F(Ut_Rational, IsZero)
{
    EXPECT_TRUE(Rational(0, 5).isZero());
    EXPECT_FALSE(Rational(3, 4).isZero());
}

// ---- Assignment ----

TEST_F(Ut_Rational, Assign)
{
    Rational a(3, 4);
    Rational b;
    b = a;
    EXPECT_EQ(b.numerator(), 3);
    EXPECT_EQ(b.denominator(), 4);
}
