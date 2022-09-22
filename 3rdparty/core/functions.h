// Copyright (C) 2004 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008-2009, 2013 @heldercorreia
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_FUNCTION_H
#define CORE_FUNCTION_H

#include "errors.h"
#include "../math/quantity.h"

#include <QHash>
#include <QObject>
#include <QStringList>
#include <QVector>

class Function;

class Function : public QObject
{
    Q_OBJECT
public:
    typedef QVector<Quantity> ArgumentList;
    typedef Quantity(*FunctionImpl)(Function *, const ArgumentList &);

    Function(const QString &identifier, FunctionImpl ptr, QObject *parent = 0)
        : QObject(parent)
        , m_identifier(identifier)
        , m_ptr(ptr)
    {
        m_error = Success;
    }

    const QString &identifier() const { return m_identifier; }
    const QString &name() const { return m_name; }
    const QString &usage() const { return m_usage; }
    Error error() const { return m_error; }
    Quantity exec(const ArgumentList &);

    void setName(const QString &name) { m_name = name; }
    void setUsage(const QString &usage) { m_usage = usage; }
    void setError(Error error) { m_error = error; }

private:
    Q_DISABLE_COPY(Function)
    Function();

    QString m_identifier;
    QString m_name;
    QString m_usage;
    Error m_error;
    FunctionImpl m_ptr;
};

class FunctionRepo : public QObject
{
    Q_OBJECT
public:
    static FunctionRepo *instance();

    void insert(Function *);
    QStringList getIdentifiers() const;
    Function *find(const QString &identifier) const;

public slots:
    void retranslateText();

private:
    Q_DISABLE_COPY(FunctionRepo)
    FunctionRepo();

    void createFunctions();
    void setFunctionNames();
    void setNonTranslatableFunctionUsages();
    void setTranslatableFunctionUsages();

    QHash<QString, Function *> m_functions;
};

#endif // CORE_FUNCTION_H
