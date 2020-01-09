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
#include <QTimer>
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
      m_isUndo(false),
      m_Selected(-1)
{
    // init inputEdit attributes.
    m_inputEdit->setFixedHeight(55);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);

    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    DPalette pl = this->palette();
    pl.setColor(DPalette::Light,QColor(0,0,0,0));
    pl.setColor(DPalette::Dark,QColor(0,0,0,0));
    pl.setColor(DPalette::Base,QColor(0,0,0,0));
    this->setPalette(pl);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);

    setMinimumHeight(160);
    initConnect();
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
        clearLinkageCache();
    }
    if (!m_isContinue) {
        // the cursor position at the end, it will clear edit text.

        if (cursorPosAtEnd())
            m_inputEdit->clear();

        m_isContinue = true;
        m_listView->setFocus();
    }

    m_inputNumber = false;
    m_isUndo = false;
    QString oldText = m_inputEdit->text();
    SSelection selection = m_inputEdit->getSelection();
    if (selection.selected != "") {
        QString exp = m_inputEdit->text();
        exp.remove(selection.curpos, selection.selected.size());
        exp.insert(selection.curpos, text);
        m_inputEdit->setText(pointFaultTolerance(exp));
    } else {
        m_inputEdit->insert(text);
    }
    emit clearStateChanged(false);
}

void ExpressionBar::enterSymbolEvent(const QString &text)
{
    if (!m_hisLink.isEmpty() && m_hisLink.last().linkedItem == -1) {
        m_hisLink.last().linkedItem = m_listModel->rowCount(QModelIndex());
        m_hisLink.last().isLink = true;
        m_listDelegate->setHisLinked(m_hisLink.last().linkedItem);
        m_isLinked = false;
        if (m_hisLink.size() > 9) {
            m_hisLink.removeFirst();
            m_listDelegate->removeLine(0);
        }
    }
    clearSelectSymbol();
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text = m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm =row - 1;
                his.linkageValue =result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    m_isUndo = false;
    QString oldText = m_inputEdit->text();
    SSelection selection = m_inputEdit->getSelection();
    if (selection.selected != "") {
        QString exp = oldText;
        exp.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(selection.curpos);
    }
    if (m_inputEdit->text().isEmpty()) {
        if (text != "-") {
            m_inputEdit->setText(oldText);
        } else {
            m_inputEdit->insert(text);
        }
    } else {
        if (m_inputEdit->text() == "－")
            m_inputEdit->setText(oldText);
        getSelection();
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
                m_inputEdit->setText(oldText);
            } else {
                if (text == QString::fromUtf8("－") || text == "-")
                    m_inputEdit->insert(text);
            }
        } else {
            QString infront = exp.at(curPos - 1);
            QString behand = exp.at(curPos);
            if (isOperator(infront) || isOperator(behand)) {
                m_inputEdit->setText(oldText);
            } else {
                m_inputEdit->insert(text);
            }
        }
    }
    //if (m_inputEdit->text() == oldText)
    //    m_inputEdit->setText(oldText);
    m_isContinue = true;
}

void ExpressionBar::enterPercentEvent()
{
    if (m_isLinked)
        clearLinkageCache();
    if (m_inputEdit->text().isEmpty())
        return;
    QString oldText = m_inputEdit->text();
    QString exp = m_inputEdit->text();
    SSelection selection = m_inputEdit->getSelection();
    int curPos = m_inputEdit->cursorPosition();
    if (selection.selected != "") {
        exp.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(exp);
        if (curPos > selection.curpos && curPos <= selection.curpos + selection.selected.size())
            curPos = selection.curpos;
        m_inputEdit->setCursorPosition(curPos);
    }
    if (curPos == 0)
        return;
    QString sRegNum = "[0-9]+";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (cursorPosAtEnd()) {
        if (rx.exactMatch(exp.right(1)))
            m_inputEdit->insert("%");
        else
            m_inputEdit->setText(oldText);
    } else {
        QString infront = exp.at(curPos - 1);
        QString behand = exp.at(curPos);
        if (rx.exactMatch(infront) && behand != "%")
            m_inputEdit->insert("%");
        else
            m_inputEdit->setText(oldText);
    }
    m_listView->scrollToBottom();
    m_isContinue = true;
    m_isUndo = false;
}

