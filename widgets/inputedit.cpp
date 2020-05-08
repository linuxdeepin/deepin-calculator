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
#include "../math/floatconfig.h"
#include "../utils.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStringList>

#include <DMenu>

InputEdit::InputEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_ans(0)
    , m_ansStartPos(0)
    , m_ansLength(0)
    , m_ansVaild(false)
    , m_currentInAns(false)
    , m_currentOnAnsLeft(false)
    , m_oldText("")
    , m_isprecentans(false)
    , m_lastPos(0)
{
    setAttribute(Qt::WA_InputMethodEnabled, false);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::StrongFocus);
    autoZoomFontSize();
    initAction();

    this->setFrame(false);
    this->setClearButtonEnabled(false);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QLineEdit::textChanged, this, &InputEdit::handleTextChanged);
    connect(this, &QLineEdit::cursorPositionChanged, this, &InputEdit::handleCursorPositionChanged);
    connect(this, &QLineEdit::customContextMenuRequested, this, &InputEdit::showTextEditMenu);
    connect(this, &InputEdit::returnPressed, this, &InputEdit::pressSlot);
    connect(this, &QLineEdit::selectionChanged, this, &InputEdit::selectionChangedSlot);
    connect(this, &QLineEdit::selectionChanged, [ = ] {
        int pos = this->cursorPosition();
        this->cursorPositionChanged(pos, pos);
    });

    DPalette pl = this->palette();
    // pl.setColor(DPalette::Text,QColor(48,48,48));
    pl.setColor(DPalette::Button, Qt::transparent);
    pl.setColor(DPalette::Highlight, Qt::transparent);
    pl.setColor(DPalette::HighlightedText, Qt::blue);
    this->setPalette(pl);
}

InputEdit::~InputEdit() {}

QString InputEdit::expressionPercent(QString &str)
{
    QString t = str;
    bool longnumber = false;

    QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
    if (ans.length() > 17) {
        for (int i = 17; i < ans.length(); i++) {
            if (ans.at(i) != "0") {
                longnumber = true;
                break;
            }
        }
    }
    if (longnumber && m_isprecentans && m_lastPos == m_ansStartPos + m_ansLength + 1)
        t = ans + str.back();
//    if (m_ansVaild) {
//        QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
//        t.remove(m_ansStartPos, m_ansLength);
//        t.insert(m_ansStartPos, ans);
//    }

    return t;
}

QString InputEdit::expressionText()
{
    QString t = text();
    //edit for bug-19653 20200416  当数字长度超过精度范围时，保留小数点最后的数。
    bool longnumber = false;

    QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
    if (ans.length() > 17) {
        for (int i = 17; i < ans.length(); i++) {
            if (ans.at(i) != "0") {
                longnumber = true;
                break;
            }
        }
    }
    if (m_ansVaild || longnumber) {
        t.remove(m_ansStartPos, m_ansLength);
        if (m_ansLength != 0) {
            t.insert(m_ansStartPos, ans);
        }
    }
//    if (m_ansVaild) {
//        QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
//        t.remove(m_ansStartPos, m_ansLength);
//        t.insert(m_ansStartPos, ans);
//    }

    return t;
}

void InputEdit::setAnswer(const QString &str, const Quantity &ans)
{
    m_isprecentans = false;
    m_ans = ans;
    m_ansStartPos = 0;
    m_ansLength = str.length();
    m_oldText = "";
    setText(str);
}

void InputEdit::setPercentAnswer(const QString &str1, const QString &str2, const Quantity &ans,
                                 const int &Pos)
{
    m_isprecentans = true;
    m_ans = ans;
    m_ansStartPos = Pos + ((Pos == 0) ? 0 : 1); //edit 20200416
    m_ansLength = str2.length();
    m_oldText = "";
    setText(str1);
    int ansEnd = m_ansStartPos + m_ansLength;
    while (ansEnd > str1.length()) {
        --ansEnd;
    }
    m_ansVaild = m_ansLength > 10 && (m_ansStartPos == 0 || !str1[m_ansStartPos - 1].isDigit()) &&
                 (ansEnd == str1.length() || !str1[ansEnd].isDigit());
}

void InputEdit::clear()
{
    m_ansLength = 0;
    setText("");
}

void InputEdit::setUndoAction(bool state)
{
    m_undo->setEnabled(state);
}

void InputEdit::setRedoAction(bool state)
{
    m_redo->setEnabled(state);
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    Q_EMIT keyPress(e);
    return;
}

void InputEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    selectAll();
    m_selected.selected = text();
    /*if (e->button() == Qt::LeftButton) {
        int position = this->cursorPositionAt(e->pos());
        int posBegin = findWordBeginPosition(position);
        int posEnd = findWordEndPosition(position);
        this->setSelection(posBegin, posEnd - posBegin + 1);
    }*/
}

void InputEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        setFocus();
        m_selected.selected = "";
        emit setResult();
        //        qDebug() << m_selected.selected;
    }
    QLineEdit::mousePressEvent(e);
}

void InputEdit::initAction()
{
    m_undo = new QAction(tr("&Undo"), this);
    m_redo = new QAction(tr("&Redo"), this);
    m_cut = new QAction(tr("Cu&t"), this);
    m_copy = new QAction(tr("&Copy"), this);
    m_paste = new QAction(tr("&Paste"), this);
    m_delete = new QAction(tr("Delete"), this);
    m_select = new QAction(tr("Select All"), this);

    connect(m_undo, &QAction::triggered, this, &InputEdit::undo);
    connect(m_redo, &QAction::triggered, this, &InputEdit::redo);
    connect(m_cut, &QAction::triggered, this, &InputEdit::cut);
    connect(m_copy, &QAction::triggered, this, &InputEdit::copy);
    connect(m_paste, &QAction::triggered, this, &InputEdit::paste);
    connect(m_delete, &QAction::triggered, this, &InputEdit::deleteText);
    connect(m_select, &QAction::triggered, this, &InputEdit::selectAllText);

    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    m_cut->setEnabled(false);
    m_copy->setEnabled(false);
    m_delete->setEnabled(false);
    m_select->setEnabled(false);
}

void InputEdit::updateAction()
{
    if (this->text().isEmpty()) {
        m_select->setEnabled(false);
        m_delete->setEnabled(false);
        m_copy->setEnabled(false);
        m_cut->setEnabled(false);
    } else {
        m_select->setEnabled(true);
        m_delete->setEnabled(true);
        m_copy->setEnabled(true);
        m_cut->setEnabled(true);
    }
}

