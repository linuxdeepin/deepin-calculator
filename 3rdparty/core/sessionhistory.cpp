// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "sessionhistory.h"


HistoryEntry::HistoryEntry(const QJsonObject & json)
{
    deSerialize(json);
}

QString HistoryEntry::expr() const
{
    return m_expr;
}

Quantity HistoryEntry::result() const
{
    return m_result;
}

void HistoryEntry::setExpr(const QString & e)
{
    m_expr = e;
}

void HistoryEntry::setResult(const Quantity & n)
{
    m_result = n;
}

void HistoryEntry::serialize(QJsonObject & json) const
{
    json["expression"] = m_expr;
    QJsonObject result;
    m_result.serialize(result);
    json["result"] = result;
    return;
}

void HistoryEntry::deSerialize(const QJsonObject & json)
{
    *this = HistoryEntry();

    if (json.contains("expression"))
        m_expr = json["expression"].toString();

    if (json.contains("result"))
        m_result = Quantity(json["result"].toObject());
    return;
}
