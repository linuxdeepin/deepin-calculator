/*
* Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
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

#ifndef PROEXPRESSIONBAR_H
#define PROEXPRESSIONBAR_H

#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>

#include "inputedit.h"
#include "../views/simplelistview.h"
#include "../views/simplelistmodel.h"
#include "../views/simplelistdelegate.h"

DWIDGET_USE_NAMESPACE

class ProExpressionBar : public DWidget
{
    Q_OBJECT

public:
    explicit ProExpressionBar(QWidget *parent = nullptr);
    ~ProExpressionBar();
    void mouseMoveEvent(QMouseEvent *event);
    InputEdit *getInputEdit();
    bool isnumber(QChar a);
    bool judgeinput();

signals:
//    void keyPress(QKeyEvent *);
    void clearStateChanged(bool);
    void keyPress(QKeyEvent *);

public slots:
    //输入事件
    void enterNumberEvent(const QString &text);
    void enterSymbolEvent(const QString &text);
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
//    void enterModEvent();
    void enterNotEvent();
    void enterOperatorEvent(const QString &text);
    void enterOppositeEvent();
    void enterLeftBracketsEvent();
    void enterRightBracketsEvent();

    void moveLeft();
    void moveRight();

    void initTheme(int type);
    void revisionResults(const QModelIndex &index);
    void addUndo();
    void copyResultToClipboard();
    void copyClipboard2Result();
    void allElection();
    void shear();
    void deleteText();
    void Undo();
    void Redo();
    void setResultFalse();

private:
    void initConnect();
    void replaceSelection(QString text);
    QString symbolComplement(const QString exp);
    bool cursorPosAtEnd();
    bool isOperator(const QString &text);
    void expressionCheck();
    QString symbolFaultTolerance(const QString &text);
    bool isNumberOutOfRange(const QString &text);
    void selectedPartDelete(const QRegExp &rx);
    bool curposInNumber(int curpos);

private slots:
    void handleTextChanged();

private:
    Evaluator *m_evaluator;
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;           //计算结果
    bool m_inputNumber;        //输入数字
    bool m_isUndo;
    QVector<QString> m_undo;
    QVector<QString> m_redo;
    QList<QString> m_funclist; //支持的函数
    QString m_expression = QString();

    QVector<QString> m_numvec;
};

#endif // PROEXPRESSIONBAR_H
