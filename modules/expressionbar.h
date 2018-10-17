/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPRESSIONBAR_H
#define EXPRESSIONBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>

#include "simplelistview.h"
#include "simplelistmodel.h"
#include "simplelistdelegate.h"
#include "../widgets/inputedit.h"
#include "../core/evaluator.h"

class ExpressionBar : public QWidget
{
    Q_OBJECT

public:
    ExpressionBar(QWidget *parent = nullptr);
    ~ExpressionBar();

    void setContinue(bool isContinue);

signals:
    void keyPress(QKeyEvent *);
    void clearStateChanged(bool);

public slots:
    void enterNumberEvent(const QString &text);
    void enterSymbolEvent(const QString &text);
    void enterPointEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    void enterBracketsEvent();
    void copyResultToClipboard();

private slots:
    void handleTextChanged(const QString &text);

private:
    bool cursorPosAtEnd();
    QString formatExpression(const QString &text);

private:
    Evaluator *m_evaluator;
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    bool m_isContinue;
    bool m_isAllClear;
};

#endif