bool InputEdit::isSymbolCategoryChanged(int pos1, int pos2)
{
    QString str = text();
    QChar::Category category1 = str.at(pos1).category();
    QChar::Category category2 = str.at(pos2).category();

    if (category1 == QChar::Number_DecimalDigit || category1 == QChar::Punctuation_Other) {
        if (category2 == QChar::Number_DecimalDigit || category2 == QChar::Punctuation_Other) {
            return false;
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

    // the maximum font is 30, minimum font is 15.
    for (int i = 30; i > 16; --i) {
        font.setPixelSize(i);

        QFontMetrics fm(font);
        int fontWidth = fm.width(text());
        int editWidth = width() - 45;

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
        for (; i < minValue && text[textLength - i] == m_oldText[oldTextLength - i]; ++i)
            ;

        int diff = (textLength - i) - (oldTextLength - i);
        m_ansStartPos += diff;
    }

    if (text.indexOf("=") != -1) {
        QString exp = text;
        exp.remove(text.indexOf("="), 1);
        setText(exp);
        Q_EMIT equal();
        return;
    }

    int ansEnd = m_ansStartPos + m_ansLength;
    m_oldText = text;
    while (ansEnd > text.length()) {
        --ansEnd;
    }
    m_ansVaild = m_ansLength > 10 && (m_ansStartPos == 0 || !text[m_ansStartPos - 1].isDigit()) &&
                 (ansEnd == text.length() || !text[ansEnd].isDigit());
    m_oldText = text;

    int oldPosition = this->cursorPosition();
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

    multipleArithmetic(reformatStr);
    reformatStr.remove(QRegExp("[^0-9＋－×÷,.%()e]"));
    // reformatStr = pointFaultTolerance(reformatStr);
    //    reformatStr = symbolFaultTolerance(reformatStr);
    setText(reformatStr);
    autoZoomFontSize();
    updateAction();

    // reformat text.
    int oldLength = text.length();
    int newLength = reformatStr.length();
    int pos;
    if (newLength > oldLength)
        pos = oldPosition + (newLength - oldLength);
    else
        pos = oldPosition;
    if (pos > newLength)
        pos = newLength;
    this->setCursorPosition(pos);
}

QString InputEdit::pointFaultTolerance(const QString &text)
{
    QString exp = text;
    QString oldText = text;
    QStringList list = exp.split(QRegExp("[＋－×÷()]"));
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        int firstPoint = item.indexOf(".");
        if (firstPoint == -1)
            continue;
        if (firstPoint == 0) {
            item.insert(firstPoint, "0");
            ++firstPoint;
            // oldText.replace(list[i], item);
        } else {
            if (item.at(firstPoint - 1) == ")" || item.at(firstPoint - 1) == "%") {
                item.remove(firstPoint, 1);
                oldText.replace(list[i], item);
            }
        }
        if (item.count(".") > 1) {
            int cur = cursorPosition();
            item.remove(".");
            item.insert(firstPoint, ".");
            oldText.replace(list[i], item);
            setCursorPosition(cur);
        }
    }
    return oldText;
}

QString InputEdit::symbolFaultTolerance(const QString &text)
{
    if (text.isEmpty())
        return text;
    QString exp = text;
    QString newText;
    /*if (isSymbol(exp.at(0))) {
        if (exp.at(0) != QString::fromUtf8("－"))
            return "";
        else {
            for (int i = 0; i < exp.length(); ++i) {}
        }
    }*/
    QStringList symbolList;
    for (int i = 0; i < exp.length(); ++i) {
        /*if (isSymbol(exp.at(i)) && i < exp.length() - 2) {
            while (isSymbol(exp.at(i + 1)) && i < exp.length() - 2) {
                ++i;
            }
            newText.append(exp.at(i));
        } else {
            newText.append(exp.at(i));
        }*/
        if (!isSymbol(exp.at(i))) {
            if (!symbolList.isEmpty()) {
                if (!newText.isEmpty())
                    newText.append(symbolList.last());
                if (newText.isEmpty() && symbolList.last() == "－")
                    newText.append(symbolList.last());
            }
            newText.append(exp.at(i));
            symbolList.clear();
        } else {
            symbolList.append(exp.at(i));
            continue;
        }
    }
    if (!symbolList.isEmpty())
        newText.append(symbolList.last());
    return newText;
}

bool InputEdit::isSymbol(const QString &text)
{
    if (text == QString::fromUtf8("＋"))
        return true;
    else if (text == QString::fromUtf8("－"))
        return true;
    else if (text == QString::fromUtf8("×"))
        return true;
    else if (text == QString::fromUtf8("÷"))
        return true;
    else
        return false;
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
               ((selectStart >= m_ansStartPos && selectStart < ansEnd)) ||
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
    m_lastPos = newPos;
}

void InputEdit::BracketCompletion(QKeyEvent *e)
{
    QString oldText = text();
    int curs = this->cursorPosition();
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
                oldText.insert(curs, ")");
            } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0) {
                oldText.insert(curs, "(");
            } else {
                oldText.insert(curs, "()");
            }
            //如果左侧左括号小于等于左侧右括号
        } else {
            //如果右侧左括号小于右括号
            if (rightLeftParen < rightrightParen) {
                oldText.insert(curs, "(");
            } else {
                oldText.insert(curs, "()");
            }
        }
        //相等则输入一对括号
    } else {
        oldText.insert(curs, "()");
    }
    this->setCursorPosition(curs);
    setText(oldText);
}

bool InputEdit::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        Q_EMIT keyPress(keyEvent);
        return true;
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        mouseDoubleClickEvent(mouseEvent);
        return true;
    }
    return QLineEdit::eventFilter(watched, event);
}

void InputEdit::multipleArithmetic(QString &text)
{
    int index = text.indexOf("\n");
    if (index != -1) {
        int count = text.count("\n");
        for (int i = 0; i < count; ++i) {
            index = text.indexOf("\n", i);
            if (index == 0)
                continue;
            if (text.at(index - 1) == ")" || text.at(index - 1) == "%")
                text.replace(index, 1, "×");
        }
    }
}

void InputEdit::showTextEditMenu(QPoint p)
{
    DMenu *menu = new DMenu(this);
    menu->addAction(m_undo);
    menu->addAction(m_redo);
    menu->addAction(m_cut);
    menu->addAction(m_copy);
    menu->addAction(m_paste);
    menu->addAction(m_delete);
    menu->addSeparator();
    menu->addAction(m_select);

    if (QApplication::clipboard()->text().isEmpty())
        m_paste->setEnabled(false);
    else
        m_paste->setEnabled(true);

    if (this->selectedText().isEmpty())
        m_cut->setEnabled(false);
    else
        m_cut->setEnabled(true);

    menu->move(cursor().pos());
    menu->exec();
    menu->deleteLater();
}

void InputEdit::pressSlot()
{
    return;
}

void InputEdit::selectionChangedSlot()
{
    if (!hasFocus())
        return;
    m_selected.oldText = text();
    m_selected.selected = selectedText();
    m_selected.curpos = selectionStart();
    //    qDebug() << m_selected.selected;
}

Quantity InputEdit::getanswer()
{
    Quantity ans = m_ans;
    return ans;
}
