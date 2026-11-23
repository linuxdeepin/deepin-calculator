// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ut_units.h"

#include "../../src/math/units.h"
#include "../../src/math/quantity.h"
#include "../../src/math/hmath.h"

#include <QString>

// ---- Base SI units ----

TEST_F(Ut_Units, BaseUnitsExist)
{
    Quantity m = Units::meter();
    EXPECT_FALSE(m.isNan());
    EXPECT_FALSE(m.isZero());

    Quantity s = Units::second();
    EXPECT_FALSE(s.isNan());

    Quantity kg = Units::kilogram();
    EXPECT_FALSE(kg.isNan());

    Quantity a = Units::ampere();
    EXPECT_FALSE(a.isNan());

    Quantity mol = Units::mole();
    EXPECT_FALSE(mol.isNan());

    Quantity k = Units::kelvin();
    EXPECT_FALSE(k.isNan());

    Quantity cd = Units::candela();
    EXPECT_FALSE(cd.isNan());
}

TEST_F(Ut_Units, BitUnit)
{
    Quantity b = Units::bit();
    EXPECT_FALSE(b.isNan());
}

// ---- SI prefixes ----

TEST_F(Ut_Units, PrefixKilo)
{
    Quantity k = Units::kilo();
    EXPECT_EQ(DMath::format(k).toStdString(), "1E+3");
}

TEST_F(Ut_Units, PrefixMilli)
{
    Quantity m = Units::milli();
    EXPECT_EQ(DMath::format(m).toStdString(), "1E-3");
}

TEST_F(Ut_Units, PrefixCenti)
{
    Quantity c = Units::centi();
    EXPECT_EQ(DMath::format(c).toStdString(), "1E-2");
}

TEST_F(Ut_Units, PrefixMega)
{
    Quantity m = Units::mega();
    EXPECT_EQ(DMath::format(m).toStdString(), "1E+6");
}

// ---- Binary prefixes ----

TEST_F(Ut_Units, PrefixKibi)
{
    Quantity k = Units::kibi();
    EXPECT_EQ(DMath::format(k).toStdString(), "1.024E+3");
}

TEST_F(Ut_Units, PrefixMebi)
{
    Quantity m = Units::mebi();
    // 1024^2 = 1048576
    EXPECT_EQ(DMath::format(m).toStdString(), "1.048576E+6");
}

// ---- Derived units ----

TEST_F(Ut_Units, NewtonIsDerived)
{
    Quantity n = Units::newton();
    EXPECT_FALSE(n.isNan());
    EXPECT_TRUE(n.hasDimension());
}

TEST_F(Ut_Units, HertzIsDerived)
{
    Quantity h = Units::hertz();
    EXPECT_FALSE(h.isNan());
    EXPECT_TRUE(h.hasDimension());
}

TEST_F(Ut_Units, JouleIsDerived)
{
    Quantity j = Units::joule();
    EXPECT_FALSE(j.isNan());
    EXPECT_TRUE(j.hasDimension());
}

TEST_F(Ut_Units, WattIsDerived)
{
    Quantity w = Units::watt();
    EXPECT_FALSE(w.isNan());
    EXPECT_TRUE(w.hasDimension());
}

// ---- Non-SI units ----

TEST_F(Ut_Units, InchUnit)
{
    Quantity in = Units::inch();
    EXPECT_FALSE(in.isNan());
    EXPECT_TRUE(in.hasDimension());
    // 1 inch = 0.0254 meter — check the numeric value, dimension carries the unit.
    EXPECT_EQ(DMath::format(in.numericValue()).toStdString(), "2.54E-2");
}

TEST_F(Ut_Units, PoundUnit)
{
    Quantity lb = Units::pound();
    EXPECT_FALSE(lb.isNan());
    // 1 pound = 0.45359237 kg
    EXPECT_EQ(DMath::format(lb.numericValue()).toStdString(), "4.5359237E-1");
}

TEST_F(Ut_Units, MinuteUnit)
{
    Quantity min = Units::minute();
    EXPECT_EQ(DMath::format(min.numericValue()).toStdString(), "6E+1");
}

TEST_F(Ut_Units, HourUnit)
{
    Quantity hr = Units::hour();
    EXPECT_EQ(DMath::format(hr.numericValue()).toStdString(), "3.6E+3");
}

TEST_F(Ut_Units, DayUnit)
{
    Quantity d = Units::day();
    EXPECT_EQ(DMath::format(d.numericValue()).toStdString(), "8.64E+4");
}

// ---- Unit list ----

TEST_F(Ut_Units, GetListIsNotEmpty)
{
    const QList<Unit> list = Units::getList();
    EXPECT_FALSE(list.isEmpty());
    // Each entry should have a non-empty name
    for (const Unit &u : list) {
        EXPECT_FALSE(u.name.isEmpty());
    }
}

// ---- findUnit / clearCache ----

TEST_F(Ut_Units, ClearCacheDoesNotCrash)
{
    Units::clearCache();
    // Accessing a unit after clearing cache should still work (rebuilds cache)
    Quantity k = Units::kilo();
    EXPECT_EQ(DMath::format(k).toStdString(), "1E+3");
}

// ---- Percent ----

TEST_F(Ut_Units, PercentUnit)
{
    Quantity p = Units::percent();
    EXPECT_FALSE(p.isNan());
    EXPECT_EQ(DMath::format(p.numericValue()).toStdString(), "1E-2");
}

// ---- Gram (derived from kilogram) ----

TEST_F(Ut_Units, GramUnit)
{
    Quantity g = Units::gram();
    EXPECT_FALSE(g.isNan());
    // gram = milli * kilogram = 1e-3 * 1 = 1e-3
    EXPECT_EQ(DMath::format(g.numericValue()).toStdString(), "1E-3");
}
