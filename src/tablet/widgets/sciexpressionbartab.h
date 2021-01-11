/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     xiajing <xiajing@uniontech.com>
*
* Maintainer: jingzhou <jingzhou@uniontech.com>
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

#ifndef SCIEXPRESSIONBARTAB_H
#define SCIEXPRESSIONBARTAB_H

#include <QKeyEvent>
#include <QPair>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <QToolTip>

#include <DWidget>

#include "core/evaluator.h"
#include "widgets/inputedit.h"
#include "tablet/views/simplelistdelegatetab.h"
#include "views/simplelistmodel.h"
#include "tablet/views/simplelistviewtab.h"
#include "performancemonitor.h"

DWIDGET_USE_NAMESPACE

/**
 * @brief 科学计算器表达式计算函数
 */
class SciExpressionBarTab : public DWidget
{
    Q_OBJECT

public:
    SciExpressionBarTab(QWidget *parent = nullptr);
    ~SciExpressionBarTab();

    void mouseMoveEvent(QMouseEvent *e);

    QPair<bool, QString> getexpression();
    Quantity getanswer();

signals:
    void keyPress(QKeyEvent *);
    void clearStateChanged(bool);
    void themeChange(int type);
    void turnDeg();
    void fEStateChanged(bool);

public slots:
    //输入事件
    void enterNumberEvent(const QString &text);
    void enterSymbolEvent(const QString &text);
    void enterPointEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    void enterPercentEvent();
    void enterLeftBracketsEvent();
    void enterRightBracketsEvent();
    void enterDegEvent(int mod);
    void enterSinEvent();
    void enterFEEvent(bool isdown);
    void enterPIEvent();
    void enterEulerEvent(); //无理数e
    void enterModEvent();
    void enterx2Event();
    void enterDerivativeEvent(); //导数
    void enterFactorialsEvent(); //阶乘
    void enterExpEvent();
    void enterCosEvent();
    void enterx3Event();
    void enterTanEvent();
    void enterxyEvent();
    void enterCotEvent();
    void enter10xEvent();
    void enterModulusEvent(); //模(绝对值)
    void enterLogEvent();
    void enterRandEvent();
    void enterLnEvent();
    void enterArcsinEvent();
    void enterSqrtEvent();
    void enterArccosEvent();
    void enterCbrtEvent();
    void enterArctanEvent();
    void enterYrootEvent();
    void enterArccotEvent();
    void enter2xEvent();
    void enterlogyxEvent();
    void enterexEvent();
    void enterOppositeEvent(); //相反数
    void moveLeft();
    void moveRight();
    void copyResultToClipboard();
    void copyClipboard2Result();
    void allElection();
    void shear();
    void deleteText();
//    void computationalResults(const QString &expression, QString &result);
//    void historicalLinkage(int index, QString newValue);
//    void clearLinkageCache(const QString &text, bool isequal);
//    void setLinkState(const QModelIndex index);
//    void settingLinkage(const QModelIndex &index);
//    void settingLinkage();
    void Undo();
    void addUndo();
    void Redo();
    void initTheme(int type);
//    void setSelection();
//    void getSelection();
    void setResultFalse();
    void replaceSelection(QString text);
    InputEdit *getInputEdit();
    void revisionResults(const QModelIndex &index);
    void hisRevisionResults(const QModelIndex &index, Quantity ans);
    bool judgeinput();

private slots:
    void handleTextChanged(const QString &text);


private:
    bool cursorPosAtEnd();
    QString formatExpression(const QString &text);
//    QString completedBracketsCalculation(QString &text);
    bool isOperator(const QString &text);
//    bool cancelLink(int index);
//    void judgeLinkageAgain();
    void initConnect();
    QString symbolComplement(const QString exp);
//    QString pasteFaultTolerance(QString exp);
    QString pointFaultTolerance(const QString &text);
//    void clearSelectSymbol();
    void expressionCheck();

private:
    Evaluator *m_evaluator;
    SimpleListViewTab *m_listView;
    SimpleListDelegateTab *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    QString m_unfinishedExp;  //未完成表达式
    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;           //计算结果
    bool m_isAutoComputation;  //自动计算
    bool m_inputNumber;        //输入数字
    bool m_isUndo;
    QString m_selection;
    QVector<QString> m_undo;
    QVector<QString> m_redo;

    bool m_meanexp;    //是否表达式为纯数字
    bool m_FEisdown = false; //科学计算开关打开
    //支持的功能列表
    QList<QString> m_funclist;
    QString m_expression = QString();
    QPair<bool, QString> m_pair; //getexpression()函数的返回值，first为是否是错误表达式，second为表达式
    Quantity m_listanswer = Quantity(); //历史记录对应的answer
};
#endif // SCIEXPRESSIONBARTAB_H
