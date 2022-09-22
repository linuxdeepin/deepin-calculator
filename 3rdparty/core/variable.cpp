// Copyright (C) 2004, 2005, 2007 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2007-2009, 2013, 2014 @heldercorreia
// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "variable.h"


Variable::Variable(const QJsonObject &json)
{
    deSerialize(json);
}

void Variable::serialize(QJsonObject &json) const
{
    json["identifier"] = m_identifier;
    QJsonObject value;
    m_value.serialize(value);
    json["value"] = value;
    json["type"] = (m_type==UserDefined) ? QStringLiteral("User") : QStringLiteral("BuiltIn");
}

void Variable::deSerialize(const QJsonObject &json)
{
    if (json.contains("identifier"))
        m_identifier = json["identifier"].toString();

    if (json.contains("type")) {
        QString str = json["type"].toString();
        m_type = (str=="User") ? UserDefined : BuiltIn;
    }

    if (json.contains("value"))
        m_value = Quantity(json["value"].toObject());
}

