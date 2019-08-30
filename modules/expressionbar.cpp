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

#include <QPushButton>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include "expressionbar.h"
#include "../utils.h"

ExpressionBar::ExpressionBar(QWidget *parent)
    : QWidget(parent),
      m_listView(new SimpleListView),
      m_listDelegate(new SimpleListDelegate),
      m_listModel(new SimpleListModel),
      m_inputEdit(new InputEdit),
      m_evaluator(Evaluator::instance()),
      m_isContinue(true),
      m_isAllClear(false),
      m_isResult(false),
      m_inputNumber(false),
      m_hisRevision(-1),
      m_linkageIndex(-1),
      m_Selected(-1)
{
    // init inputEdit attributes.
    m_inputEdit->setFixedHeight(55);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);

    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);

    setMinimumHeight(160);

    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, this, &ExpressionBar::revisionResults);
    //connect(m_listDelegate, &SimpleListDelegate::historicalLinkage, this, &ExpressionBar::settingLinkage);
    connect(m_inputEdit, &InputEdit::textChanged, this, &ExpressionBar::handleTextChanged);
    connect(m_inputEdit, &InputEdit::keyPress, this, &ExpressionBar::keyPress);
}

ExpressionBar::~ExpressionBar()
{
}

void ExpressionBar::setContinue(bool isContinue)
{
    m_isContinue = isContinue;
}

void ExpressionBar::enterNumberEvent(const QString &text)
{
    if (m_inputNumber) {
        m_inputEdit->clear();
        m_inputNumber = false;
        m_isResult = false;
    }
    if (!m_inputEdit->text().isEmpty() && m_isResult) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    if (!m_isContinue) {
        // the cursor position at the end, it will clear edit text.

        if (cursorPosAtEnd())
            m_inputEdit->clear();

        m_isContinue = true;
    }

    m_inputEdit->insert(text);
    emit clearStateChanged(false);
}

void ExpressionBar::enterSymbolEvent(const QString &text)
{
    if (m_inputNumber)
        return;
    if (m_unfinishedExp == "") {
        if (m_hisRevision == -1) {
            m_unfinishedExp = m_inputEdit->text() + text;
            m_listModel->updataList(m_unfinishedExp, -1);
            if (!m_hisLink.isEmpty())
                if (m_hisLink.last().linkedItem == -1)
                    m_hisLink.last().linkedItem = m_listModel->rowCount(QModelIndex()) - 1;
        } else {
            int cur = m_inputEdit->cursorPosition();
            QString inputText = m_inputEdit->text();
            inputText.insert(cur,text);
            m_inputEdit->setText(inputText);
        }
    } else {
        m_unfinishedExp = m_unfinishedExp + m_inputEdit->text() + text;
        QString result;
        computationalResults(m_unfinishedExp,result);
        m_listModel->updataList(m_unfinishedExp, m_listModel->rowCount(QModelIndex()) - 1);
    }
    m_isResult = true;
    m_isContinue = true;
    m_inputNumber = true;
}

void ExpressionBar::enterPointEvent()
{
    if (!m_isContinue) {
        if (cursorPosAtEnd()) {
            m_inputEdit->clear();
            m_inputEdit->insert("0.");
        } else {
            m_inputEdit->insert(".");
        }

        m_isContinue = true;
    } else {
        m_inputEdit->insert(".");
    }
}

void ExpressionBar::enterBackspaceEvent()
{
    m_inputEdit->backspace();

    m_isContinue = true;
    m_isAllClear = false;
}

void ExpressionBar::enterClearEvent()
{
    if (m_isAllClear) {
        m_listModel->clearItems();
        m_listView->reset();
        m_isAllClear = false;
        m_unfinishedExp.clear();
        m_isAutoComputation = false;
        m_hisRevision = -1;
        m_hisLink.clear();

        emit clearStateChanged(false);
    } else {
        m_inputEdit->clear();
        m_isAllClear = true;
        clearLinkageCache();

        emit clearStateChanged(true);
    }
    m_Selected = -1;
}

