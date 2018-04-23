// This file is part of the SpeedCrunch project
// Copyright (C) 2004 Ariya Hidayat <ariya@kde.org>
// Copyright (C) 2008, 2009, 2010, 2013 @heldercorreia
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

#ifndef CORE_EVALUATOR_H
#define CORE_EVALUATOR_H

#include "core/token.h"
#include "core/functions.h"
#include "math/hmath.h"
#include "math/cmath.h"
#include "math/quantity.h"

#include <QHash>
#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QVector>
#include <core/opcode.h>
#include "core/variable.h"
#include "core/userfunction.h"

class Session;

class Tokens : public QVector<Token> {
public:
    Tokens() : QVector<Token>(), m_valid(true) { }

    bool valid() const { return m_valid; }
    void setValid(bool v) { m_valid = v; }

#ifdef EVALUATOR_DEBUG
    void append(const Token&);
#endif  /* EVALUATOR_DEBUG */

protected:
    bool m_valid;
};


class Evaluator : public QObject
{
    Q_OBJECT

public:
    static Evaluator* instance();
    void reset();

    void setSession(Session * s);
    const Session *session();

    static bool isSeparatorChar(const QChar&);
    static bool isRadixChar(const QChar&);
    static QString fixNumberRadix(const QString&);

    QString autoFix(const QString&);
    QString dump();
    QString error() const;
    Quantity eval();
    Quantity evalNoAssign();
    Quantity evalUpdateAns();
    QString expression() const;
    bool isValid();
    Tokens scan(const QString&) const;
    void setExpression(const QString&);
    Tokens tokens() const;
    bool isUserFunctionAssign() const;

    Variable getVariable(const QString&) const;
    QList<Variable> getVariables() const;
    QList<Variable> getUserDefinedVariables() const;
    QList<Variable> getUserDefinedVariablesPlusAns() const;
    void setVariable(const QString&, Quantity, Variable::Type = Variable::UserDefined);
    void unsetVariable(const QString&, bool force_builtin = false);
    void unsetAllUserDefinedVariables();
    bool isBuiltInVariable(const QString&) const;
    bool hasVariable(const QString&) const;
    void initializeBuiltInVariables();
    void initializeAngleUnits();

    QList<UserFunction> getUserFunctions() const;
    void setUserFunction(const UserFunction & f);
    void unsetUserFunction(const QString&);
    void unsetAllUserFunctions();
    bool hasUserFunction(const QString&) const;

protected:
    void compile(const Tokens&);

private:
    Evaluator();
    Q_DISABLE_COPY(Evaluator)

    bool m_dirty;
    QString m_error;
    QString m_expression;
    bool m_valid;
    QString m_assignId;
    bool m_assignFunc;
    QStringList m_assignArg;
    QVector<Opcode> m_codes;
    QVector<Quantity> m_constants;
    QStringList m_identifiers;
    Session * m_session;
    QSet<QString> m_functionsInUse;

    const Quantity& checkOperatorResult(const Quantity&);
    static QString stringFromFunctionError(Function*);
    Quantity exec(const QVector<Opcode>& opcodes, const QVector<Quantity>& constants,
               const QStringList& identifiers);
    Quantity execUserFunction(const UserFunction* function, QVector<Quantity>& arguments);
    const UserFunction * getUserFunction(const QString&) const;

    bool isFunction(Token token) { return token.isIdentifier() && (FunctionRepo::instance()->find(token.text()) || hasUserFunction(token.text())); };
};

#endif
