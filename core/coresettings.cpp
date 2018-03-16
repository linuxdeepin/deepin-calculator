// This file is part of the SpeedCrunch project
// Copyright (C) 2004, 2005, 2007, 2008 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2005-2006 Johan Thelin <e8johan@gmail.com>
// Copyright (C) 2007-2016 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.

#include "core/coresettings.h"

#include "math/floatconfig.h"

#include <QDir>
#include <QLocale>
#include <QSettings>
#include <QApplication>
#include <QFont>
#include <QtCore/QStandardPaths>

// The current config revision. Based on the application version number
// ('1200' corresponds to 0.12.0, '10300' would be 1.3.0 etc.). When making
// a backwards-incompatible change to the config format, bump this number to
// the next release (if not already happened), then update the migration code
// in createQSettings. Don't bump the config version unnecessarily for
// releases that don't contain incompatible changes.
static const int ConfigVersion = 1200;


static const char* DefaultColorScheme = "Terminal";

static CoreSettings* s_settingsInstance = 0;
static char s_radixCharacter = 0;

static void s_deleteSettings()
{
    delete s_settingsInstance;
}

CoreSettings* CoreSettings::instance()
{
    if (!s_settingsInstance) {
        s_settingsInstance = new CoreSettings;
        qAddPostRoutine(s_deleteSettings);
    }

    return s_settingsInstance;
}

CoreSettings::CoreSettings()
{
}

char CoreSettings::radixCharacter() const
{
    if (isRadixCharacterAuto() || isRadixCharacterBoth())
        return QLocale().decimalPoint().toLatin1();

    return s_radixCharacter;
}

bool CoreSettings::isRadixCharacterAuto() const
{
    return s_radixCharacter == 0;
}

bool CoreSettings::isRadixCharacterBoth() const
{
    return s_radixCharacter == '*';
}

void CoreSettings::setRadixCharacter(char c)
{
    s_radixCharacter = (c != ',' && c != '.' && c != '*') ? 0 : c;
}