void ExpressionBar::enterEqualEvent()
{
    QString resultText;
    int index;
    historicalLinkageIndex hisIndex;
    if (m_unfinishedExp == "" && m_hisRevision == -1) {
        resultText = m_inputEdit->text();
        index = -1;
        hisIndex.linkageTerm = 0;
    }
    if (m_hisRevision != -1) {
        resultText = m_inputEdit->text();
        index = m_hisRevision;
        //m_hisRevision = -1;
    } else {
        if (!m_inputEdit->text().isEmpty()) {
            m_unfinishedExp += m_inputEdit->text();
        }
        resultText = m_unfinishedExp;
        m_unfinishedExp.clear();
        index = m_listModel->rowCount(QModelIndex()) - 1;
    }
    hisIndex.linkageTerm = index;
    resultText = formatExpression(resultText);
    QString newResultText = completedBracketsCalculation(resultText);
    m_evaluator->setExpression(newResultText);
    Quantity ans = m_evaluator->evalUpdateAns();

    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        const QString result = DMath::format(ans, Quantity::Format::Fixed());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+").replace(QString::fromUtf8("－"), "-")
                                   .replace(QString::fromUtf8("×"), "*").replace(QString::fromUtf8("÷"), "/")
                                   .replace(QString::fromUtf8(","), "");

        //if (formatResult != m_inputEdit->text()) {
            m_listModel->updataList(resultText + "＝" + formatResult, index);
            m_inputEdit->setAnswer(formatResult, ans);
            m_isContinue = false;
        //}
            if (m_hisRevision != -1) {
                historicalLinkage(m_hisRevision, result);
            } else {
                hisIndex.linkageValue = formatResult;
                m_hisLink.push_back(hisIndex);
            }
    } else {
        m_listModel->updataList(resultText + "＝" + tr("Expression Error"), index);
    }

    m_listView->scrollToBottom();
    m_isResult = false;
    m_hisRevision = -1;
}

void ExpressionBar::enterBracketsEvent()
{
    if (!m_isContinue) {
        m_inputEdit->clear();
    }

    QString oldText, bracketsText;
    if (m_unfinishedExp == "") {
        oldText = m_inputEdit->text();
        int currentPos = m_inputEdit->cursorPosition();
        int right = oldText.length() - currentPos;
        int leftLeftParen = oldText.left(currentPos).count("(");
        int leftRightParen = oldText.left(currentPos).count(")");
        int rightLeftParen = oldText.right(right).count("(");
        int rightrightParen = oldText.right(right).count(")");
        //左右括号总数是否相等
        if (oldText.count("(") != oldText.count(")")) {
            //光标左侧左括号大于右括号
            if (leftLeftParen > leftRightParen) {
                if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) > 0) {
                    bracketsText = ")";
                    //m_inputEdit->insert(")");
                } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0) {
                    bracketsText = "(";
                    //m_inputEdit->insert("(");
                    //m_inputEdit->setCursorPosition(currentPos + 1);
                } else {
                    bracketsText = "(";
                    //m_inputEdit->insert("(");
                    //m_inputEdit->setCursorPosition(currentPos + 1);
                }
            //如果左侧左括号小于等于左侧右括号
            } else {
                //如果右侧左括号小于右括号
                if (rightLeftParen < rightrightParen) {
                    bracketsText = "(";
                    //m_inputEdit->insert("(");
                } else {
                    bracketsText = "(";
                    //m_inputEdit->insert("(");
                }
            }
        //相等则输入一对括号
        } else {
            bracketsText = "(";
            //m_inputEdit->insert("(");
        }
        if (m_hisRevision == -1) {
            m_unfinishedExp = m_inputEdit->text() + bracketsText;
            m_listModel->updataList(m_unfinishedExp, -1);
        } else {
            int cur = m_inputEdit->cursorPosition();
            QString inputText = m_inputEdit->text();
            inputText.insert(cur,bracketsText);
            m_inputEdit->setText(inputText);
        }
    }
    else {
        oldText = m_unfinishedExp;
        int leftBracket = oldText.count("(");
        int rightBracket = oldText.count(")");
        if (leftBracket - rightBracket == 0) {
            bracketsText = "(";
            m_unfinishedExp += bracketsText;
            m_inputEdit->clear();
        } else {
            bracketsText = ")";
            QString result, Computational;
            m_unfinishedExp = m_unfinishedExp + m_inputEdit->text() + bracketsText;
            Computational = m_unfinishedExp;
            for (int i = 0; i < leftBracket - rightBracket; ++i) {
                Computational += bracketsText;
            }
            computationalResults(Computational,result);
        }
        m_listModel->updataList(m_unfinishedExp, m_listModel->rowCount(QModelIndex()) - 1);
    }

    m_isAllClear = false;
    m_isContinue = true;
}

void ExpressionBar::copyResultToClipboard()
{
    if (m_inputEdit->text().isEmpty())
        return;

    const QString expression = formatExpression(m_inputEdit->expressionText());
    m_evaluator->setExpression(expression);
    Quantity ans = m_evaluator->evalUpdateAns();

    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        const QString result = DMath::format(ans, Quantity::Format::Fixed());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace('-', "－").replace('+', "＋");
        // m_inputEdit->setAnswer(formatResult, ans);

        QApplication::clipboard()->setText(formatResult);
    } else {
        QApplication::clipboard()->setText(m_inputEdit->text());
    }
}