void ExpressionBar::enterPointEvent()
{
    if (m_isLinked)
        clearLinkageCache();
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    if (curpos == 0) {
        m_inputEdit->insert("0.");
    } else {
        if (exp.at(curpos - 1) == ".")
            return;
        if (exp.at(curpos - 1) != ")" && exp.at(curpos - 1) != "%") {
            QString sRegNum = "[0-9]+";
            QRegExp rx;
            rx.setPattern(sRegNum);
            if (rx.exactMatch(exp.at(curpos - 1)))
                m_inputEdit->insert(".");
            else
                m_inputEdit->insert("0.");
        }
    }
    exp = pointFaultTolerance(m_inputEdit->text());
    if (exp != m_inputEdit->text())
        m_inputEdit->setText(exp);
    m_isUndo = false;
}

void ExpressionBar::enterBackspaceEvent()
{
    if (m_isResult)
        clearLinkageCache();
    //m_inputEdit->backspace();
    SSelection selection = m_inputEdit->getSelection();
    if (selection.selected != "") {
        m_inputEdit->setText(m_inputEdit->text().remove(selection.curpos, selection.selected.size()));
        return;
    }
    QString text = m_inputEdit->text();
    int cur = m_inputEdit->cursorPosition();
    if (text.size() > 0 && text[cur - 1] == ",") {
        text.remove(cur - 2, 2);
        m_inputEdit->setText(text);
        m_inputEdit->setCursorPosition(cur - 2);
    } else
        m_inputEdit->backspace();
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }

    QTimer::singleShot(5000, this, [=] {
            int curpos = m_inputEdit->cursorPosition();
            m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
            m_inputEdit->setCursorPosition(curpos);
        });

    m_isContinue = true;
    m_isUndo = false;
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
        if (m_listModel->rowCount(QModelIndex()) == 0)
            emit clearStateChanged(false);
        else
            emit clearStateChanged(true);

        m_inputEdit->clear();
        m_isAllClear = true;
        clearLinkageCache();
    }
    m_isResult = false;
    m_isUndo = false;
    m_Selected = -1;
    addUndo();
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
        QString exp = symbolComplement(expression);
        m_evaluator->setExpression(exp);
    } else {
        const QString expression = formatExpression(m_inputEdit->text());
        QString exp = symbolComplement(expression);
        m_evaluator->setExpression(exp);
    }
    Quantity ans = m_evaluator->evalUpdateAns();

    QString newResult;
    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        const QString result = DMath::format(ans, Quantity::Format::Fixed());
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+").replace(QString::fromUtf8("－"), "-")
                                   .replace(QString::fromUtf8("×"), "*").replace(QString::fromUtf8("÷"), "/");
                                   //.replace(QString::fromUtf8(","), "");

        QString tStr = m_inputEdit->text().replace(QString::fromUtf8(","), "");
        if (formatResult != tStr) {
            m_listModel->updataList(m_inputEdit->text() + "＝" + formatResult, m_hisRevision);
            m_inputEdit->setAnswer(formatResult, ans);
            newResult = formatResult;
        }
        m_isContinue = false;
    } else {
        m_listModel->updataList(m_inputEdit->text() + "＝" + tr("Expression error"), m_hisRevision);
        if (m_hisRevision == -1)
            m_hisRevision = m_listModel->rowCount(QModelIndex()) - 1;
        else {
            for (int i = 0; i < m_hisLink.size(); ++i) {
                if (m_hisLink[i].linkageTerm == m_hisRevision) {
                    //m_listDelegate->removeLine(m_hisLink[i].linkageTerm, m_hisLink[i].linkedItem);
                    m_listDelegate->removeLine(i);
                    m_hisLink.removeAt(i);
                    --i;
                }
            }
        }
    }

    if (m_isLinked) {
        if (m_hisRevision == -1) {
            m_isLinked = false;
            return;
        }
        for (int i = 0; i < m_hisLink.size(); ++i) {
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
                newResult = newResult.replace("+", QString::fromUtf8("＋")).replace("-", QString::fromUtf8("－"));
                m_hisLink[i].linkageValue = newResult;
                QString newText = newResult + linkedExp.right(linkedExp.length() - length);
                m_inputEdit->setText(newText);
                enterEqualEvent();
            }
        }
        if (m_evaluator->error().isEmpty())
            m_inputEdit->clear();
    }

    if (m_evaluator->error().isEmpty())
        m_hisRevision = -1;
    m_listView->scrollToBottom();
    m_isLinked = false;
    m_isResult = true;
    m_isUndo = false;
}

