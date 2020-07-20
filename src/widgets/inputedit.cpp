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

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QStringList>
#include <DMenu>

#include "src/math/floatconfig.h"
#include "src/utils.h"

InputEdit::InputEdit(QWidget *parent)
    : QLineEdit(parent)
    , m_ans(0)
    , m_ansStartPos(0)
    , m_ansLength(0)
    , m_ansVaild(false)
    , m_currentInAns(false)
    , m_currentOnAnsLeft(false)
    , m_oldText("")
    , m_lastPos(0)
    , m_memoryans(0)
    , m_percentexp(QString())
{
    m_evaluator = Evaluator::instance();
    setAttribute(Qt::WA_InputMethodEnabled, false); //禁止中文输入法
    setAttribute(Qt::WA_TranslucentBackground); //设置窗口透明
    setFocusPolicy(Qt::StrongFocus); //接受Tab键和鼠标单击做焦点
    autoZoomFontSize();
    initAction();

    this->setFrame(false);
    this->setClearButtonEnabled(false); //不显示清除按钮
    this->setContextMenuPolicy(Qt::CustomContextMenu); //信号(customContextMenuRequested)槽完成右击菜单事件

    connect(this, &QLineEdit::textChanged, this, &InputEdit::handleTextChanged);
    connect(this, &QLineEdit::cursorPositionChanged, this, &InputEdit::handleCursorPositionChanged);
    connect(this, &QLineEdit::customContextMenuRequested, this, &InputEdit::showTextEditMenu); //右键菜单信号槽
    connect(this, &InputEdit::returnPressed, this, &InputEdit::pressSlot);
    connect(this, &QLineEdit::selectionChanged, this, &InputEdit::selectionChangedSlot);
    connect(this, &QLineEdit::selectionChanged, [ = ] {
        int pos = this->cursorPosition();
        this->cursorPositionChanged(pos, pos);
    });

    connect(this, &QLineEdit::textChanged, this, &InputEdit::isExpressionEmpty);

    DPalette pl = this->palette();
    // pl.setColor(DPalette::Text,QColor(48,48,48));
    pl.setColor(DPalette::Button, Qt::transparent);
    pl.setColor(DPalette::Highlight, Qt::transparent);
    pl.setColor(DPalette::HighlightedText, Qt::blue);
    this->setPalette(pl);

    m_funclist = {"arcsin", "arccos", "arctan", "arccot", "sin", "cos", "tan", "cot"
                  , "abs", "lg", "ln", "log", "mod", "sqrt", "cbrt", "yroot", "pi", "π"
                 };
}

InputEdit::~InputEdit() {}

QString InputEdit::expressionPercent(QString &str)
{
    QString t = str;
    bool longnumber = false;

    QString ans = DMath::format(m_ans, Quantity::Format::Fixed() + Quantity::Format::Precision(DECPRECISION));
    m_evaluator->setVariable(QLatin1String("precentans"), m_ans, Variable::BuiltIn);
    if (ans.length() > 17) {
        for (int i = 17; i < ans.length(); i++) {
            if (ans.at(i) != "0") {
                longnumber = true;
                break;
            }
        }
    }
    if (longnumber && m_lastPos == m_ansStartPos + m_ansLength + 1) {
        t = QLatin1String("precentans") + str.back();
        m_ispercentanswer = true;
    }
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
//    t.remove(",");
    //edit for bug-19653 20200416  当数字长度超过精度范围时，保留小数点最后的数。
    bool longnumber = false;

    QString ans = DMath::format(m_ans, Quantity::Format::Fixed() + Quantity::Format::Precision(DECPRECISION));
    m_evaluator->setVariable(QLatin1String("lastans"), m_ans, Variable::BuiltIn);
    if (ans.length() > 17) {
        for (int i = 17; i < ans.length(); i++) {
            if (ans.at(i) != "0") {
                longnumber = true;
                break;
            }
        }
    }

    //20200619输入框是否包含ans，长数字等于后撤销退格一位按等于计算错误;不撤销退格一位按等于号计算错误
    if (m_ansVaild && longnumber && text().contains(m_strans)) {
        t.remove(m_ansStartPos, m_ansLength);
        if (m_ansLength != 0) {
            t.insert(m_ansStartPos, QLatin1String("lastans"));
        }
    }
//    if (m_ansVaild) {
//        QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
//        t.remove(m_ansStartPos, m_ansLength);
//        t.insert(m_ansStartPos, ans);
//    }
//    qDebug() << t;
    return t;
}

