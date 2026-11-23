// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Unit definitions and automatic unit matching. Base SI units carry a
// dimension; derived units are products/quotients of base units. All
// conversion factors use the same values as the upstream definitions.

#include "units.h"

#include "quantity.h"
#include "rational.h"

#include <QString>
#include <QStringList>

#define UNIT_CACHE(name, value) \
    const Quantity Units::name() \
    { \
        if (!m_cache.contains(#name)) \
            m_cache[#name] = value; \
        return (m_cache[#name]); \
    }

#define BASE_UNIT_CACHE(name, dimension) \
    const Quantity Units::name() \
    { \
        if (!m_cache.contains(#name)) { \
            Quantity name(1); \
            name.modifyDimension(dimension, 1); \
            m_cache[#name] = name; \
        } \
        return m_cache[#name]; \
    }

// qHash for QMap<QString, Rational> so it can serve as a QHash key (Qt5 lacks it).
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
unsigned int qHash(QMap<QString, Rational> dimension)
{
    QStringList keyList(dimension.keys());
    keyList.sort();
    QString blob;
    for (int i = 0; i < keyList.size(); ++i) {
        keyList[i].append(dimension[keyList[i]].toString());
        blob.append(keyList[i]);
    }
    return qHash(blob);
}
#endif

QHash<QMap<QString, Rational>, Unit> Units::m_matchLookup;
QMap<QString, Quantity> Units::m_cache;

void Units::pushUnit(Quantity q, QString name)
{
    q.cleanDimension();
    m_matchLookup.insert(q.getDimension(), Unit(name, q));
}

void Units::initTable()
{
    m_matchLookup.clear();
    pushUnit(joule(), "newton meter");
    pushUnit(newton(), "newton");
    pushUnit(watt(), "watt");
    pushUnit(pascal(), "pascal");
    pushUnit(coulomb(), "coulomb");
    pushUnit(volt(), "volt");
    pushUnit(ohm(), "ohm");
    pushUnit(siemens(), "siemens");
    pushUnit(ohm()*meter(), "ohm meter");
    pushUnit(siemens() / meter(), "siemens/meter");
    pushUnit(siemens() / meter() / mole(), "siemens/(meter mole)");
    pushUnit(farad(), "farad");
    pushUnit(tesla(), "tesla");
    pushUnit(weber(), "weber");
    pushUnit(henry(), "henry");
    pushUnit(coulomb() / cbmeter(), "coulomb/meter³");
    pushUnit(coulomb() / sqmeter(), "coulomb/meter²");
    pushUnit(coulomb() / kilogram(), "coulomb/kilogram");
    pushUnit(farad() / meter(), "farad/meter");
    pushUnit(henry() / meter(), "henry/meter");
    pushUnit(joule() / kilogram() / kelvin(), "joule/(kilogram kelvin)");
    pushUnit(joule() / mole() / kelvin(), "joule/(mole kelvin");
    pushUnit(mole() / second() / cbmeter(), "mole/(second meter³)");
    pushUnit(newton() / meter(), "newton/meter");
    pushUnit(pascal()*second(), "pascal second");
    pushUnit(volt() / meter(), "volt/meter");
    pushUnit(watt() / meter() / kelvin(), "watt/(meter kelvin)");
    pushUnit(watt() / sqmeter(), "watt/meter²");
    pushUnit(joule() / kelvin(), "joule/kelvin");
    pushUnit(joule() / kilogram(), "joule/kilogram");
}

void Units::findUnit(Quantity &q)
{
    QString unit_name;
    CNumber unit(1);
    q.cleanDimension();

    if (m_matchLookup.isEmpty())
        initTable();

    if (m_matchLookup.contains(q.getDimension())) {
        Unit temp(m_matchLookup[q.getDimension()]);
        q.setDisplayUnit(temp.value.numericValue(), temp.name);
        return;
    }

    // Autogenerate a name from the base-unit dimensions.
    auto dimension = q.getDimension();
    auto i = dimension.constBegin();
    while (i != dimension.constEnd()) {
        auto exponent = i.value().toString();
        if (exponent.contains('/'))
            exponent = "^(" + exponent + ')';
        else if (exponent == "1")
            exponent = "";
        else
            exponent = '^' + exponent;

        // Unicode superscripts for common small exponents.
        if (exponent == QLatin1String("^0")) exponent = QString::fromUtf8("⁰");
        else if (exponent == QLatin1String("^2")) exponent = QString::fromUtf8("²");
        else if (exponent == QLatin1String("^3")) exponent = QString::fromUtf8("³");
        else if (exponent == QLatin1String("^4")) exponent = QString::fromUtf8("⁴");
        else if (exponent == QLatin1String("^5")) exponent = QString::fromUtf8("⁵");
        else if (exponent == QLatin1String("^6")) exponent = QString::fromUtf8("⁶");
        else if (exponent == QLatin1String("^7")) exponent = QString::fromUtf8("⁷");
        else if (exponent == QLatin1String("^8")) exponent = QString::fromUtf8("⁸");
        else if (exponent == QLatin1String("^9")) exponent = QString::fromUtf8("⁹");
        else if (exponent == QLatin1String("^-1")) exponent = QString::fromUtf8("⁻¹");
        else if (exponent == QLatin1String("^-2")) exponent = QString::fromUtf8("⁻²");
        else if (exponent == QLatin1String("^-3")) exponent = QString::fromUtf8("⁻³");
        else if (exponent == QLatin1String("^-4")) exponent = QString::fromUtf8("⁻⁴");
        else if (exponent == QLatin1String("^-5")) exponent = QString::fromUtf8("⁻⁵");
        else if (exponent == QLatin1String("^-6")) exponent = QString::fromUtf8("⁻⁶");
        else if (exponent == QLatin1String("^-7")) exponent = QString::fromUtf8("⁻⁷");
        else if (exponent == QLatin1String("^-8")) exponent = QString::fromUtf8("⁻⁸");
        else if (exponent == QLatin1String("^-9")) exponent = QString::fromUtf8("⁻⁹");

        static const QMap<QString, QString> baseNames = {
            {"length", "meter"}, {"time", "second"}, {"mass", "kilogram"},
            {"el. current", "ampere"}, {"amount", "mole"},
            {"luminous intensity", "candela"}, {"temperature", "kelvin"},
            {"information", "bit"},
        };
        unit_name += ' ' + baseNames.value(i.key(), i.key()) + exponent;
        ++i;
    }
    q.setDisplayUnit(unit, unit_name.trimmed());
}

#define ADD_UNIT(name) result.append(Unit(#name, name()))
#define ADD_UNIT_ALIAS(name, alias) result.append(Unit(#alias, name()))

const QList<Unit> Units::getList()
{
    QList<Unit> result;

    ADD_UNIT(meter); ADD_UNIT(second); ADD_UNIT(kilogram); ADD_UNIT(ampere);
    ADD_UNIT(mole); ADD_UNIT(candela); ADD_UNIT(kelvin); ADD_UNIT(bit);

    ADD_UNIT(yocto); ADD_UNIT(zepto); ADD_UNIT(atto); ADD_UNIT(femto);
    ADD_UNIT(pico); ADD_UNIT(nano); ADD_UNIT(micro); ADD_UNIT(milli);
    ADD_UNIT(centi); ADD_UNIT(deci); ADD_UNIT(deca); ADD_UNIT(hecto);
    ADD_UNIT(kilo); ADD_UNIT(mega); ADD_UNIT(giga); ADD_UNIT(tera);
    ADD_UNIT(peta); ADD_UNIT(exa); ADD_UNIT(zetta); ADD_UNIT(yotta);
    ADD_UNIT(kibi); ADD_UNIT(mebi); ADD_UNIT(gibi); ADD_UNIT(tebi);
    ADD_UNIT(pebi); ADD_UNIT(exbi); ADD_UNIT(zebi); ADD_UNIT(yobi);

    ADD_UNIT(sqmeter); ADD_UNIT(cbmeter); ADD_UNIT(newton); ADD_UNIT(hertz);
    ADD_UNIT(joule); ADD_UNIT(watt); ADD_UNIT(pascal); ADD_UNIT(coulomb);
    ADD_UNIT(volt); ADD_UNIT(ohm); ADD_UNIT(farad); ADD_UNIT(tesla);
    ADD_UNIT(weber); ADD_UNIT(henry); ADD_UNIT(siemens); ADD_UNIT(becquerel);
    ADD_UNIT(gray); ADD_UNIT(sievert); ADD_UNIT(katal); ADD_UNIT(steradian);
    ADD_UNIT(lumen); ADD_UNIT(lux);

    ADD_UNIT(metric_ton); ADD_UNIT(short_ton); ADD_UNIT(long_ton);
    ADD_UNIT(pound); ADD_UNIT(ounce); ADD_UNIT(grain); ADD_UNIT(gram);
    ADD_UNIT(atomic_mass_unit); ADD_UNIT(carat);

    ADD_UNIT(micron); ADD_UNIT(angstrom); ADD_UNIT(astronomical_unit);
    ADD_UNIT(lightyear); ADD_UNIT(lightsecond); ADD_UNIT(lightminute);
    ADD_UNIT(parsec); ADD_UNIT(inch); ADD_UNIT(foot); ADD_UNIT(yard);
    ADD_UNIT(mile); ADD_UNIT(rod); ADD_UNIT(furlong); ADD_UNIT(fathom);
    ADD_UNIT(nautical_mile); ADD_UNIT(cable);

    ADD_UNIT(UK_gallon); ADD_UNIT(US_gallon);
    ADD_UNIT_ALIAS(US_gallon, gallon_US);
    ADD_UNIT_ALIAS(UK_gallon, gallon_UK);
    ADD_UNIT_ALIAS(UK_gallon, imperial_gallon);
    ADD_UNIT(UK_quart); ADD_UNIT(US_quart);
    ADD_UNIT_ALIAS(US_quart, quart_US);
    ADD_UNIT_ALIAS(UK_quart, quart_UK);
    ADD_UNIT(UK_pint); ADD_UNIT(US_pint);
    ADD_UNIT_ALIAS(US_pint, pint_US);
    ADD_UNIT_ALIAS(UK_pint, pint_UK);
    ADD_UNIT(UK_fluid_ounce); ADD_UNIT(US_fluid_ounce);
    ADD_UNIT_ALIAS(US_fluid_ounce, fluid_ounce_US);
    ADD_UNIT_ALIAS(UK_fluid_ounce, fluid_ounce_UK);
    ADD_UNIT(liter);

    ADD_UNIT(minute); ADD_UNIT(hour); ADD_UNIT(day); ADD_UNIT(week);
    ADD_UNIT(julian_year); ADD_UNIT(tropical_year); ADD_UNIT(sidereal_year);
    ADD_UNIT_ALIAS(julian_year, year_julian);
    ADD_UNIT_ALIAS(tropical_year, year_tropical);
    ADD_UNIT_ALIAS(sidereal_year, year_sidereal);

    ADD_UNIT(percent); ADD_UNIT(ppm); ADD_UNIT(ppb); ADD_UNIT(karat);

    ADD_UNIT(bar); ADD_UNIT(atmosphere); ADD_UNIT(torr); ADD_UNIT(pounds_per_sqinch);

    ADD_UNIT(electron_volt); ADD_UNIT(calorie); ADD_UNIT(british_thermal_unit);

    ADD_UNIT(nat); ADD_UNIT(hartley); ADD_UNIT(byte);

    ADD_UNIT(tablespoon); ADD_UNIT(teaspoon); ADD_UNIT(cup);

    ADD_UNIT(gravity);
    ADD_UNIT_ALIAS(gravity, force);
    ADD_UNIT(speed_of_light); ADD_UNIT(speed_of_sound_STP);
    ADD_UNIT(elementary_charge); ADD_UNIT(knot); ADD_UNIT(horsepower);

    return result;
}

// --- Base SI units (each carries a dimension) ---
BASE_UNIT_CACHE(meter, "length")
BASE_UNIT_CACHE(second, "time")
BASE_UNIT_CACHE(kilogram, "mass")
BASE_UNIT_CACHE(ampere, "el. current")
BASE_UNIT_CACHE(mole, "amount")
BASE_UNIT_CACHE(kelvin, "temperature")
BASE_UNIT_CACHE(candela, "luminous intensity")
BASE_UNIT_CACHE(bit, "information")

// --- SI prefixes ---
UNIT_CACHE(yocto, HNumber("1e-24"))
UNIT_CACHE(zepto, HNumber("1e-21"))
UNIT_CACHE(atto, HNumber("1e-18"))
UNIT_CACHE(femto, HNumber("1e-15"))
UNIT_CACHE(pico, HNumber("1e-12"))
UNIT_CACHE(nano, HNumber("1e-9"))
UNIT_CACHE(micro, HNumber("1e-6"))
UNIT_CACHE(milli, HNumber("1e-3"))
UNIT_CACHE(centi, HNumber("1e-2"))
UNIT_CACHE(deci, HNumber("1e-1"))
UNIT_CACHE(deca, HNumber("1e1"))
UNIT_CACHE(hecto, HNumber("1e2"))
UNIT_CACHE(kilo, HNumber("1e3"))
UNIT_CACHE(mega, HNumber("1e6"))
UNIT_CACHE(giga, HNumber("1e9"))
UNIT_CACHE(tera, HNumber("1e12"))
UNIT_CACHE(peta, HNumber("1e15"))
UNIT_CACHE(exa, HNumber("1e18"))
UNIT_CACHE(zetta, HNumber("1e21"))
UNIT_CACHE(yotta, HNumber("1e24"))

// --- Binary prefixes ---
UNIT_CACHE(kibi, HNumber("1024"))
UNIT_CACHE(mebi, kibi()*kibi())
UNIT_CACHE(gibi, kibi()*mebi())
UNIT_CACHE(tebi, kibi()*gibi())
UNIT_CACHE(pebi, kibi()*tebi())
UNIT_CACHE(exbi, kibi()*pebi())
UNIT_CACHE(zebi, kibi()*exbi())
UNIT_CACHE(yobi, kibi()*zebi())

// --- Derived SI units ---
UNIT_CACHE(newton,        meter() * kilogram() / (second()*second()))
UNIT_CACHE(hertz,         Quantity(1) / second())
UNIT_CACHE(pascal,        newton() / sqmeter())
UNIT_CACHE(joule,         newton() * meter())
UNIT_CACHE(watt,          joule() / second())
UNIT_CACHE(coulomb,       ampere() * second())
UNIT_CACHE(volt,          watt() / ampere())
UNIT_CACHE(farad,         coulomb() / volt())
UNIT_CACHE(ohm,           volt() / ampere())
UNIT_CACHE(siemens,       ampere() / volt())
UNIT_CACHE(weber,         volt() * second())
UNIT_CACHE(tesla,         weber() / sqmeter())
UNIT_CACHE(henry,         weber() / ampere())
UNIT_CACHE(becquerel,     Quantity(1) / second())
UNIT_CACHE(gray,          joule() / kilogram())
UNIT_CACHE(sievert,       joule() / kilogram())
UNIT_CACHE(katal,         mole() / second())
UNIT_CACHE(steradian,     1)
UNIT_CACHE(lumen,         candela()*steradian())
UNIT_CACHE(lux,           lumen() / sqmeter())

UNIT_CACHE(sqmeter,       meter() * meter())
UNIT_CACHE(cbmeter,       sqmeter() * meter())

// --- Mass ---
UNIT_CACHE(metric_ton,    mega()*gram())
UNIT_CACHE(gram,          milli()*kilogram())
UNIT_CACHE(pound,         HNumber("0.45359237") * kilogram())
UNIT_CACHE(ounce,         pound() / HNumber(16))
UNIT_CACHE(grain,         pound() / HNumber(7000))
UNIT_CACHE(short_ton,     HNumber(2000) * pound())
UNIT_CACHE(long_ton,      HNumber(2240) * pound())
UNIT_CACHE(atomic_mass_unit, HNumber("1.660539040e-27") * kilogram())
UNIT_CACHE(carat,         HNumber(200) * milli()*gram())

// --- Distance/length ---
UNIT_CACHE(micron,        micro()*meter())
UNIT_CACHE(angstrom,      HNumber("1e-10") * meter())
UNIT_CACHE(astronomical_unit, HNumber("149597870700") * meter())
UNIT_CACHE(lightyear,     speed_of_light() * julian_year())
UNIT_CACHE(lightminute,   speed_of_light() * minute())
UNIT_CACHE(lightsecond,   speed_of_light() * second())
UNIT_CACHE(parsec,        HNumber(648000) / HMath::pi() * astronomical_unit())
UNIT_CACHE(inch,          HNumber("0.0254") * meter())
UNIT_CACHE(foot,          HNumber(12) * inch())
UNIT_CACHE(yard,          HNumber(36) * inch())
UNIT_CACHE(mile,          HNumber(1760) * yard())
UNIT_CACHE(rod,           HNumber("5.5") * yard())
UNIT_CACHE(furlong,       HNumber(40) * rod())
UNIT_CACHE(fathom,        HNumber(6) * foot())
UNIT_CACHE(nautical_mile, HNumber("1852") * meter())
UNIT_CACHE(cable,         HNumber("0.1") * nautical_mile())

// --- Area ---
UNIT_CACHE(are,           HNumber(100) * sqmeter())
UNIT_CACHE(hectare,       HNumber(100) * are())
UNIT_CACHE(acre,          mile()*mile() / HNumber(640))

// --- Volume ---
UNIT_CACHE(US_gallon,     HNumber("3.785") * liter())
UNIT_CACHE(UK_gallon,     HNumber("4.54609") * liter())
UNIT_CACHE(US_quart,      US_gallon() / HNumber(4))
UNIT_CACHE(UK_quart,      UK_gallon() / HNumber(4))
UNIT_CACHE(US_pint,       US_gallon() / HNumber(8))
UNIT_CACHE(UK_pint,       UK_gallon() / HNumber(8))
UNIT_CACHE(US_fluid_ounce, US_gallon() / HNumber(128))
UNIT_CACHE(UK_fluid_ounce, UK_gallon() / HNumber(160))
UNIT_CACHE(liter,         milli() * cbmeter())

// --- Time ---
UNIT_CACHE(minute,        HNumber(60) * second())
UNIT_CACHE(hour,          HNumber(60) * minute())
UNIT_CACHE(day,           HNumber(24) * hour())
UNIT_CACHE(week,          HNumber(7) * day())
UNIT_CACHE(julian_year,   HNumber("365.25") * day())
UNIT_CACHE(tropical_year, HNumber("365.24219") * day())
UNIT_CACHE(sidereal_year, HNumber("365.25636") * day())

// --- Concentration ---
UNIT_CACHE(percent,       HNumber("0.01"))
UNIT_CACHE(ppm,           HNumber("1e-6"))
UNIT_CACHE(ppb,           HNumber("1e-9"))
UNIT_CACHE(karat,         Rational(1, 24).toHNumber())

// --- Pressure ---
UNIT_CACHE(bar,           HNumber("1e5") * pascal())
UNIT_CACHE(atmosphere,    HNumber("1.01325") * bar())
UNIT_CACHE(torr,          atmosphere() / HNumber(760))
UNIT_CACHE(pounds_per_sqinch, pound() * gravity() / (inch()*inch()))

// --- Energy ---
UNIT_CACHE(electron_volt, elementary_charge() * volt())
UNIT_CACHE(calorie,       HNumber("4.1868") * joule())
UNIT_CACHE(british_thermal_unit, HNumber("1055.056") * joule())

// --- Information ---
UNIT_CACHE(nat,           bit() / HMath::ln(2))
UNIT_CACHE(hartley,       HMath::ln(10) * nat())
UNIT_CACHE(byte,          HNumber(8) * bit())

// --- Cooking ---
UNIT_CACHE(tablespoon,    HNumber(15) * milli()*liter())
UNIT_CACHE(teaspoon,      HNumber(5) * milli()*liter())
UNIT_CACHE(cup,           HNumber(240) * milli()*liter())

// --- Various constants ---
UNIT_CACHE(gravity,       HNumber("9.80665") * newton() / kilogram())
UNIT_CACHE(speed_of_light, HNumber(299792458) * meter() / second())
UNIT_CACHE(elementary_charge, HNumber("1.6021766208e-19") * coulomb())
UNIT_CACHE(speed_of_sound_STP, HNumber(331) * meter() / second())
UNIT_CACHE(knot,          nautical_mile() / hour())
UNIT_CACHE(horsepower,    HNumber(550) * foot() * pound() * gravity() / second())
