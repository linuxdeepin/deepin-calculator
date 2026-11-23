// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_hnumber.h"

#include "../../src/math/hmath.h"
#include "../../3rdparty/core/errors.h"

#include <QJsonObject>
#include <QString>

// Convenience precision format helpers used throughout the suite.
static HNumber::Format prec(int p)
{
    return HNumber::Format::General() + HNumber::Format::Precision(p);
}

// ---- Construction & predicates ----

TEST_F(Ut_HNumber, DefaultConstructIsNan)
{
    HNumber n;
    EXPECT_TRUE(n.isNan());
    EXPECT_FALSE(n.isZero());
    EXPECT_FALSE(n.isPositive());
    EXPECT_FALSE(n.isNegative());
    EXPECT_FALSE(n.isInteger());
}

TEST_F(Ut_HNumber, ConstructFromInt)
{
    HNumber n(42);
    EXPECT_FALSE(n.isZero());
    EXPECT_TRUE(n.isPositive());
    EXPECT_EQ(n.toInt(), 42);
    EXPECT_EQ(HMath::format(n).toStdString(), "4.2E+1");
}

TEST_F(Ut_HNumber, ConstructFromString)
{
    HNumber n("3.5");
    EXPECT_TRUE(n.isPositive());
    EXPECT_FALSE(n.isInteger());
    EXPECT_EQ(HMath::format(n).toStdString(), "3.5E+0");
}

TEST_F(Ut_HNumber, ConstructNegative)
{
    HNumber n("-5");
    EXPECT_TRUE(n.isNegative());
    EXPECT_FALSE(n.isPositive());
    EXPECT_EQ(HMath::format(n).toStdString(), "-5E+0");
}

TEST_F(Ut_HNumber, IsNan)
{
    HNumber n = HMath::nan(ZeroDivide);
    EXPECT_TRUE(n.isNan());
    EXPECT_EQ(n.error(), ZeroDivide);
}

TEST_F(Ut_HNumber, IsIntegerPredicate)
{
    EXPECT_TRUE(HNumber("7").isInteger());
    EXPECT_FALSE(HNumber("3.7").isInteger());
}

// ---- Constants (golden 78-digit) ----

TEST_F(Ut_HNumber, ConstantOneThird78)
{
    HNumber r = HNumber("1") / HNumber("3");
    EXPECT_EQ(HMath::format(r, prec(78)).toStdString(),
              "0.333333333333333333333333333333333333333333333333333333333333333333333333333333");
}

TEST_F(Ut_HNumber, ConstantPi78)
{
    EXPECT_EQ(HMath::format(HMath::pi(), prec(78)).toStdString(),
              "3.141592653589793238462643383279502884197169399375105820974944592307816406286208");
}

TEST_F(Ut_HNumber, ConstantE78)
{
    EXPECT_EQ(HMath::format(HMath::e(), prec(78)).toStdString(),
              "2.718281828459045235360287471352662497757247093699959574966967627724076630353547");
}

TEST_F(Ut_HNumber, ConstantSqrt278)
{
    EXPECT_EQ(HMath::format(HMath::sqrt(HNumber("2")), prec(78)).toStdString(),
              "1.414213562373095048801688724209698078569671875376948073176679737990732478462107");
}

TEST_F(Ut_HNumber, ConstantPhi78)
{
    EXPECT_EQ(HMath::format(HMath::phi(), prec(78)).toStdString(),
              "1.618033988749894848204586834365638117720309179805762862135448622705260462818902");
}

// ---- Formatting modes ----

TEST_F(Ut_HNumber, FormatGeneralDefault)
{
    EXPECT_EQ(HMath::format(HNumber("3.14159"), HNumber::Format::General()).toStdString(),
              "3.14159E+0");
}

TEST_F(Ut_HNumber, FormatFixedPrecision2)
{
    EXPECT_EQ(HMath::format(HNumber("3.14159"),
                            HNumber::Format::Fixed() + HNumber::Format::Precision(2)).toStdString(),
              "3.14");
}

TEST_F(Ut_HNumber, FormatScientificPrecision3)
{
    EXPECT_EQ(HMath::format(HNumber("12345"),
                            HNumber::Format::Scientific() + HNumber::Format::Precision(3)).toStdString(),
              "1.234E+4");
}

TEST_F(Ut_HNumber, FormatEngineeringPrecision3)
{
    EXPECT_EQ(HMath::format(HNumber("12345"),
                            HNumber::Format::Engineering() + HNumber::Format::Precision(3)).toStdString(),
              "12.34E+3");
}

TEST_F(Ut_HNumber, FormatNegativeGeneral)
{
    EXPECT_EQ(HMath::format(HNumber("-3.14")).toStdString(), "-3.14E+0");
}

// ---- Complement / base formatting ----

TEST_F(Ut_HNumber, ComplementHex255)
{
    EXPECT_EQ(HMath::format(HNumber("255"),
                            HNumber::Format::Complement() + HNumber::Format::Hexadecimal()).toStdString(),
              "0xFF");
}

