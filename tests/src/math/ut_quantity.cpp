// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_quantity.h"

#include "../../src/math/quantity.h"
#include "../../src/math/hmath.h"

#include <QString>

static Quantity::Format qprec(int p)
{
    return Quantity::Format::General() + Quantity::Format::Precision(p);
}

// ---- Construction ----

TEST_F(Ut_Quantity, DefaultConstructIsZero)
{
    Quantity q;
    EXPECT_TRUE(q.isZero());
    EXPECT_FALSE(q.isNan());
    EXPECT_TRUE(q.isReal());
    EXPECT_FALSE(q.hasUnit());
    EXPECT_FALSE(q.hasDimension());
    EXPECT_TRUE(q.isDimensionless());
}

TEST_F(Ut_Quantity, ConstructFromInt)
{
    Quantity q(42);
    EXPECT_EQ(DMath::format(q).toStdString(), "4.2E+1");
    EXPECT_TRUE(q.isInteger());
}

TEST_F(Ut_Quantity, ConstructFromHNumber)
{
    Quantity q(HNumber("3.5"));
    EXPECT_EQ(DMath::format(q).toStdString(), "3.5E+0");
    EXPECT_FALSE(q.isInteger());
}

TEST_F(Ut_Quantity, ConstructFromCNumber)
{
    Quantity q(CNumber(HNumber("2"), HNumber("3")));
    EXPECT_FALSE(q.isReal());
    EXPECT_FALSE(q.isZero());
}

// ---- Format ----

TEST_F(Ut_Quantity, FormatGeneralPi78)
{
    Quantity q(HMath::pi());
    EXPECT_EQ(DMath::format(q, qprec(78)).toStdString(),
              "3.141592653589793238462643383279502884197169399375105820974944592307816406286208");
}

TEST_F(Ut_Quantity, FormatComplementHex255)
{
    Quantity q(HNumber("255"));
    EXPECT_EQ(DMath::format(q,
                            Quantity::Format::Complement() + Quantity::Format::Hexadecimal()).toStdString(),
              "0xFF");
}

// ---- Complement mode ----

TEST_F(Ut_Quantity, ComplementOctal)
{
    Quantity q(HNumber("8"));
    EXPECT_EQ(DMath::format(q,
                            Quantity::Format::Complement() + Quantity::Format::Precision(65)
                            + Quantity::Format::Octal()).toStdString(),
              "0o10");
}

// ---- Predicates ----

TEST_F(Ut_Quantity, IsPositiveNegative)
{
    EXPECT_TRUE(Quantity(HNumber("5")).isPositive());
    EXPECT_TRUE(Quantity(HNumber("-5")).isNegative());
}

TEST_F(Ut_Quantity, IsEven)
{
    EXPECT_TRUE(Quantity(4).isEven());
    EXPECT_FALSE(Quantity(3).isEven());
}

// ---- Dimension ----

TEST_F(Ut_Quantity, DimensionlessByDefault)
{
    Quantity q(HNumber("1"));
    EXPECT_TRUE(q.isDimensionless());
    EXPECT_TRUE(q.getDimension().isEmpty());
}

TEST_F(Ut_Quantity, SameDimension)
{
    Quantity a(HNumber("1"));
    Quantity b(HNumber("2"));
    EXPECT_TRUE(a.sameDimension(b));
}

// ---- Arithmetic ----

TEST_F(Ut_Quantity, AddQuantities)
{
    Quantity a(HNumber("1"));
    Quantity b(HNumber("2"));
    Quantity r = a + b;
    EXPECT_EQ(DMath::format(r).toStdString(), "3E+0");
}

TEST_F(Ut_Quantity, MultiplyQuantities)
{
    Quantity a(HNumber("3"));
    Quantity b(HNumber("4"));
    Quantity r = a * b;
    EXPECT_EQ(DMath::format(r).toStdString(), "1.2E+1");
}

TEST_F(Ut_Quantity, AbsQuantity)
{
    Quantity q(HNumber("-7"));
    Quantity r = DMath::abs(q);
    EXPECT_EQ(DMath::format(r).toStdString(), "7E+0");
}

// ---- Constants ----

TEST_F(Ut_Quantity, PiConstant)
{
    Quantity pi = DMath::pi();
    EXPECT_EQ(DMath::format(pi, qprec(78)).toStdString(),
              "3.141592653589793238462643383279502884197169399375105820974944592307816406286208");
}

TEST_F(Ut_Quantity, EConstant)
{
    Quantity e = DMath::e();
    EXPECT_EQ(DMath::format(e, qprec(78)).toStdString(),
              "2.718281828459045235360287471352662497757247093699959574966967627724076630353547");
}