void ExpressionBar::handleTextChanged(const QString &text)
{
    m_isAllClear = false;
    m_isContinue = true;

    emit clearStateChanged(false);
}

bool ExpressionBar::cursorPosAtEnd()
{
    return m_inputEdit->cursorPosition() == m_inputEdit->text().length();
}

QString ExpressionBar::formatExpression(const QString &text)
{
    return QString(text).replace(QString::fromUtf8("＋"), "+").replace(QString::fromUtf8("－"), "-")
                        .replace(QString::fromUtf8("×"), "*").replace(QString::fromUtf8("÷"), "/")
                        .replace(QString::fromUtf8(","), "");
}

void ExpressionBar::revisionResults(const QModelIndex &index)
{
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString("＝"), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = historic.at(0);
    m_hisRevision = index.row();
    m_inputEdit->setText(expression);
    m_Selected = m_hisRevision;
}

void ExpressionBar::computationalResults(const QString &expression, QString &result)
{
    if (m_inputEdit->text().isEmpty())
        return;

    QString exp = expression.left(expression.size() - 1);
    exp = formatExpression(exp);
    m_evaluator->setExpression(formatExpression(exp));
    Quantity ans = m_evaluator->evalUpdateAns();

    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        const QString tResult = DMath::format(ans, Quantity::Format::Fixed());
        result = Utils::formatThousandsSeparators(tResult);
        result = formatExpression(result);
        m_inputEdit->setAnswer(result,ans);

        if (result != m_inputEdit->text()) {
            m_isContinue = false;
        }
    } else {
        result = tr("Expression Error");
        m_inputEdit->setText(result);
    }
}

QString ExpressionBar::completedBracketsCalculation(QString &text)
{
    int leftBrack = text.count("(");
    int rightBrack = text.count(")");
    QString newText = text;
    if (leftBrack > rightBrack) {
        for (int i = 0;i < leftBrack - rightBrack;i++)
            newText += ")";
    }
    return newText;
}

void ExpressionBar::historicalLinkage(int index, QString newValue)
{
    for (int i = 0;i < m_hisLink.size();i++) {
        if (m_hisLink[i].linkageTerm == index && m_hisLink[i].isLink) {
            QString text = m_listModel->index(m_hisLink[i].linkedItem).data(SimpleListModel::ExpressionRole).toString();
            QString expression = text.split("＝").first();
            QString subStr = m_hisLink[i].linkageValue;
            expression.replace(expression.indexOf(subStr), subStr.size(), newValue);
            QString result;
            expression = formatExpression(expression);
            m_evaluator->setExpression(formatExpression(expression));
            Quantity ans = m_evaluator->evalUpdateAns();

            if (m_evaluator->error().isEmpty()) {
                if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
                    return;

                const QString tResult = DMath::format(ans, Quantity::Format::Fixed());
                result = Utils::formatThousandsSeparators(tResult);
                result = formatExpression(result);
                m_inputEdit->setAnswer(result,ans);

                if (result != m_inputEdit->text()) {
                    m_isContinue = false;
                }
            } else {
                result = tr("Expression Error");
                m_inputEdit->setText(result);
            }
            m_hisLink[i].linkageValue = newValue;
            m_listModel->updataList(expression + "＝" + result,m_hisLink[i].linkedItem);
        }
    }
}

void ExpressionBar::setLinkState(const QModelIndex index)
{
    int row = index.row();
    for (int i = 0;i < m_hisLink.size();i++) {
        if (m_hisLink[i].linkageTerm == row)
            m_hisLink[i].isLink = true;
    }
}

void ExpressionBar::clearLinkageCache()
{
    if (m_hisLink.isEmpty())
        return;
    if (m_hisLink.last().linkedItem == -1)
        m_hisLink.removeLast();
}

void ExpressionBar::settingLinkage()
{
    enterEqualEvent();
    QString result = m_hisLink.last().linkageValue;
    m_listModel->updataList(result, -1);
    m_hisLink.last().isLink = true;
    m_hisLink.last().linkedItem = m_listModel->rowCount(QModelIndex()) - 1;
    m_unfinishedExp = result;
    m_inputEdit->clear();
    m_listDelegate->setHisLink(m_hisLink.last().linkageTerm, m_hisLink.last().linkedItem);
}

void ExpressionBar::settingLinkage(const QModelIndex &index)
{
    int row = index.row();
    for (int i = 0;i < m_hisLink.size();i++) {
        if (m_hisLink[i].linkageTerm == row) {
            m_hisLink[i].isLink = true;
            m_inputEdit->clear();
        }
    }
}
