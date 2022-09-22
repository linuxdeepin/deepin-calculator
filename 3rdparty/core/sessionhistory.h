// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SESSIONHISTORY_H
#define CORE_SESSIONHISTORY_H

#include <QJsonArray>
#include <QString>
#include <QList>

#include "../math/quantity.h"


class HistoryEntry
{
private:
    QString m_expr;
    Quantity m_result;
public:
    HistoryEntry() : m_expr(""), m_result(0) {}
    HistoryEntry(const QJsonObject &json);
    HistoryEntry(const QString &expr, const Quantity &num) : m_expr(expr), m_result(num) {}
    HistoryEntry(const HistoryEntry &other) :  m_expr(other.m_expr), m_result(other.m_result) {}

    void setExpr(const QString &e);
    void setResult(const Quantity &n);

    QString expr() const;
    Quantity result() const;

    void serialize(QJsonObject &json) const;
    void deSerialize(const QJsonObject &json);
};

#endif // CORE_SESSIONHISTORY_H