void InputEdit::setAnswer(const QString &str, const Quantity &ans)
{
    m_ans = ans;
    m_ansStartPos = 0;
    m_ansLength = str.length();
    m_oldText = "";
    setText(str);
    m_strans = str;
    m_strans = m_strans.replace('+', QString::fromUtf8("＋"))
               .replace('-', QString::fromUtf8("－"))
               .replace('*', QString::fromUtf8("×"))
               .replace(QString::fromUtf8("（"), "(")
               .replace(QString::fromUtf8("）"), ")");
}

void InputEdit::setPercentAnswer(const QString &str1, const QString &str2, const Quantity &ans,
                                 const int &Pos)
{
    if (m_ispercentanswer) {
        m_ans = ans;
        m_ansStartPos = Pos + ((Pos == 0) ? 0 : 1); //edit 20200416
        m_ansLength = str2.length();
        m_oldText = "";
        setText(str1);
        int ansEnd = m_ansStartPos + m_ansLength;
        while (ansEnd > str1.length()) {
            --ansEnd;
        }
        m_ansVaild = /*m_ansLength > 10 &&*/ m_ansLength > 0 && (m_ansStartPos == 0 || !str1[m_ansStartPos - 1].isDigit()) &&
                                             (ansEnd == str1.length() || !str1[ansEnd].isDigit());
    }
    m_ispercentanswer = false;
}

void InputEdit::clear()
{
    m_ansLength = 0;
    setText("");
}

/**
 * @brief 由undo列表设置右键菜单中撤销状态
 */
void InputEdit::setUndoAction(bool state)
{
    m_undo->setEnabled(state);
}

/**
 * @brief 由redo列表设置右键菜单中撤销状态
 */
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
    Q_UNUSED(e);
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
        m_copy->setEnabled(false);
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

/**
 * @brief InputEdit::输入框字号变化
 */
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
    //20200612修复下方m_ansValid赋值判断text[m_ansStartPos - 1]越界问题
    if (m_currentInAns) {
        m_ansLength = 0; //光标在ans中间且text改变，清空ans
    } else if (m_currentOnAnsLeft && m_oldText.length() != 0) {
        int textLength = text.length();
        int oldTextLength = m_oldText.length();
//        int minValue = std::min(textLength, oldTextLength);

//        int i = 1;
//        for (; i < minValue && text[textLength - i] == m_oldText[oldTextLength - i]; ++i)
//            ; //i=diff?不知为何又用到下侧int计算　i无用

//        int diff = (textLength - i) - (oldTextLength - i);
        int diff = textLength - oldTextLength;
        m_ansStartPos += diff;
    }
    if (text.indexOf("=") != -1) {
        QString exp = text;
        exp.remove(text.indexOf("="), 1);
        setText(exp);
        Q_EMIT equal(); //当前外界键盘及计算器键盘中=均不走此；猜测为了复制粘贴等于式；当前粘贴会去除=，也不会走此；暂不删除
        return;
    }


    int ansEnd = m_ansStartPos + m_ansLength;

    m_oldText = text;
    while (ansEnd > text.length()) {
        --ansEnd;
    }

    //ans是否有效；判断条件：ans未被改变,ans左侧右侧均不是小数点
    m_ansVaild = /*m_ansLength > 10 &&*/ m_ansLength > 0 && (m_ansStartPos == 0 || !text[m_ansStartPos - 1].isDigit()) &&
                                         (ansEnd == text.length() || !text[ansEnd].isDigit());
    int oldPosition = this->cursorPosition();
    QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
    reformatStr = reformatStr.replace('+', QString::fromUtf8("＋"))
                  .replace('-', QString::fromUtf8("－"))
                  .replace("_", QString::fromUtf8("－"))
                  .replace('*', QString::fromUtf8("×"))
                  .replace(QString::fromUtf8("＊"), QString::fromUtf8("×"))
