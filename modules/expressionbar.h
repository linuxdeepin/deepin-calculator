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
#include <QVector>
#include <QPair>

#include "simplelistview.h"
#include "simplelistmodel.h"
#include "simplelistdelegate.h"
#include "../widgets/inputedit.h"
#include "../core/evaluator.h"

struct historicalLinkageIndex

{
    int linkageTerm;
    int linkedItem;
    QString linkageValue;
    bool isLink;
    historicalLinkageIndex() {
        linkageTerm = -1;
        linkedItem = -1;
        isLink = false;
    }
};

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
    void computationalResults(const QString &expression, QString &result);
    void historicalLinkage(int index, QString newValue);
    void clearLinkageCache();
    void setLinkState(const QModelIndex index);
    void settingLinkage(const QModelIndex &index);
    void settingLinkage();

private slots:
    void handleTextChanged(const QString &text);
    void revisionResults(const QModelIndex &index);

private:
    bool cursorPosAtEnd();
    QString formatExpression(const QString &text);
    QString completedBracketsCalculation(QString &text);

private:
    Evaluator *m_evaluator;
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    InputEdit *m_inputEdit;

    QString m_unfinishedExp;    //未完成表达式
    bool m_isContinue;
    bool m_isAllClear;
    bool m_isResult;             //计算结果
    bool m_isAutoComputation;    //自动计算
    bool m_inputNumber;          //输入数字
    int m_hisRevision;           //历史记录修改

    int m_linkageIndex;          //联动索引缓存
    int m_Selected;              //选中
    QVector<historicalLinkageIndex> m_hisLink;   //历史联动索引
};

#endif
