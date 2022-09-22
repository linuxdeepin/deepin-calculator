// Copyright (C) 2015 Pol Welter <polwelter@gmail.com>
//
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_SESSION_H
#define CORE_SESSION_H

#include "../math/hmath.h"
#include "sessionhistory.h"
#include "variable.h"
#include "userfunction.h"
#include <QList>
#include <QHash>
#include <QString>
#include <QJsonArray>
#include <QObject>

class Session : public QObject
{
private:
    typedef QList<HistoryEntry> History ;
    typedef QHash<QString, Variable> VariableContainer;
    typedef QHash<QString, UserFunction> FunctionContainer;
    History m_history;
    VariableContainer m_variables;
    FunctionContainer m_userFunctions;

public:
    Session(QObject *p = nullptr);
//    Session(QJsonObject & json);

    void load();
    void save();

    void serialize(QJsonObject &json) const;
    int deSerialize(const QJsonObject &json, bool merge);


    void addVariable(const Variable &var);
    bool hasVariable(const QString &id) const;
    void removeVariable(const QString &id);
    void clearVariables();
    Variable getVariable(const QString &id) const;
    QList<Variable> variablesToList() const;
    bool isBuiltInVariable(const QString &id) const;

    void addHistoryEntry(const HistoryEntry &entry);
    void insertHistoryEntry(const int index, const HistoryEntry &entry);
    void removeHistoryEntryAt(const int index);
    HistoryEntry historyEntryAt(const int index) const;
    QList<HistoryEntry> historyToList() const {return m_history;}
    void clearHistory();

    void addUserFunction(const UserFunction &func);
    void removeUserFunction(const QString &str);
    void clearUserFunctions();
    bool hasUserFunction(const QString &str) const;
    QList<UserFunction> UserFunctionsToList() const;
    const UserFunction *getUserFunction(const QString &fname) const;
};

#endif // CORE_SESSION_H
