﻿// This file is part of the SpeedCrunch project

// Copyright (C) 2013 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "numberformatter.h"

#include "settings.h"
#include "../math/quantity.h"

QString NumberFormatter::format(Quantity q)
{
    Settings *settings = Settings::instance();

    Quantity::Format format = q.format();
    if (format.base == Quantity::Format::Base::Null) {
        switch (settings->resultFormat) {
        case 'b':
            format.base = Quantity::Format::Base::Binary;
            break;
        case 'o':
            format.base = Quantity::Format::Base::Octal;
            break;
        case 'h':
            format.base = Quantity::Format::Base::Hexadecimal;
            break;
        case 'n':
            format.base = Quantity::Format::Base::Decimal;
            break;
        case 'f':
            format.base = Quantity::Format::Base::Decimal;
            break;
        case 'e':
            format.base = Quantity::Format::Base::Decimal;
            break;
        case 'g':
        default:
            format.base = Quantity::Format::Base::Decimal;
            break;
        }
    }

    if (format.mode == Quantity::Format::Mode::Null) {
        if (format.base == Quantity::Format::Base::Decimal) {
            switch (settings->resultFormat) {
            case 'n':
                format.mode = Quantity::Format::Mode::Engineering;
                break;
            case 'f':
                format.mode = Quantity::Format::Mode::Fixed;
                break;
            case 'e':
                format.mode = Quantity::Format::Mode::Scientific;
                break;
            case 'g':
            default:
                format.mode = Quantity::Format::Mode::General;
                break;
            }
        } else {
            format.mode = Quantity::Format::Mode::Fixed;
        }
    }

    if (format.precision == Quantity::Format::PrecisionNull)
        format.precision = settings->resultPrecision;
    if (format.notation == Quantity::Format::Notation::Null) {
        if (settings->resultFormatComplex == 'c')
            format.notation = Quantity::Format::Notation::Cartesian;
        else if (settings->resultFormatComplex == 'p')
            format.notation = Quantity::Format::Notation::Polar;
    }

    QString result = DMath::format(q, format);

    if (settings->radixCharacter() == ',')
        result.replace('.', ',');

    result.replace('-', QString::fromUtf8("−"));

    // Replace all spaces between units with dot operator.
    int emptySpaces = 0;
    for (auto &ch : result) {
        if (ch.isSpace()) {
            ++emptySpaces;
            if (emptySpaces > 1)
                ch = u'⋅';
        }
    }

    return result;
}