//                  .replace('/', QString::fromUtf8("÷"))
                  .replace('x', QString::fromUtf8("×"))
                  .replace('X', QString::fromUtf8("×"))
                  .replace(QString::fromUtf8("（"), "(")
                  .replace(QString::fromUtf8("）"), ")")
                  .replace(QString::fromUtf8("。"), ".")
                  .replace(QString::fromUtf8("——"), QString::fromUtf8("－"))
                  .replace(QString::fromUtf8("％"), "%");

    multipleArithmetic(reformatStr);
//    reformatStr.remove(QRegExp("[^0-9＋－×÷,.%()e]"));
    // reformatStr = pointFaultTolerance(reformatStr);
    //    reformatStr = symbolFaultTolerance(reformatStr);
    setText(reformatStr);
    autoZoomFontSize();
    updateAction(); //textchanged时更新右键菜单状态

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

    m_selected.oldText = this->text(); //选中输入情况下清空被选部分
    m_selected.selected = selectedText();
    m_selected.curpos = selectionStart() < selectionEnd() ? selectionStart() : selectionEnd();
}

QString InputEdit::pointFaultTolerance(const QString &text)
{
    QString exp = text;
    QString oldText = text;
    QStringList list = exp.split(QRegExp("[＋－×÷/()]"));
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
                    newText.append(symbolList.last()); //保证数字中间的符号只有一个，去除多余符号
                if (newText.isEmpty() && symbolList.last() == "－")
                    newText.append(symbolList.last()); //保存负数的-号
            }
            newText.append(exp.at(i));
            symbolList.clear();
        } else {
            symbolList.append(exp.at(i));
            continue;
        }
    }