void ExpressionBar::enterBracketsEvent()
{
    replaceSelection(m_inputEdit->text());
    int currentPos = m_inputEdit->cursorPosition();
    m_inputEdit->insert("()");
    m_inputEdit->setCursorPosition(currentPos + 1);
    m_isUndo = false;
    /*QString sRegNum = "[0-9]+";
    QRegExp rx;
    rx.setPattern(sRegNum);
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
                //if (currentPos != 0 && !rx.exactMatch(oldText.at(currentPos - 1)))
                //    return;
                m_inputEdit->insert(")");
            } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0) {
                //bracketsText = "(";
                //if (currentPos != 0 && rx.exactMatch(oldText.at(currentPos - 1)))
                //    return;
                m_inputEdit->insert("(");
                m_inputEdit->setCursorPosition(currentPos + 1);
            } else {
                //bracketsText = "(";
                //if (currentPos != 0 && rx.exactMatch(oldText.at(currentPos - 1)))
                //    return;
                m_inputEdit->insert("(");
                m_inputEdit->setCursorPosition(currentPos + 1);
            }
        //如果左侧左括号小于等于左侧右括号
        } else {
            //如果右侧左括号小于右括号
            if (rightLeftParen < rightrightParen) {
                //bracketsText = "(";
                //if (currentPos != 0 && rx.exactMatch(oldText.at(currentPos - 1)))
                //    return;
                m_inputEdit->insert("(");
            } else {
                //bracketsText = "(";
                //if (currentPos != 0 && rx.exactMatch(oldText.at(currentPos - 1)))
                //    return;
                m_inputEdit->insert("(");
            }
        }
    //相等则输入一对括号
    } else {
        //bracketsText = "(";
        //if (currentPos != 0 && rx.exactMatch(oldText.at(currentPos - 1)))
        //    return;
        m_inputEdit->insert("(");
    }*/
}

void ExpressionBar::enterLeftBracketsEvent()
{
    replaceSelection(m_inputEdit->text());
    m_inputEdit->insert("(");
    m_isUndo = false;
}

void ExpressionBar::enterRightBracketsEvent()
{
    replaceSelection(m_inputEdit->text());
    m_inputEdit->insert(")");
    m_isUndo = false;
}

void ExpressionBar::enterDeleteEvent()
{
    m_inputEdit->clear();
    m_isUndo = false;
    /*int curPos = m_inputEdit->cursorPosition();
    if (curPos != m_inputEdit->text().length()) {
        QString text = m_inputEdit->text();
        int index = curPos;
        if (text.at(curPos) == ",")
            ++index;
        text.remove(index, 1);
        m_inputEdit->setText(text);
        m_inputEdit->setCursorPosition(curPos);
    }*/
}

void ExpressionBar::entereEvent()
{
    /*QString exp = m_inputEdit->text();
    if(exp.isEmpty() || m_inputEdit->cursorPosition() == 0)
        return;
    QString sRegNum = "[0-9]+";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (rx.exactMatch(exp.at(m_inputEdit->cursorPosition() - 1)))
        m_inputEdit->insert("e");*/
    m_inputEdit->insert("e");
    m_isUndo = false;
}

void ExpressionBar::copyResultToClipboard()
{
    if (m_inputEdit->text().isEmpty())
        return;
    //QApplication::clipboard()->setText(m_inputEdit->selectedText());

    const QString expression = formatExpression(m_inputEdit->text());
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

void ExpressionBar::copyClipboard2Result()
{
    QString oldText = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    replaceSelection(oldText);
    QString exp = m_inputEdit->text();
    QString text = QApplication::clipboard()->text();
    text.remove("e");
    text = pasteFaultTolerance(text);
    //m_inputEdit->insert(text);
    m_inputEdit->setText(text);
    clearLinkageCache();
    m_isUndo = false;
    if (m_inputEdit->text() == exp) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curpos);
        qDebug() << "Invalid content";
    }
    if (!m_inputEdit->text().isEmpty())
        emit clearStateChanged(false);
}

void ExpressionBar::allElection()
{
    m_inputEdit->selectAll();
}

void ExpressionBar::shear()
{
    QString text = m_inputEdit->text();
    QString selectText = m_inputEdit->selectedText();
    selectText = selectText.replace(",", "");
    QApplication::clipboard()->setText(selectText);
    int start = m_inputEdit->selectionStart();
    int length = m_inputEdit->selectionLength();
    text.remove(start,length);
    m_inputEdit->setText(text);
    addUndo();
    m_isUndo = false;
}

void ExpressionBar::handleTextChanged(const QString &text)
{
    m_isAllClear = false;
    m_isContinue = true;
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
    clearLinkageCache();
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString("＝"), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = historic.at(0);
    m_hisRevision = index.row();
    m_inputEdit->setText(expression);
    m_Selected = m_hisRevision;
    m_isResult = false;

    emit clearStateChanged(false);
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
        result = tr("Expression error");
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
                result = tr("Expression error");
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
        m_isResult = false;
    }
}

