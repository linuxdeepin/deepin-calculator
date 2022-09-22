// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_USERFUNCTION_H
#define CORE_USERFUNCTION_H

#include<QString>
#include<QStringList>
#include<QVector>
#include "opcode.h"
#include "../math/quantity.h"

class UserFunction
{
private:
    QString m_name;
    QStringList m_arguments;
    QString m_expression;
    QString m_description;

public:
    QVector<Quantity> constants;
    QStringList identifiers;
    QVector<Opcode> opcodes;

    UserFunction(QString name, QStringList arguments, QString expression)
        : m_name(name), m_arguments(arguments), m_expression(expression) {}
    UserFunction() {}
    UserFunction(const QJsonObject &json);

    QString name() const;
    QStringList arguments() const;
    QString expression() const;
    QString description() const;

    void setName(const QString &str);
    void setArguments(const QStringList &args);
    void setExpression(const QString &expr);
    void setDescription(const QString &expr);

    void serialize(QJsonObject &json) const;
    void deSerialize(const QJsonObject &json);
};

#endif // CORE_USERFUNCTION_H
