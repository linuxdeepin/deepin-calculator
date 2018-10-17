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
      m_isAllClear(false)
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
    m_inputEdit->insert(text);
    m_isContinue = true;
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

        emit clearStateChanged(false);
    } else {
        m_inputEdit->clear();
        m_isAllClear = true;

        emit clearStateChanged(true);
    }
}

void ExpressionBar::enterEqualEvent()
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

        if (formatResult != m_inputEdit->text()) {
            m_listModel->appendText(m_inputEdit->text() + "＝" + formatResult);
            m_inputEdit->setAnswer(formatResult, ans);
            m_isContinue = false;
        }
    } else {
        m_listModel->appendText(m_inputEdit->text() + "＝" + tr("Expression Error"));
    }

    m_listView->scrollToBottom();
}

void ExpressionBar::enterBracketsEvent()
{
    if (!m_isContinue) {
        m_inputEdit->clear();
    }

    int currentPos = m_inputEdit->cursorPosition();
    m_inputEdit->insert("()");
    m_inputEdit->setCursorPosition(currentPos + 1);

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
