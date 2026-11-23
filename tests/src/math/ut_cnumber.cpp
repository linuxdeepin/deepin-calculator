// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_cnumber.h"

#include "../../src/math/cmath.h"

#include <QString>

static HNumber::Format prec(int p)
{
    return HNumber::Format::General() + HNumber::Format::Precision(p);
}

// ---- Construction ----

TEST_F(Ut_CNumber, DefaultConstructIsZero)
{
    CNumber c;
    EXPECT_TRUE(c.isZero());
    EXPECT_FALSE(c.isNan());
    EXPECT_TRUE(c.isReal());
}

TEST_F(Ut_CNumber, ConstructFromHNumber)
{
    CNumber c(HNumber("7"));
    EXPECT_EQ(HMath::format(c.real).toStdString(), "7E+0");
    EXPECT_TRUE(c.isReal());
    // A real integer is also a Gaussian integer (both parts are integers).
    EXPECT_TRUE(c.isGaussian());
}

TEST_F(Ut_CNumber, ConstructRealImag)
{
    CNumber c(HNumber("3"), HNumber("4"));
    EXPECT_EQ(HMath::format(c.real).toStdString(), "3E+0");
    EXPECT_EQ(HMath::format(c.imag).toStdString(), "4E+0");
    EXPECT_FALSE(c.isReal());
    EXPECT_TRUE(c.isGaussian());
}

TEST_F(Ut_CNumber, ConstructFromInt)
{
    CNumber c(5);
    EXPECT_TRUE(c.isReal());
    EXPECT_EQ(HMath::format(c.real).toStdString(), "5E+0");
}

TEST_F(Ut_CNumber, ConstructFromString)
{
    CNumber c("2.5");
    EXPECT_EQ(HMath::format(c.real).toStdString(), "2.5E+0");
}

// ---- Complex multiplication (golden) ----

TEST_F(Ut_CNumber, ComplexMultiplication)
{
    // (3 + 4i) * (1 + 2i) = (3*1 - 4*2) + (3*2 + 4*1)i = -5 + 10i
    CNumber a(HNumber("3"), HNumber("4"));
    CNumber b(HNumber("1"), HNumber("2"));
    CNumber r = a * b;
    EXPECT_EQ(HMath::format(r.real).toStdString(), "-5E+0");
    EXPECT_EQ(HMath::format(r.imag).toStdString(), "1E+1");
}

// ---- Abs (modulus) ----

TEST_F(Ut_CNumber, Abs3Plus4i)
{
    // |3 + 4i| = 5
    CNumber c(HNumber("3"), HNumber("4"));
    CNumber result = CMath::abs(c);
    EXPECT_EQ(HMath::format(result.real).toStdString(), "5E+0");
}

// ---- Phase ----

TEST_F(Ut_CNumber, Phase1Plus1i)
{
    // arg(1 + i) = pi/4
    CNumber c(HNumber("1"), HNumber("1"));
    CNumber result = CMath::phase(c);
    EXPECT_EQ(HMath::format(result.real, prec(65)).toStdString(),
              "0.78539816339744830961566084581987572104929234984377645524373614807");
}

// ---- Predicates ----

TEST_F(Ut_CNumber, IsRealVsGaussian)
{
    // A purely real integer is both real and Gaussian (imag=0 is an integer).
    EXPECT_TRUE(CNumber(HNumber("5")).isReal());
    EXPECT_TRUE(CNumber(HNumber("5")).isGaussian());
    // A non-integer real is real but not Gaussian.
    EXPECT_TRUE(CNumber(HNumber("2.5")).isReal());
    EXPECT_FALSE(CNumber(HNumber("2.5")).isGaussian());
    // A complex with both parts integer is Gaussian but not real.
    EXPECT_FALSE(CNumber(HNumber("1"), HNumber("1")).isReal());
    EXPECT_TRUE(CNumber(HNumber("1"), HNumber("1")).isGaussian());
}

TEST_F(Ut_CNumber, IsPositiveNegative)
{
    EXPECT_TRUE(CNumber(HNumber("5")).isPositive());
    EXPECT_TRUE(CNumber(HNumber("-5")).isNegative());
}

// ---- Constants via CMath ----

TEST_F(Ut_CNumber, PiConstant)
{
    CNumber pi = CMath::pi();
    EXPECT_EQ(HMath::format(pi.real, prec(65)).toStdString().substr(0, 15),
              HMath::format(HMath::pi(), prec(65)).toStdString().substr(0, 15));
}

// ---- Conjugate ----

TEST_F(Ut_CNumber, Conjugate)
{
    CNumber c(HNumber("3"), HNumber("4"));
    CNumber conj = CMath::conj(c);
    EXPECT_EQ(HMath::format(conj.real).toStdString(), "3E+0");
    EXPECT_EQ(HMath::format(conj.imag).toStdString(), "-4E+0");
}

// ---- Arithmetic ----

TEST_F(Ut_CNumber, AddComplex)
{
    CNumber a(HNumber("1"), HNumber("2"));
    CNumber b(HNumber("3"), HNumber("4"));
    CNumber r = a + b;
    EXPECT_EQ(HMath::format(r.real).toStdString(), "4E+0");
    EXPECT_EQ(HMath::format(r.imag).toStdString(), "6E+0");
}

TEST_F(Ut_CNumber, RealAndImagAccessors)
{
    CNumber c(HNumber("7"), HNumber("9"));
    EXPECT_EQ(HMath::format(CMath::real(c).real).toStdString(), "7E+0");
    EXPECT_EQ(HMath::format(CMath::imag(c).real).toStdString(), "9E+0");
}