void ExpressionBar::settingLinkage()
{
    const int hisRecision = m_hisRevision;
    if (hisRecision != -1) {
        for (int i = 0;i < m_hisLink.size();i++) {
            if (m_hisLink[i].linkedItem == hisRecision) {
                if (cancelLink(i+1) && i != 0)
                    --i;
                if (m_hisLink.size() == 0)
                    break;
            }
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
        //judgeLinkageAgain();
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

void ExpressionBar::judgeLinkageAgain()
{
    if (m_hisLink.isEmpty())
        return;
    QString text = m_inputEdit->text();
    text.replace(",","");
    QStringList list = text.split(QRegExp("[＋－×÷()]"));
    QString linkValue = m_hisLink.last().linkageValue;
    if (list.at(0) != linkValue) {
        //m_listDelegate->removeLine(m_hisLink.last().linkageTerm,m_hisLink.last().linkedItem);
        m_listDelegate->removeLine(m_hisLink.size() - 1);
        m_hisLink.removeLast();
    }
}

void ExpressionBar::initConnect()
{
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, this, &ExpressionBar::revisionResults);
    connect(m_inputEdit, &InputEdit::textChanged, this, &ExpressionBar::handleTextChanged);
    connect(m_inputEdit, &InputEdit::keyPress, this, &ExpressionBar::keyPress);
    connect(m_inputEdit, &InputEdit::equal, this, &ExpressionBar::enterEqualEvent);
    connect(m_inputEdit, &InputEdit::cut, this, &ExpressionBar::shear);
    connect(m_inputEdit, &InputEdit::copy, this, &ExpressionBar::copyResultToClipboard);
    connect(m_inputEdit, &InputEdit::paste, this, &ExpressionBar::copyClipboard2Result);
    connect(m_inputEdit, &InputEdit::deleteText, this, &ExpressionBar::enterClearEvent);
    connect(m_inputEdit, &InputEdit::selectAllText, this, &ExpressionBar::allElection);
    connect(m_inputEdit, &InputEdit::undo, this, &ExpressionBar::Undo);
    connect(m_inputEdit, &InputEdit::redo, this, &ExpressionBar::Redo);
    connect(m_inputEdit, &InputEdit::setResult, this, &ExpressionBar::setResultFalse);
}

QString ExpressionBar::symbolComplement(const QString exp)
{
    QString text = exp;
    int index = text.indexOf("(", 0);
    while (index != -1) {
        if (index >= 1 && text.at(index - 1).isNumber()) {
            text.insert(index, "×");
            ++index;
        }
        ++index;
        index = text.indexOf("(", index);
    }
    index = text.indexOf(")", 0);
    while (index != -1) {
        if (index < text.length() - 1 && text.at(index + 1).isNumber()) {
            text.insert(index + 1, "×");
            ++index;
        }
        ++index;
        index = text.indexOf(")", index);
    }
    return text;
}

QString ExpressionBar::pasteFaultTolerance(QString exp)
{
    exp = m_inputEdit->text().insert(m_inputEdit->cursorPosition(), exp);
    exp = pointFaultTolerance(exp);
    for (int i = 0; i < exp.size(); ++i) {
        while (exp[i].isNumber()) {
            if (exp[i] == "0" && exp[i+1] != "." && (i == 0 || !exp[i-1].isNumber()) && (exp.size() == 1 || exp[i+1].isNumber())) {
                exp.remove(i,1);
                --i;
            }
            ++i;
        }
        if (exp[i] == "." && (i == 0 || !exp[i-1].isNumber())) {
            exp.insert(i,"0");
            ++i;
        }
    }
    return exp;
}

QString ExpressionBar::pointFaultTolerance(const QString &text)
{
    QString oldText = text;
    //QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
    QString reformatStr = oldText.replace('+', QString::fromUtf8("＋"))
                             .replace('-', QString::fromUtf8("－"))
                             .replace("_", QString::fromUtf8("－"))
                             .replace('*', QString::fromUtf8("×"))
                             .replace('/', QString::fromUtf8("÷"))
                             .replace('x', QString::fromUtf8("×"))
                             .replace('X', QString::fromUtf8("×"))
                             .replace(QString::fromUtf8("（"), "(")
                             .replace(QString::fromUtf8("）"), ")")
                             .replace(QString::fromUtf8("。"), ".")
                             .replace(QString::fromUtf8("——"), QString::fromUtf8("－"));
    QStringList list = reformatStr.split(QRegExp("[＋－×÷()]"));
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        int firstPoint = item.indexOf(".");
        if (firstPoint == -1)
            continue;
        if (firstPoint == 0) {
            item.insert(firstPoint, "0");
            ++firstPoint;
            //oldText.replace(list[i], item);
        } else {
            if (item.at(firstPoint - 1) == ")" || item.at(firstPoint - 1) == "%") {
                item.remove(firstPoint, 1);
                reformatStr.replace(list[i], item);
            }
        }
        if (item.count(".") > 1) {
            item.remove(".");
            item.insert(firstPoint, ".");
            reformatStr.replace(list[i], item);
        }
    }
    return reformatStr;
}

void ExpressionBar::clearSelectSymbol()
{
    SSelection select = m_inputEdit->getSelection();
    if (select.selected.size() == 1 && (select.selected == "＋" || select.selected == "－"
                                        || select.selected == "×" || select.selected == "÷")) {
        select.selected = "";
        QString exp = m_inputEdit->text();
        exp.remove(select.curpos,1);
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(select.curpos);
    }
}

void ExpressionBar::Undo()
{
    if (m_undo.isEmpty())
        return;
    clearLinkageCache();
    m_redo.append(m_undo.last());
    m_inputEdit->setRedoAction(true);
    m_undo.removeLast();
    m_isUndo = true;
    if (!m_undo.isEmpty())
        m_inputEdit->setText(m_undo.last());
    else {
        m_inputEdit->clear();
        m_inputEdit->setUndoAction(false);
    }
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }
}