TEST_F(Ut_HNumber, ComplementOct255)
{
    EXPECT_EQ(HMath::format(HNumber("255"),
                            HNumber::Format::Complement() + HNumber::Format::Precision(65)
                            + HNumber::Format::Octal()).toStdString(),
              "0o377");
}

TEST_F(Ut_HNumber, ComplementBin255)
{
    EXPECT_EQ(HMath::format(HNumber("255"),
                            HNumber::Format::Complement() + HNumber::Format::Precision(65)
                            + HNumber::Format::Binary()).toStdString(),
              "0b11111111");
}

TEST_F(Ut_HNumber, ComplementDecMinus1)
{
    EXPECT_EQ(HMath::format(HNumber("-1"),
                            HNumber::Format::Complement() + HNumber::Format::Precision(65)).toStdString(),
              "-1");
}

// ---- Arithmetic ----

TEST_F(Ut_HNumber, AddOneAndTwo)
{
    EXPECT_EQ(HMath::format(HNumber("1") + HNumber("2")).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Div10By4)
{
    EXPECT_EQ(HMath::format(HNumber("10") / HNumber("4")).toStdString(), "2.5E+0");
}

TEST_F(Ut_HNumber, Mod10By3)
{
    EXPECT_EQ(HMath::format(HNumber("10") % HNumber("3")).toStdString(), "1E+0");
}

TEST_F(Ut_HNumber, Sqrt9)
{
    EXPECT_EQ(HMath::format(HMath::sqrt(HNumber("9"))).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Cbrt27)
{
    EXPECT_EQ(HMath::format(HMath::cbrt(HNumber("27"))).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Floor37)
{
    EXPECT_EQ(HMath::format(HMath::floor(HNumber("3.7"))).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Ceil37)
{
    EXPECT_EQ(HMath::format(HMath::ceil(HNumber("3.7"))).toStdString(), "4E+0");
}

TEST_F(Ut_HNumber, AbsMinus5)
{
    EXPECT_EQ(HMath::format(HMath::abs(HNumber("-5"))).toStdString(), "5E+0");
}

TEST_F(Ut_HNumber, Integer37)
{
    EXPECT_EQ(HMath::format(HMath::integer(HNumber("3.7"))).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Frac37)
{
    EXPECT_EQ(HMath::format(HMath::frac(HNumber("3.7"))).toStdString(), "7E-1");
}

TEST_F(Ut_HNumber, Lb8)
{
    EXPECT_EQ(HMath::format(HMath::lb(HNumber("8"))).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, Exp0)
{
    EXPECT_EQ(HMath::format(HMath::exp(HNumber("0"))).toStdString(), "1E+0");
}

TEST_F(Ut_HNumber, Ln1)
{
    EXPECT_EQ(HMath::format(HMath::ln(HNumber("1"))).toStdString(), "0E+0");
}

TEST_F(Ut_HNumber, Gcd12And8)
{
    EXPECT_EQ(HMath::format(HMath::gcd(HNumber("12"), HNumber("8"))).toStdString(), "4E+0");
}

TEST_F(Ut_HNumber, Raise2To10)
{
    EXPECT_EQ(HMath::format(HMath::raise(HNumber("2"), 10)).toStdString(), "1.024E+3");
}

TEST_F(Ut_HNumber, Factorial5)
{
    EXPECT_EQ(HMath::format(HMath::factorial(HNumber("5"))).toStdString(), "1.2E+2");
}

TEST_F(Ut_HNumber, Ncr5Choose2)
{
    EXPECT_EQ(HMath::format(HMath::nCr(HNumber("5"), HNumber("2"))).toStdString(), "1E+1");
}

// ---- Trig (degree inputs) ----

TEST_F(Ut_HNumber, Sin30)
{
    EXPECT_EQ(HMath::format(HMath::sin(HMath::deg2rad(HNumber("30"))), prec(65)).toStdString(), "0.5");
}

TEST_F(Ut_HNumber, Cos60)
{
    EXPECT_EQ(HMath::format(HMath::cos(HMath::deg2rad(HNumber("60"))), prec(65)).toStdString(), "0.5");
}

TEST_F(Ut_HNumber, Tan45)
{
    EXPECT_EQ(HMath::format(HMath::tan(HMath::deg2rad(HNumber("45"))), prec(65)).toStdString(), "1");
}

// ---- Higher math: erf, erfc, gamma, lngamma, arctan ----

TEST_F(Ut_HNumber, Erf1)
{
    EXPECT_EQ(HMath::format(HMath::erf(HNumber("1")), prec(65)).toStdString(),
              "0.84270079294971486934122063508260925929606699796630290845993789783");
}

TEST_F(Ut_HNumber, Erfc1)
{
    EXPECT_EQ(HMath::format(HMath::erfc(HNumber("1")), prec(65)).toStdString(),
              "0.15729920705028513065877936491739074070393300203369709154006210216");
}

TEST_F(Ut_HNumber, Erf0)
{
    EXPECT_EQ(HMath::format(HMath::erf(HNumber("0")), prec(65)).toStdString(), "0");
}

TEST_F(Ut_HNumber, Gamma1)
{
    EXPECT_EQ(HMath::format(HMath::gamma(HNumber("1")), prec(65)).toStdString(), "1");
}

TEST_F(Ut_HNumber, LnGamma1)
{
    EXPECT_EQ(HMath::format(HMath::lnGamma(HNumber("1")), prec(65)).toStdString(), "0");
}

TEST_F(Ut_HNumber, GammaHalf)
{
    EXPECT_EQ(HMath::format(HMath::gamma(HNumber("0.5")), prec(65)).toStdString(),
              "1.77245385090551602729816748334114518279754945612238712821380778985");
}

TEST_F(Ut_HNumber, Arctan1)
{
    EXPECT_EQ(HMath::format(HMath::arctan(HNumber("1")), prec(65)).toStdString(),
              "0.78539816339744830961566084581987572104929234984377645524373614807");
}

TEST_F(Ut_HNumber, Sinh0)
{
    EXPECT_EQ(HMath::format(HMath::sinh(HNumber("0")), prec(65)).toStdString(), "0");
}

// ---- Bitwise ----

TEST_F(Ut_HNumber, BitAnd12And10)
{
    EXPECT_EQ(HMath::format(HNumber("12") & HNumber("10")).toStdString(), "8E+0");
}

TEST_F(Ut_HNumber, BitOr12And10)
{
    EXPECT_EQ(HMath::format(HNumber("12") | HNumber("10")).toStdString(), "1.4E+1");
}

TEST_F(Ut_HNumber, BitXor12And10)
{
    EXPECT_EQ(HMath::format(HNumber("12") ^ HNumber("10")).toStdString(), "6E+0");
}

TEST_F(Ut_HNumber, ShiftLeft1By4)
{
    EXPECT_EQ(HMath::format(HNumber("1") << HNumber("4")).toStdString(), "1.6E+1");
}

TEST_F(Ut_HNumber, ShiftRight256By4)
{
    EXPECT_EQ(HMath::format(HNumber("256") >> HNumber("4")).toStdString(), "1.6E+1");
}

// ---- Comparison ----

TEST_F(Ut_HNumber, Comparisons)
{
    EXPECT_TRUE(HNumber("3") > HNumber("2"));
    EXPECT_TRUE(HNumber("2") < HNumber("3"));
    EXPECT_TRUE(HNumber("3") >= HNumber("3"));
    EXPECT_TRUE(HNumber("3") <= HNumber("3"));
    EXPECT_TRUE(HNumber("3") == HNumber("3"));
    EXPECT_TRUE(HNumber("3") != HNumber("4"));
}

// ---- Parsing with base prefixes ----

TEST_F(Ut_HNumber, ParseHex)
{
    EXPECT_EQ(HMath::format(HNumber("0xFF", true)).toStdString(), "2.55E+2");
}

TEST_F(Ut_HNumber, ParseOct)
{
    EXPECT_EQ(HMath::format(HNumber("0o17", true)).toStdString(), "1.5E+1");
}

TEST_F(Ut_HNumber, ParseBin)
{
    EXPECT_EQ(HMath::format(HNumber("0b1010", true)).toStdString(), "1E+1");
}

TEST_F(Ut_HNumber, ParseStrStopsAtInvalid)
{
    const char *end = nullptr;
    HNumber parsed = HMath::parse_str("42abc", &end);
    EXPECT_EQ(HMath::format(parsed).toStdString(), "4.2E+1");
    ASSERT_NE(end, nullptr);
    EXPECT_EQ(std::string(end), "abc");
}

// ---- Serialize / deserialize ----

TEST_F(Ut_HNumber, SerializeRoundtrip)
{
    HNumber original("3.14159");
    QJsonObject json;
    original.serialize(json);
    HNumber restored = HNumber::deSerialize(json);
    EXPECT_EQ(HMath::format(restored,
                            HNumber::Format::Fixed() + HNumber::Format::Precision(5)).toStdString(),
              "3.14159");
}

// ---- Copy & assignment ----

TEST_F(Ut_HNumber, CopyConstruct)
{
    HNumber a("7.5");
    HNumber b(a);
    EXPECT_EQ(HMath::format(b).toStdString(), "7.5E+0");
}

TEST_F(Ut_HNumber, Assign)
{
    HNumber a("1.25");
    HNumber b;
    b = a;
    EXPECT_EQ(HMath::format(b).toStdString(), "1.25E+0");
}

// ---- Compound assignment ----

TEST_F(Ut_HNumber, PlusEqual)
{
    HNumber a("1");
    a += HNumber("2");
    EXPECT_EQ(HMath::format(a).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, MinusEqual)
{
    HNumber a("5");
    a -= HNumber("2");
    EXPECT_EQ(HMath::format(a).toStdString(), "3E+0");
}

TEST_F(Ut_HNumber, TimesEqual)
{
    HNumber a("3");
    a *= HNumber("4");
    EXPECT_EQ(HMath::format(a).toStdString(), "1.2E+1");
}

TEST_F(Ut_HNumber, DivEqual)
{
    HNumber a("10");
    a /= HNumber("4");
    EXPECT_EQ(HMath::format(a).toStdString(), "2.5E+0");
}
