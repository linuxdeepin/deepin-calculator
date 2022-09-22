// Copyright (C) 2013 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_NUMBERFORMATTER_H
#define CORE_NUMBERFORMATTER_H

#include "../math/quantity.h"

#include <QtCore/QString>

struct NumberFormatter {
    static QString format(HNumber &num) { return format(Quantity(num)); }
    static QString format(CNumber &num) { return format(Quantity(num)); }
    static QString format(Quantity);
};

#endif