void ExpressionBar::addUndo()
{
    if (!m_undo.isEmpty() && m_inputEdit->text() == m_undo.last())
        return;
    m_undo.append(m_inputEdit->text());
    m_redo.clear();
    m_inputEdit->setUndoAction(true);
    SSelection selection;
    m_inputEdit->setSelection(selection);
}

void ExpressionBar::Redo()
{
    if (m_redo.isEmpty())
        return;
    clearLinkageCache();
    m_inputEdit->setText(m_redo.last());
    m_undo.append(m_inputEdit->text());
    m_redo.removeLast();
    if (m_redo.isEmpty())
        m_inputEdit->setRedoAction(false);
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }
}

void ExpressionBar::initTheme(int type)
{
    m_listDelegate->setThemeType(type);
}

void ExpressionBar::clearSelection()
{
    SSelection select = m_inputEdit->getSelection();
    if (!select.selected.isEmpty()) {
        int start = select.curpos - select.selected.length();
        QString exp = m_inputEdit->text();
        exp = exp.remove(start,select.selected.length());
        select.clearText = exp;
    }
}

void ExpressionBar::setSelection()
{
    SSelection select = m_inputEdit->getSelection();
    if (select.selected.isEmpty())
        return;
    if (m_inputEdit->text() == select.clearText)
        m_inputEdit->setText(select.oldText);
    select.selected = "";
}

void ExpressionBar::getSelection()
{
    if (!m_inputEdit->selectedText().isEmpty()) {
        int start = m_inputEdit->selectionStart();
        QString exp = m_inputEdit->text();
        exp = exp.remove(start,m_inputEdit->selectionLength());
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(start);
    }
}

void ExpressionBar::setResultFalse()
{
    m_isResult = false;
}

void ExpressionBar::replaceSelection(QString text)
{
    SSelection selection = m_inputEdit->getSelection();
    int curPos = m_inputEdit->cursorPosition();
    if (selection.selected != "") {
        text.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(text);
        if (curPos > selection.curpos && curPos <= selection.curpos + selection.selected.size())
            curPos = selection.curpos;
        m_inputEdit->setCursorPosition(curPos);
    }
}

bool ExpressionBar::cancelLink(int index)
{
    bool isRemove = false;
    for (int i = 0; i < index; ++i) {
        if (m_hisLink[i].linkedItem == m_hisRevision) {
            QString exp = m_inputEdit->text();
            exp = exp.replace(",", "");
            QStringList list = exp.split(QRegExp("[＋－×÷()]"));
            if (exp[0] == "－")
                list[0] = "－" + list[1];
            if (list.at(0) != m_hisLink[i].linkageValue) {
                //m_listDelegate->removeLine(m_hisLink[i].linkageTerm, m_hisLink[i].linkedItem);
                m_listDelegate->removeLine(i);
                m_hisLink.remove(i);
                isRemove = true;
            }
        }
    }
    return isRemove;
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

void ExpressionBar::moveLeft()
{
    m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - 1);
    m_inputEdit->setFocus();
}

void ExpressionBar::moveRight()
{
    m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() + 1);
    m_inputEdit->setFocus();
}

QLineEdit* ExpressionBar::getInputEdit()
{
    return m_inputEdit;
}
