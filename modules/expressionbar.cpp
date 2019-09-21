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

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include "expressionbar.h"
#include "../utils.h"

ExpressionBar::ExpressionBar(QWidget *parent)
    : DWidget(parent),
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
      m_isLinked(false),
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
    if (m_isLinked)
        clearLinkageCache();
    if (m_inputNumber && m_hisRevision == -1) {
        m_inputEdit->clear();
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

    m_inputNumber = false;
    m_inputEdit->insert(text);
    emit clearStateChanged(false);
}

void ExpressionBar::enterSymbolEvent(const QString &text)
{
    if (m_isLinked) {
        m_hisLink.last().linkedItem = m_listModel->rowCount(QModelIndex());
        m_hisLink.last().isLink = true;
        m_listDelegate->setHisLinked(m_hisLink.last().linkedItem);
        m_isLinked = false;
    }
    if (m_inputEdit->text().isEmpty()) {
        if (text != QString::fromUtf8("－")) {
            return;
        } else {
            m_inputEdit->insert(text);
        }
    } else {
        int curPos = m_inputEdit->cursorPosition();
        QString exp = m_inputEdit->text();
        if (cursorPosAtEnd()) {
            QString lastStr = exp.right(1);
            if (isOperator(lastStr))
                exp.chop(1);
            m_inputEdit->setText(exp + text);
        } else if (curPos == 0) {
            QString firstStr = exp.left(1);
            if (firstStr == QString::fromUtf8("－")) {
                return;
            } else {
                if (text == QString::fromUtf8("－"))
                    m_inputEdit->insert(text);
            }
        } else {
            QString infront = exp.at(curPos - 1);
            QString behand = exp.at(curPos);
            if (isOperator(infront) || isOperator(behand)) {
                return;
            } else {
                m_inputEdit->insert(text);
            }
        }
    }
    m_isContinue = true;
}

void ExpressionBar::enterPercentEvent()
{
    if (m_isLinked)
        clearLinkageCache();
    if (m_inputEdit->text().isEmpty())
        return;
    QString exp = m_inputEdit->text();
    int curPos = m_inputEdit->cursorPosition();
    if (curPos == 0)
        return;
    QString sRegNum = "[0-9]+";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (cursorPosAtEnd()) {
        if (rx.exactMatch(exp.right(1)))
            m_inputEdit->insert("%");
    } else {
        QString infront = exp.at(curPos - 1);
        QString behand = exp.at(curPos);
        if (rx.exactMatch(infront) && behand != "%")
            m_inputEdit->insert("%");
    }
    m_listView->scrollToBottom();
    m_isContinue = true;
}

void ExpressionBar::enterPointEvent()
{
    if (m_isLinked)
        clearLinkageCache();
    if (m_inputEdit->cursorPosition() == 0) {
        m_inputEdit->insert("0.");
    } else {
        m_inputEdit->insert(".");
    }
    /*if (!m_isContinue) {
        if (cursorPosAtEnd()) {
            m_inputEdit->clear();
            m_inputEdit->insert("0.");
        } else {
            m_inputEdit->insert(".");
        }

        m_isContinue = true;
    } else {
        m_inputEdit->insert(".");
    }*/
}

void ExpressionBar::enterBackspaceEvent()
{
    if (m_isResult)
        clearLinkageCache();
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
        m_listDelegate->removeAllLink();

        emit clearStateChanged(false);
    } else {
        m_inputEdit->clear();
        m_isAllClear = true;
        clearLinkageCache();

        emit clearStateChanged(true);
    }
    m_isResult = false;
    m_Selected = -1;
}

void ExpressionBar::enterEqualEvent()
{
    /*QString resultText;
    int index;
    if (m_unfinishedExp == "" && m_hisRevision == -1) {
        resultText = m_inputEdit->text();
        index = -1;
    }
    //修改历史记录
    if (m_hisRevision != -1) {
        resultText = m_inputEdit->text();
        index = m_hisRevision;
    } else {
        if (!m_inputEdit->text().isEmpty() && !m_isResult) {
            m_unfinishedExp += m_inputEdit->text();
        }
        resultText = m_unfinishedExp;
        index = m_listModel->rowCount(QModelIndex()) - 1;
        QString tmp = m_listModel->index(index).data(SimpleListModel::ExpressionRole).toString();
        if (tmp.indexOf("＝") != -1)
            ++index;
    }
    m_unfinishedExp.clear();
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
        historicalLinkage(m_hisRevision,formatResult);
    } else {
        m_listModel->updataList(resultText + "＝" + tr("Expression Error"), index);
    }

    m_hisRevision = -1;
    m_listView->scrollToBottom();
    m_isResult = false;*/
    if (m_inputEdit->text().isEmpty())
        return;

    if (m_hisRevision == -1) {
        const QString expression = formatExpression(m_inputEdit->expressionText());
        m_evaluator->setExpression(expression);
    } else {
        const QString expression = formatExpression(m_inputEdit->text());
        m_evaluator->setExpression(expression);
    }
    Quantity ans = m_evaluator->evalUpdateAns();

    QString newResult;
    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        const QString result = DMath::format(ans, Quantity::Format::Fixed());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+").replace(QString::fromUtf8("－"), "-")
                                   .replace(QString::fromUtf8("×"), "*").replace(QString::fromUtf8("÷"), "/")
                                   .replace(QString::fromUtf8(","), "");

        if (formatResult != m_inputEdit->text()) {
            m_listModel->updataList(m_inputEdit->text() + "＝" + formatResult, m_hisRevision);
            m_inputEdit->setAnswer(formatResult, ans);
            newResult = formatResult;
            m_isContinue = false;
        }
    } else {
        m_listModel->updataList(m_inputEdit->text() + "＝" + tr("Expression Error"), m_hisRevision);
    }
    if (m_isLinked) {
        if (m_hisRevision == -1) {
            m_isLinked = false;
            return;
        }
        for (int i = m_hisRevision; i < m_hisLink.size(); ++i) {
            if (m_hisRevision == m_hisLink[i].linkageTerm) {
                m_hisRevision = m_hisLink[i].linkedItem;
                if (m_hisRevision == -1) {
                    m_hisLink.remove(i);
                    m_listDelegate->removeHisLink();
                    break;
                }
                QString text = m_listModel->index(m_hisRevision).data(SimpleListModel::ExpressionRole).toString();
                QString linkedExp = text.split("＝").first();
                int length = m_hisLink[i].linkageValue.length();
                if (linkedExp.left(length) != m_hisLink[i].linkageValue || !isOperator(linkedExp.at(length))) {
                    m_hisLink.remove(i);
                    m_listDelegate->removeHisLink();
                    break;
                }
                if (newResult.isEmpty())
                    break;
                m_hisLink[i].linkageValue = newResult;
                QString newText = newResult + linkedExp.right(linkedExp.length() - length);
                m_inputEdit->setText(newText);
                enterEqualEvent();
            }
        }
    }

    m_hisRevision = -1;
    m_listView->scrollToBottom();
    m_isLinked = false;
}