//    qDebug() << symbolList << " a " << newText;
    if (!symbolList.isEmpty() /*&& !newText.isEmpty()*/) { //防止输入栏中只有符号可输入*/+;暂未屏蔽%
        if ((newText.isEmpty() && symbolList.contains("－"))) {
            newText.append("－");
        } else if (!newText.isEmpty()) {
            newText.append(symbolList.last());
        }
    }
    //edit 20200526 for bug-28491
    int expPos = newText.indexOf("e");
    if (expPos > 0) {
        //e后非＋／－
        if (newText.length() > expPos + 1 && newText.at(expPos + 1) != QString::fromUtf8("－") && newText.at(expPos + 1) != QString::fromUtf8("＋")
                && newText.at(expPos + 1) != "-" && newText.at(expPos + 1) != "+") {
            while (newText.at(expPos + 1) == "(" || newText.at(expPos + 1) == ")") {
                newText.remove(expPos + 1, 1); //避免e后可输入()情况
            }
            return newText;
        }
        //e+/e-后有数字
        if (newText.length() > expPos + 2) {
            while (newText.length() > expPos + 2 && newText.at(expPos + 2).isNumber() == false) {
                newText.remove(expPos + 2, 1); //e+/e-和数字间不可以插入非数字
            }
            int nextsymbolpos = newText.indexOf(QRegExp("[＋－×÷/()]"), expPos + 2); //e+/e-右侧第一个符号
            for (int i = expPos; i < (nextsymbolpos == -1 ? newText.length() : nextsymbolpos); i++) {
                if (newText.at(i) == "." || newText.at(i) == QString::fromUtf8("。"))
                    newText.remove(i, 1); //去除从e到下一个运算符中的小数点
            }
        }
    }
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
    QString sRegNum = "[A-Za-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    int leftfunpos = -1;
    int rightfunpos = -1;
    int i, j;
    if (cursorPosition() > 0 && cursorPosition() != text().length()
            && rx.exactMatch(text().at(cursorPosition() - 1))
            && rx.exactMatch(text().at(cursorPosition()))) {
        for (i = 0; i < m_funclist.size(); i++) {
            leftfunpos = text().lastIndexOf(m_funclist[i], cursorPosition() - 1);
            if (leftfunpos != -1 && leftfunpos + m_funclist[i].length() == cursorPosition())
                break;
            else
                leftfunpos = -1;
        }
        for (j = 0; j < m_funclist.size(); j++) {
            rightfunpos = text().indexOf(m_funclist[j], cursorPosition());
            if (rightfunpos != -1 && rightfunpos == cursorPosition())
                break;
            else
                rightfunpos = -1;
        }
        if (leftfunpos == -1 || rightfunpos == -1)
            setCursorPosition(text().indexOf(QRegExp("[^A-Za-z]"), cursorPosition()));
    }

    int ansEnd = m_ansStartPos + m_ansLength;
    int selectStart = m_selected.curpos; //选中输入后选中部分被清空　不可用selectStart()
    int selectEnd = selectStart + m_selected.selected.length();

    if (newPos > m_ansStartPos && newPos < ansEnd) {
        m_currentInAns = true; //当前光标在ans开始后结束前
    } else if (((selectStart >= m_ansStartPos && selectStart < ansEnd)) ||
               (selectEnd > m_ansStartPos && selectEnd <= ansEnd) ||
               (selectStart < m_ansStartPos && selectEnd > ansEnd)) { //选中->输入->selection为空->cursprchanged导致此处不会进入;删除hasSelectedText();
        m_currentInAns = true; //选中区与ans有交集
    } else if (newPos <= m_ansStartPos) {
        m_currentInAns = false;
        m_currentOnAnsLeft = true; //光标在ans左侧，保存以去计算ans开始位置
    } else {
        m_currentInAns = false;
        m_currentOnAnsLeft = false;
    }
    m_lastPos = newPos;
}

void InputEdit::BracketCompletion(QKeyEvent *e)
{
    Q_UNUSED(e);
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
    Q_UNUSED(p);
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
    if (this->selectedText().isEmpty()) { //无选中项时关闭右键复制
        m_copy->setEnabled(false);
    } else {
        m_copy->setEnabled(true);
    }
    if (!hasFocus())
        return; //只有选中被改变情况下给m_selected赋值,选中输入不会改变;选中输入后优先级高于cursorchanged，去掉return无意义
    m_selected.oldText = text();
    m_selected.selected = selectedText();
    m_selected.curpos = selectionStart() < selectionEnd() ? selectionStart() : selectionEnd();
}

QPair<bool, Quantity> InputEdit::getMemoryAnswer()
{
    QPair<bool, Quantity> pair;
    QString expression;
    expression = symbolComplement(expressionText()).replace(QString::fromUtf8("＋"), "+")
                 .replace(QString::fromUtf8("－"), "-")
                 .replace(QString::fromUtf8("×"), "*")
                 .replace(QString::fromUtf8("÷"), "/")
                 .replace(QString::fromUtf8(","), "");
    m_evaluator->setExpression(expression);
    m_memoryans = m_evaluator->evalUpdateAns();
    if (m_evaluator->error().isEmpty()) {
        if (m_memoryans.isNan() && !m_evaluator->isUserFunctionAssign()) {
            pair.first = false;
            pair.second = Quantity(0);
            return pair;
        }
        pair.first = true;
        pair.second = m_memoryans;
        return pair;
    } else {
        pair.first = false;
        pair.second = Quantity(0);
        return pair;
    }
}

void InputEdit::isExpressionEmpty()
{
    if (text().isEmpty())
        emit emptyExpression(true);
    else
        emit emptyExpression(false);
}

void InputEdit::hisexpression()
{
    m_ansLength = 0;
}

QString InputEdit::symbolComplement(const QString exp)
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

