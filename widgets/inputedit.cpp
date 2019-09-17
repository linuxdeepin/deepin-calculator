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
#include <QStringList>

InputEdit::InputEdit(QWidget *parent)
    : DLineEdit(parent),
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
    setFrame(false);
    setClearButtonEnabled(false);
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
#if 0
    switch (e->key()) {
    case Qt::Key_Equal: case Qt::Key_Period:
        return;
    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        BracketCompletion(e);
        break;
    default:
        QLineEdit::keyPressEvent(e);
        break;
    }
#else
    switch (e->key()) {
    case Qt::Key_Equal: case Qt::Key_Period:
        return;
    case Qt::Key_ParenRight:
        QLineEdit::keyPressEvent(e);
        setCursorPosition(cursorPosition() - 1);
        break;
    /*case Qt::Key_Plus: case Qt::Key_Minus: case Qt::Key_Underscore:
    case Qt::Key_Asterisk: case Qt::Key_X: case Qt::Key_Slash:
        return;
    default:
        QLineEdit::keyPressEvent(e);*/
    }
#endif
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
    for (int i = 26; i > 1; --i) {
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
                             .replace("_", QString::fromUtf8("－"))
                             .replace('*', QString::fromUtf8("×"))
                             .replace('/', QString::fromUtf8("÷"))
                             .replace('x', QString::fromUtf8("×"))
                             .replace('X', QString::fromUtf8("×"))
                             .replace(QString::fromUtf8("（"), "(")
                             .replace(QString::fromUtf8("）"), ")")
                             .replace(QString::fromUtf8("。"), ".")
                             .replace(QString::fromUtf8("——"), QString::fromUtf8("－"));

    reformatStr = pointFaultTolerance(reformatStr);
    setText(reformatStr);

    int newLength = reformatStr.length();
    setCursorPosition(oldPosition + (newLength - oldLength));

    autoZoomFontSize();
}

QString InputEdit::pointFaultTolerance(const QString &text)
{
    QString exp = text;
    QString oldText = text;
    exp = exp.replace(QString::fromUtf8("－"),QString::fromUtf8("＋"))
             .replace(QString::fromUtf8("×"),QString::fromUtf8("＋"))
             .replace(QString::fromUtf8("÷"),QString::fromUtf8("＋"));
    QStringList list = exp.split(QString::fromUtf8("＋"));
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        int firstPoint = item.indexOf(".");
        int lastPoint = item.lastIndexOf(".");
        int count = item.count(".");
        if (lastPoint - firstPoint + 1 == count) {
            item.remove(firstPoint,lastPoint);
            item.insert(firstPoint, ".");
            oldText.replace(list[i], item);
        }
    }
    return oldText;
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

void InputEdit::BracketCompletion(QKeyEvent *e)
{
    QString oldText = text();
    int curs = cursorPosition();
    int right = oldText.length() - curs;
    int leftLeftParen = oldText.left(curs).count("(");
    int leftRightParen = oldText.left(curs).count(")");
    int rightLeftParen = oldText.right(right).count("(");
    int rightrightParen = oldText.right(right).count(")");
    //左右括号总数是否相等
    if (oldText.count("(") != oldText.count(")")) {
        //光标左侧左括号大于右括号
        if (leftLeftParen > leftRightParen) {
            if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) > 0) {
                oldText.insert(curs,")");
            } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0) {
                oldText.insert(curs,"(");
            } else {
                oldText.insert(curs,"()");
            }
        //如果左侧左括号小于等于左侧右括号
        } else {
            //如果右侧左括号小于右括号
            if (rightLeftParen < rightrightParen) {
                oldText.insert(curs,"(");
            } else {
                oldText.insert(curs,"()");
            }
        }
    //相等则输入一对括号
    } else {
        oldText.insert(curs,"()");
    }
    setCursorPosition(curs);
    setText(oldText);
}