void ExpressionBar::enterBracketsEvent()
{
    QString oldText, bracketsText;
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
                //bracketsText = ")";
                m_inputEdit->insert(")");
            } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0) {
                //bracketsText = "(";
                m_inputEdit->insert("(");
                m_inputEdit->setCursorPosition(currentPos + 1);
            } else {
                //bracketsText = "(";
                m_inputEdit->insert("(");
                m_inputEdit->setCursorPosition(currentPos + 1);
            }
        //如果左侧左括号小于等于左侧右括号
        } else {
            //如果右侧左括号小于右括号
            if (rightLeftParen < rightrightParen) {
                //bracketsText = "(";
                m_inputEdit->insert("(");
            } else {
                //bracketsText = "(";
                m_inputEdit->insert("(");
            }
        }
    //相等则输入一对括号
    } else {
        //bracketsText = "(";
        m_inputEdit->insert("(");
    }
    return;
    /*if (!m_isContinue) {
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
            m_unfinishedExp = m_inputEdit->text().insert(currentPos, bracketsText);
            m_listModel->updataList(m_unfinishedExp, -1);
            m_inputEdit->clear();
        } else {
            int cur = m_inputEdit->cursorPosition();
            QString inputText = m_inputEdit->text();
            inputText.insert(cur,bracketsText);
            m_inputEdit->setText(inputText);
        }
    } else {
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
    m_isContinue = true;*/
}

void ExpressionBar::copyResultToClipboard()
{
    if (m_inputEdit->text().isEmpty())
        return;
    QApplication::clipboard()->setText(m_inputEdit->selectedText());

    /*const QString expression = formatExpression(m_inputEdit->expressionText());
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
    }*/
}

void ExpressionBar::copyClipboard2Result()
{
    QString text = QApplication::clipboard()->text();
    m_inputEdit->insert(text);
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
            if (m_hisLink[i].linkedItem == -1)
                return;
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
            historicalLinkage(m_hisLink[i].linkedItem, result);
            m_isLinked = false;
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
    if (m_hisLink.last().linkedItem == -1) {
        m_hisLink.removeLast();
        m_isLinked = false;
        m_listDelegate->removeHisLink();
    }
}

void ExpressionBar::settingLinkage()
{
    const int hisRecision = m_hisRevision;
    if (hisRecision != -1) {
        for (int i = 0;i < m_hisLink.size();i++) {
            if (m_hisLink[i].linkageTerm == hisRecision) {
                m_isLinked = true;
                enterEqualEvent();
                return;
            }
        }
        historicalLinkageIndex hisIndex;
        hisIndex.linkageTerm = hisRecision;
        //hisIndex.isLink = true;
        enterEqualEvent();
        QString exp = m_listModel->index(hisRecision).data(SimpleListModel::ExpressionRole).toString();
        hisIndex.linkageValue = exp.split("＝").last();
        m_hisLink.push_back(hisIndex);
    } else {
        if (!m_hisLink.isEmpty() && m_hisLink.last().linkedItem == -1)
            return;
        enterEqualEvent();
        m_isLinked = true;
        historicalLinkageIndex hisIndex;
        //hisIndex.isLink = true;
        hisIndex.linkageTerm = m_listModel->rowCount(QModelIndex()) - 1;
        QString exp = m_listModel->index(hisIndex.linkageTerm).data(SimpleListModel::ExpressionRole).toString();
        hisIndex.linkageValue = exp.split("＝").last();
        m_hisLink.push_back(hisIndex);
    }
    m_listDelegate->setHisLink(m_hisLink.last().linkageTerm);
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

bool ExpressionBar::isOperator(const QString &text)
{
    if (text == QString::fromUtf8("＋") || text == QString::fromUtf8("－") ||
           text == QString::fromUtf8("×") || text == QString::fromUtf8("÷")) {
        return true;
    } else {
        return false;
    }
}
