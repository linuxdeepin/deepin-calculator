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

#include "inputedit.h"
#include "../utils.h"
#include "../math/floatconfig.h"

#include <QKeyEvent>
#include <QDebug>

InputEdit::InputEdit(QWidget *parent)
    : QLineEdit(parent),
      m_ans(0),
      m_ansStartPos(0),
      m_ansLength(0),
      m_ansVaild(false),
      m_currentInAns(false),
      m_currentOnAnsLeft(false),
      m_oldText("")
{
    setAttribute(Qt::WA_InputMethodEnabled, false);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::StrongFocus);
    autoZoomFontSize();

    connect(this, &QLineEdit::textChanged, this, &InputEdit::handleTextChanged);
    connect(this, &QLineEdit::cursorPositionChanged, this, &InputEdit::handleCursorPositionChanged);
    connect(this, &QLineEdit::selectionChanged,
            [=] {
                int pos = this->cursorPosition();
                this->cursorPositionChanged(pos, pos);
            });
}

InputEdit::~InputEdit()
{
}

QString InputEdit::expressionText()
{
    QString t = text();

    if (m_ansVaild) {
        QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
        t.remove(m_ansStartPos, m_ansLength);
        t.insert(m_ansStartPos, ans);
    }

    return t;
}

void InputEdit::setAnswer(const QString &str, const Quantity &ans)
{
    m_ans = ans;
    m_ansStartPos = 0;
    m_ansLength = str.length();
    m_oldText = "";
    setText(str);
}

void InputEdit::clear()
{
    m_ansLength = 0;
    setText("");
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    Q_EMIT keyPress(e);

    switch (e->key()) {
    case Qt::Key_Equal: case Qt::Key_Period:
        return;
    }

    QLineEdit::keyPressEvent(e);
}

void InputEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    QLineEdit::mouseDoubleClickEvent(e);

    if (e->button() == Qt::LeftButton) {
        int position = cursorPositionAt(e->pos());
        int posBegin = findWordBeginPosition(position);
        int posEnd = findWordEndPosition(position);
        setSelection(posBegin, posEnd - posBegin + 1);
    }
}

bool InputEdit::isSymbolCategoryChanged(int pos1, int pos2)
{
    QString str = text();
    QChar::Category category1 = str.at(pos1).category();
    QChar::Category category2 = str.at(pos2).category();

    if (category1 == QChar::Number_DecimalDigit || category1 == QChar::Punctuation_Other) {
        if (category2 == QChar::Number_DecimalDigit || category2 == QChar::Punctuation_Other) {
            return false ;
        }
    }

    return true;
}

int InputEdit::findWordBeginPosition(int pos)
{
    QString str = text();

    if (0 >= pos) {
        return 0;
    }

    while (pos > 0) {
        pos--;
        if (isSymbolCategoryChanged(pos, pos + 1)) {
            return pos + 1;
        }
    }

    return 0;
}

int InputEdit::findWordEndPosition(int pos)
{
    QString str = text();

    if (pos >= str.length()) {
        return str.length() - 1;
    }

    while (pos < str.length() - 1) {
        pos++;
        if (isSymbolCategoryChanged(pos, pos - 1)) {
            return pos - 1;
        }
    }

    return str.length() - 1;
}

void InputEdit::autoZoomFontSize()
{
    QFont font;

    // the maximum font is 28, minimum font is 9.
    for (int i = 28; i > 8; --i) {
        font.setPointSize(i);

        QFontMetrics fm(font);
        int fontWidth = fm.width(text());
        int editWidth = width() - 24;

        if (fontWidth < editWidth)
            break;
    }

    setFont(font);
}

void InputEdit::handleTextChanged(const QString &text)
{
    if (m_currentInAns) {
        m_ansLength = 0;
    } else if (m_currentOnAnsLeft && m_oldText.length() != 0) {
        int textLength = text.length();
        int oldTextLength = m_oldText.length();
        int minValue = std::min(textLength, oldTextLength);

        int i = 1;
        for (; i < minValue && text[textLength - i] == m_oldText[oldTextLength - i]; ++i) ;

        int diff = (textLength - i) - (oldTextLength - i);
        m_ansStartPos += diff;
    }

    int ansEnd = m_ansStartPos + m_ansLength;
    m_oldText = text;
    m_ansVaild = m_ansLength != 0 &&
        (m_ansStartPos == 0 || !text[m_ansStartPos - 1].isDigit()) &&
        (ansEnd == text.length() || !text[ansEnd].isDigit());
    m_oldText = text;

    // reformat text.
    int oldPosition = cursorPosition();
    int oldLength = text.length();

    QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
    reformatStr = reformatStr.replace('+', QString::fromUtf8("＋"))
                             .replace('-', QString::fromUtf8("－"))
                             .replace('*', QString::fromUtf8("×"))
                             .replace('/', QString::fromUtf8("÷"))
                             .replace('x', QString::fromUtf8("×"))
                             .replace('X', QString::fromUtf8("×"));
    setText(reformatStr);

    int newLength = reformatStr.length();
    setCursorPosition(oldPosition + (newLength - oldLength));

    autoZoomFontSize();
}

void InputEdit::handleCursorPositionChanged(int oldPos, int newPos)
{
    Q_UNUSED(oldPos);

    int ansEnd = m_ansStartPos + m_ansLength;
    int selectStart = this->selectionStart();
    int selectEnd = selectStart + this->selectedText().length();

    if (newPos > m_ansStartPos && newPos < ansEnd) {
        m_currentInAns = true;
    } else if (this->hasSelectedText() &&
               ((selectStart >= m_ansStartPos && selectStart < ansEnd) ) ||
               (selectEnd > m_ansStartPos && selectEnd <= ansEnd) ||
               (selectStart < m_ansStartPos && selectEnd > ansEnd)) {
        m_currentInAns = true;
    } else if (newPos <= m_ansStartPos) {
        m_currentInAns = false;
        m_currentOnAnsLeft = true;
    } else {
        m_currentInAns = false;
        m_currentOnAnsLeft = false;
    }
}
