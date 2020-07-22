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

#include "sciexpressionbar.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QTimer>
#include <DGuiApplicationHelper>

#include "src/utils.h"
#include "src/core/settings.h"

#define SCIPREC 31 //科学计算器精度

SciExpressionBar::SciExpressionBar(QWidget *parent)
    : DWidget(parent)
{
    m_listView = new SimpleListView;
    m_listDelegate = new SimpleListDelegate(0, this);
    m_listModel = new SimpleListModel(0, this);
    m_inputEdit = new InputEdit;
    m_evaluator = Evaluator::instance();
    m_isContinue = true;
    m_isAllClear = false;
    m_isResult = false;
    m_inputNumber = false;
    m_isUndo = false;
    m_meanexp = true;
    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    // init inputEdit attributes.
    m_listView->setFixedHeight(35);
    m_inputEdit->setFixedHeight(55);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);

    DPalette pl1 = this->palette();
    // pl.setColor(DPalette::Text,QColor(48,48,48));
    pl1.setColor(DPalette::Button, Qt::transparent);
    pl1.setColor(DPalette::Highlight, Qt::transparent);
    pl1.setColor(DPalette::HighlightedText, Qt::blue);

    DPalette pl = this->palette();
    pl.setColor(DPalette::Light, QColor(0, 0, 0, 0));
    pl.setColor(DPalette::Dark, QColor(0, 0, 0, 0));
    pl.setColor(DPalette::Base, QColor(0, 0, 0, 0));
    this->setPalette(pl);

    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->addWidget(m_listView);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);

//    setFixedHeight(100);
    initConnect();

    Settings::instance()->angleUnit = 'd';
    m_funclist = {"arcsin", "arccos", "arctan", "arccot", "sin", "cos", "tan", "cot"
                  , "abs", "lg", "ln", "log", "mod", "sqrt", "cbrt", "yroot", "pi", "π", "e"
                 };
}

SciExpressionBar::~SciExpressionBar() {}

void SciExpressionBar::setContinue(bool isContinue)
{
    m_isContinue = isContinue;
}

QPair<bool, QString> SciExpressionBar::getexpression()
{
    m_pair.second = m_expression;
    return m_pair;
}

Quantity SciExpressionBar::getanswer()
{
    if (m_pair.first == true)
        return m_listanswer;
    return Quantity();
}

void SciExpressionBar::enterNumberEvent(const QString &text)
{
    if (m_inputNumber && m_isResult == true) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    if (m_isResult) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    if (!m_isContinue) {
        // the cursor position at the end, it will clear edit text.

        if (cursorPosAtEnd())
            m_inputEdit->clear();

        m_isContinue = true;
//        m_listView->setFocus();
    }

    m_inputNumber = false;
    m_isUndo = false;

    // 20200401 修改symbolFaultTolerance执行位置
    replaceSelection(m_inputEdit->text());
    m_inputEdit->insert(text);
    int nowcur = m_inputEdit->cursorPosition();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
    m_inputEdit->setCursorPosition(nowcur);
    emit clearStateChanged(false);
}

void SciExpressionBar::enterSymbolEvent(const QString &text)
{
    QString oldText = m_inputEdit->text();
    QString symbol = text;
    symbol.replace('/', QString::fromUtf8("÷"));
    m_isResult = false;
    m_isUndo = false;
    // 20200213统一被选中光标复位代码
    replaceSelection(m_inputEdit->text());
    if (m_inputEdit->text().isEmpty()) {
        if (symbol != "-") {
            m_inputEdit->setText(oldText);
        } else {
            m_inputEdit->insert(symbol);
        }
    } else {
        // 20200316无效代码删除
        //        if (m_inputEdit->text() == "－")
        //            m_inputEdit->setText(oldText);
        //        getSelection();
        int curPos = m_inputEdit->cursorPosition();
        QString exp = m_inputEdit->text();
        if (cursorPosAtEnd()) {
            if (m_inputEdit->text() == "－") {
                m_inputEdit->setText(oldText);
            } else {
                QString lastStr = exp.right(1);
                if (isOperator(lastStr))
                    exp.chop(1);
                m_inputEdit->setText(exp + symbol);
            }
        } else if (curPos == 0) {
            QString firstStr = exp.left(1);
            if (firstStr == QString::fromUtf8("－")) {
                m_inputEdit->setText(oldText);
            } else {
                if (symbol == QString::fromUtf8("－") || symbol == "-")
                    m_inputEdit->insert(symbol);
            }
        } else {
            QString infront = exp.at(curPos - 1);
            QString behand = exp.at(curPos);
            if (isOperator(infront) || isOperator(behand)) {
                m_inputEdit->setText(oldText);
            } else
                m_inputEdit->insert(symbol);

            // 2020316修复添加符号后光标问题
            //添加符号后左侧数字不会多分隔符，只需考虑添加符号后输入框光标前的数字与添加前是否一致
            if (exp.mid(0, curPos).remove(QRegExp("[＋－×÷/,.%()e]")) ==
                    m_inputEdit->text().mid(0, curPos).remove(QRegExp("[＋－×÷/,.%()e]"))) {
                QString sRegNum = "[＋－×÷/]";
                QRegExp rx;
                rx.setPattern(sRegNum);
                rx.exactMatch(m_inputEdit->text().at(curPos))
                ? m_inputEdit->setCursorPosition(curPos + 1)
                : m_inputEdit->setCursorPosition(curPos);
            } else
                m_inputEdit->setCursorPosition(curPos - 1);
        }
    }
    m_isContinue = true;
    expressionCheck();
}

void SciExpressionBar::enterPercentEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0%");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^!%]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0 || rx.exactMatch(exp.at(curpos - 1))) {
        m_inputEdit->insert("0%");
        diff = 1;
    } else
        m_inputEdit->insert("%");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 1 + diff);
    }
}

void SciExpressionBar::enterPointEvent()
{
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    if (curpos == 0) {
        m_inputEdit->insert("0.");
    } else {
        if (exp.at(curpos - 1) == ".")
            return;
//        if (exp.at(curpos - 1) != ")" && exp.at(curpos - 1) != "%") {
        QString sRegNum = "[0-9,]+";
        QRegExp rx;
        rx.setPattern(sRegNum);
        if (rx.exactMatch(exp.at(curpos - 1))) {
            int index = exp.indexOf(QRegExp("[^0-9,]"), curpos);
            QString cut = exp.mid(curpos, index - curpos);
            int aftercurpos = cut.count(",");
            int before = exp.count(",");
            m_inputEdit->insert(".");
            int after = m_inputEdit->text().count(",");
            if (before - aftercurpos == after) {
                m_inputEdit->setCursorPosition(curpos + 1);
            } else {
                m_inputEdit->setCursorPosition(curpos);
            }
        } else
            m_inputEdit->insert("0.");
//        }
    }
    exp = pointFaultTolerance(m_inputEdit->text());
    if (exp != m_inputEdit->text())
        m_inputEdit->setText(exp);
    m_isUndo = false;
    m_isResult = false;
}

void SciExpressionBar::enterBackspaceEvent()
{
    QString sRegNum = "[A-Za-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    SSelection selection = m_inputEdit->getSelection();
    int selleftfunlen = 0; //选中左侧一部分函数长度
    int removepos = 0; //被删除位置
    if (selection.selected != "") {
        QString text = m_inputEdit->text();
        QString seloldtext = text;
        //光标不在开头且光标左侧是字母或者光标右侧是字母
        if ((selection.curpos > 0 &&
                rx.exactMatch(m_inputEdit->text().at(selection.curpos - 1)))
                || rx.exactMatch(m_inputEdit->text().at(selection.curpos + selection.selected.size()))) {
            int funpos = -1;
            int rightfunpos = -1;
            int i;
            int j;
            for (i = 0; i < m_funclist.size(); i++) {
                //记录光标左侧离光标最近的函数位
                funpos = m_inputEdit->text().lastIndexOf(m_funclist[i], selection.curpos - 1);
                if (funpos != -1 && (funpos <= selection.curpos) && (selection.curpos < funpos + m_funclist[i].length())) {
                    selleftfunlen = selection.curpos - funpos;
                    break; //光标在函数开头和函数结尾之间
                } else
                    funpos = -1;
            }
            for (j = 0; j < m_funclist.size(); j++) {
                //记录离光标最近的右侧函数位
                rightfunpos = m_inputEdit->text().lastIndexOf(m_funclist[j], selection.curpos + selection.selected.size() - 1);
                if (rightfunpos != -1 && (rightfunpos + m_funclist[j].length() > selection.curpos + selection.selected.size()))
                    break;
                else
                    rightfunpos = -1;
            }
            if (funpos != -1 || rightfunpos != -1) {
                if (funpos != -1 && rightfunpos == -1) {
                    removepos = funpos;
                    text.remove(funpos, selection.selected.size() + selleftfunlen); //仅左侧有函数
                } else if (rightfunpos != -1 && funpos == -1) {
                    removepos = selection.curpos;
                    text.remove(selection.curpos, rightfunpos + m_funclist[j].length() - selection.curpos); //仅右侧有函数
                } else {
                    removepos = funpos;
                    text.remove(funpos, rightfunpos + m_funclist[j].length() - funpos); //函数中或左右均有
                }
            }
        } else {
            removepos = selection.curpos;
            text.remove(selection.curpos, selection.selected.size());
        }

        m_inputEdit->setText(text); //输入栏删除被选中
        // 20200401 symbolFaultTolerance
        m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
        // 20200316选中部分光标置位问题修复
        if ((seloldtext.mid(0, removepos).remove(QRegExp("[＋－×÷/,.%()e]")).length()) ==
                m_inputEdit->text().mid(0, removepos).remove(QRegExp("[＋－×÷/,.%()e]")).length())
            m_inputEdit->setCursorPosition(removepos);
        else if ((seloldtext.mid(0, removepos).remove(QRegExp("[＋－×÷/,.%()e]")).length()) >
                 m_inputEdit->text().mid(0, removepos).remove(QRegExp("[＋－×÷/,.%()e]")).length())
            m_inputEdit->setCursorPosition(removepos + 1);
        else
            m_inputEdit->setCursorPosition(removepos - 1);
        // 20200401 选中部分左侧为分隔符按退格的光标处理
        int curpos = m_inputEdit->cursorPosition();
//        if (selection.curpos > 0 && text.at(selection.curpos - 1) == ",") {
//            curpos = curpos - 1;
//            m_inputEdit->setCursorPosition(curpos);
//        }
        // fix for pointfault tolerance 16022
        if (pointFaultTolerance(m_inputEdit->text()) == m_inputEdit->text()) {
            QTimer::singleShot(5000, this, [ = ] {
                m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
                m_inputEdit->setCursorPosition(curpos);
            });
        }
        // end fix
        m_isResult = false;
        return;
    }
    QString text = m_inputEdit->text();
    int cur = m_inputEdit->cursorPosition();
    int funpos = -1;
    int i;
    if (text.size() > 0 && cur > 0 && text[cur - 1] == ",") {
        text.remove(cur - 2, 2);
        m_inputEdit->setText(text);
        // 20200401 symbolFaultTolerance
        m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
        m_inputEdit->setCursorPosition(cur - 2);
    } else {
        //退函数
        //光标不在开头且光标左侧是字母
        if (m_inputEdit->cursorPosition() > 0 && rx.exactMatch(m_inputEdit->text().at(m_inputEdit->cursorPosition() - 1))) {
            for (i = 0; i < m_funclist.size(); i++) {
                //记录光标左侧离光标最近的函数位
                funpos = m_inputEdit->text().lastIndexOf(m_funclist[i], m_inputEdit->cursorPosition() - 1);
                if (funpos != -1 && (funpos <= m_inputEdit->cursorPosition())
                        && (m_inputEdit->cursorPosition() <= funpos + m_funclist[i].length()))
                    break; //光标在函数开头和函数结尾之间
                else
                    funpos = -1;
            }
            if (funpos != -1) {
                m_inputEdit->setText(m_inputEdit->text().remove(funpos, m_funclist[i].length()));
                m_inputEdit->setCursorPosition(funpos);
            }
        } else {
            int proNumber = text.count(",");
            m_inputEdit->backspace();
            int separator = proNumber - m_inputEdit->text().count(",");
            // 20200401 symbolFaultTolerance
            m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
            int newPro = m_inputEdit->text().count(",");
            if (cur > 0) {
                QString sRegNum = "[0-9]+";
                QRegExp rx;
                rx.setPattern(sRegNum);
                //退数字
                if (rx.exactMatch(text.at(cur - 1)) && proNumber > newPro) {
                    if (text.mid(cur, text.length() - cur) == m_inputEdit->text().mid(m_inputEdit->text().length() - (text.length() - cur), text.length() - cur)) {
                        m_inputEdit->setCursorPosition(cur - 2);
                    } else
                        m_inputEdit->setCursorPosition(cur - 1);
                } else {
                    if (separator < 0) {
                        m_inputEdit->setCursorPosition(cur - 1 - separator);
                    } else {
                        m_inputEdit->setCursorPosition(cur - 1);
                    }
                }
                //退小数点
                if (text.at(cur - 1) == ".") {
                    if (text.mid(0, cur).count(",") != m_inputEdit->text().mid(0, cur).count(","))
                        m_inputEdit->setCursorPosition(cur);
                    else
                        m_inputEdit->setCursorPosition(cur - 1);
                }
            }
        }
    }
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }

    QTimer::singleShot(5000, this, [ = ] {
        int curpos = m_inputEdit->cursorPosition();
        m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
        m_inputEdit->setCursorPosition(curpos);
    });

    m_isContinue = true;
    m_isUndo = false;
}

void SciExpressionBar::enterClearEvent()
{
    if (m_isAllClear) {
        m_listModel->clearItems();
        m_listView->reset();
        m_isAllClear = false;
        m_unfinishedExp.clear();
        m_isAutoComputation = false;

        emit clearStateChanged(false);
    } else {
        if (m_listModel->rowCount(QModelIndex()) == 0)
            emit clearStateChanged(false);
        else
            emit clearStateChanged(true);

        m_inputEdit->clear();
        m_isAllClear = true;
    }
    m_isResult = false;
    m_isUndo = false;
//    m_Selected = -1;
    addUndo();
    m_FEisdown = false;
    emit fEStateChanged(false);
}

void SciExpressionBar::enterEqualEvent()
{
//    m_evaluator->setVariable(QLatin1String("e"), DMath::e(), Variable::BuiltIn);
    QString exp = m_inputEdit->text();
    if (m_inputEdit->text().isEmpty())
        return;
    const QString expression = formatExpression(m_inputEdit->expressionText());
    QString exp1 = symbolComplement(expression);
    m_evaluator->setExpression(exp1);
    Quantity ans = m_evaluator->evalUpdateAns();
    // 20200403 bug-18971 表达式错误时输数字加等于再重新输入表达式历史记录错误表达式未被替换
    // 20200407 超过16位小数未科学计数
    qDebug() << "m_evaluator->error()" << m_evaluator->error();
    if (m_evaluator->error().isEmpty() && (exp.indexOf(QRegExp("[a-z＋－×÷/.,%()πℯ^!]")) != -1)) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;
        //edit 20200413 for bug--19653
        QString result;
        if (m_FEisdown)
            result = DMath::format(ans, Quantity::Format::Scientific() + Quantity::Format::Precision(SCIPREC));
        else
            result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(SCIPREC));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                       .replace(QString::fromUtf8("－"), "-")
                       .replace(QString::fromUtf8("×"), "*")
                       .replace(QString::fromUtf8("÷"), "/");
        //.replace(QString::fromUtf8(","), "");

        //        QString tStr = m_inputEdit->text().replace(QString::fromUtf8(","), "");
        QString tStr = m_inputEdit->text();
        //edit 20200518 for bug-26628
        QString StrToComp = formatResult;
        StrToComp = StrToComp.replace("+", QString::fromUtf8("＋"))
                    .replace("-", QString::fromUtf8("－"))
                    .replace("*", QString::fromUtf8("×"))
                    .replace("/", QString::fromUtf8("÷"));
        if (StrToComp == exp) {
            m_pair.first = false;
            return;
        }
        //end edit 20200518 for bug-26628
        // 20200402 需求3.2.1.6当输入的数字中有千位符，点击等号视为前后一致，不计入表达式
        if (formatResult != tStr) {
//        m_listModel->updataList(m_inputEdit->text() + "＝" + formatResult, m_hisRevision);
            m_inputEdit->setAnswer(formatResult, ans);
        }
        m_isContinue = false;
        m_inputEdit->setText(formatResult);
        formatResult = formatResult.replace(QString::fromUtf8("-"), "－");

        m_pair.first = true;
        m_expression = exp + "＝" + formatResult;
        m_listModel->updataList(m_expression,
                                -1, true);
        m_listanswer = ans;
    } else {
        m_pair.first = false;
        if (!m_evaluator->error().isEmpty()) {
            m_expression = exp + "＝" + tr("Expression error");
            m_listModel->updataList(m_expression,
                                    -1, true);
        } else {
            return;
        }
    }

    m_isResult = true;
    m_isUndo = false;
}

void SciExpressionBar::enterBracketsEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString oldText = m_inputEdit->text();
    int currentPos = m_inputEdit->cursorPosition();
    m_inputEdit->insert("()");
    // 20200401 symbolFaultTolerance
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    if (oldText.mid(0, currentPos).count(",") == m_inputEdit->text().mid(0, currentPos).count(","))
        m_inputEdit->setCursorPosition(currentPos + 1);
    else
        m_inputEdit->setCursorPosition(currentPos);
    m_isUndo = false;
}

void SciExpressionBar::enterLeftBracketsEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
}

void SciExpressionBar::enterRightBracketsEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert(")");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
}

void SciExpressionBar::enterDeleteEvent()
{
    m_inputEdit->clear();
    m_isUndo = false;
}

void SciExpressionBar::enterDegEvent(int mod)
{
    //mod=1,enter走rad运算
    emit turnDeg();
    if (mod == 1)
        Settings::instance()->angleUnit = 'r';
    else if (mod == 2) {
        Settings::instance()->angleUnit = 'g';
    } else {
        Settings::instance()->angleUnit = 'd';
    }
}

void SciExpressionBar::enterSinEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("sin(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 3);
        } else {
            m_inputEdit->setCursorPosition(curpos + 4);
        }
    }
}

void SciExpressionBar::enterFEEvent(bool isdown)
{
    if (isdown) {
        m_FEisdown = false;
        emit fEStateChanged(false);
    } else {
        m_FEisdown = true;
        emit fEStateChanged(true);
    }
}

void SciExpressionBar::enterPIEvent()
{
    if (m_isResult) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert(QString::fromUtf8("π"));
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
}

/**
 * @brief SciExpressionBar::无理数e
 */
void SciExpressionBar::enterEulerEvent()
{
    if (m_isResult) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert(QString::fromUtf8("ℯ"));
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
}

void SciExpressionBar::enterModEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0mod");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0mod");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0mod");
            diff = 2;
        } else {
            m_inputEdit->insert("0mod");
            diff = 1;
        }
    } else
        m_inputEdit->insert("mod");

    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 3 + diff);
    }
}

void SciExpressionBar::enterx2Event()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0^2");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0^2");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0^2");
            diff = 2;
        } else {
            m_inputEdit->insert("0^2");
            diff = 1;
        }
    } else
        m_inputEdit->insert("^2");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;
    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 1 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 2 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 2 + diff);
    }
    if (!cursorPosAtEnd()) {
        if (m_inputEdit->text().at(m_inputEdit->cursorPosition()).isNumber()) {
            m_inputEdit->insert("×");
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - 1);
        }
    }
}

/**
 * @brief SciExpressionBar::倒数
 */
void SciExpressionBar::enterDerivativeEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        return;
    }
    bool hasselect = (m_inputEdit->getSelection().selected != "");
    QString oldText = m_inputEdit->text();
    QString exp = m_inputEdit->text();
    // 20200316百分号选中部分格式替代
    replaceSelection(m_inputEdit->text());
    int curPos = m_inputEdit->cursorPosition();
    if (m_inputEdit->text() == QString()) {
        m_inputEdit->setText("1/(");
        return;
    }
    int epos = m_inputEdit->text().indexOf("e");
    QString sRegNum = "[0-9,.e]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curPos == 0 && hasselect == false) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    if ((curPos == 0 && hasselect == true) ||
            (m_inputEdit->text().length() > curPos && rx.exactMatch(m_inputEdit->text().at(curPos)))) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    if (epos > -1 && epos == curPos - 1) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    // start edit for task-13519
    //        QString sRegNum1 = "[^0-9,.×÷)]";
    QString sRegNum1 = "[^0-9,.)]";
    QRegExp rx1;
    rx1.setPattern(sRegNum1);
    if (rx1.exactMatch(exp.at(curPos - 1)))
        m_inputEdit->setText(oldText);
    else {
        QString newtext = m_inputEdit->text();
        int percentpos = m_inputEdit->cursorPosition();
        int operatorpos =
            newtext.lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.e]")), percentpos - 1);

        bool nooperator = false;
        if (operatorpos > 0 && newtext.at(operatorpos - 1) == "e")
            operatorpos =
                newtext.mid(0, operatorpos - 1)
                .lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.e]")), percentpos - 1);
        if (operatorpos < 0) {
            operatorpos++;
            nooperator = true;
        }
        QString exptext;  //%表达式
        if (newtext.at(percentpos - 1) == ')') {
            if (operatorpos > 0 && newtext.at(operatorpos - 1) == '(') {
                m_inputEdit->setText(oldText);
                m_inputEdit->setCursorPosition(percentpos);
                return;
            }
            do {
                operatorpos = newtext.lastIndexOf('(', operatorpos - 1);
                if (operatorpos <= 0) {
                    break;
                }
            } while (newtext.mid(operatorpos, newtext.size() - operatorpos).count('(') !=
                     newtext.mid(operatorpos, percentpos - operatorpos).count(')'));
            exptext = newtext.mid(operatorpos,
                                  percentpos - operatorpos);  //截取%表达式
        } else {
            exptext = newtext.mid(operatorpos + (nooperator == true ? 0 : 1),
                                  percentpos - operatorpos + (nooperator == true ? 1 : 0) - 1);
            //截取%表达式
        }
//        QString express = symbolComplement(exptext);
        if (exptext.count("(") == exptext.count(")")) {
            m_inputEdit->setCursorPosition(curPos - exptext.length());
            m_inputEdit->insert("1/(");
            int afterinsertpos = m_inputEdit->cursorPosition();
            m_inputEdit->setCursorPosition(afterinsertpos + exptext.length());
            m_inputEdit->insert(")");
        }
    }
}

/**
 * @brief SciExpressionBar::阶乘
 */
void SciExpressionBar::enterFactorialsEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0!");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^!%]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0 || rx.exactMatch(exp.at(curpos - 1))) {
        m_inputEdit->insert("0!");
        diff = 1;
    } else
        m_inputEdit->insert("!");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 1 + diff);
    }
}

void SciExpressionBar::enterExpEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0.e＋");
        return;
    }

    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0.e＋");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0.e＋");
            diff = 2;
        } else {
            m_inputEdit->insert("0.e＋");
            diff = 1;
        }
    } else
        m_inputEdit->insert(".e＋");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 3 + diff);
    }
}

void SciExpressionBar::enterCosEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("cos(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 3);
        } else {
            m_inputEdit->setCursorPosition(curpos + 4);
        }
    }
}

void SciExpressionBar::enterx3Event()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0^3");
        return;
    }

    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0^3");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0^3");
            diff = 2;
        } else {
            m_inputEdit->insert("0^3");
            diff = 1;
        }
    } else
        m_inputEdit->insert("^3");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;
    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 1 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 2 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 2 + diff);
    }
    if (!cursorPosAtEnd()) {
        if (m_inputEdit->text().at(m_inputEdit->cursorPosition()).isNumber()) {
            m_inputEdit->insert("×");
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - 1);
        }
    }
}

void SciExpressionBar::enterTanEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("tan(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 3);
        } else {
            m_inputEdit->setCursorPosition(curpos + 4);
        }
    }
}

void SciExpressionBar::enterxyEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->insert("0^");
        return;
    }

    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0^");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0^");
            diff = 2;
        } else {
            m_inputEdit->insert("0^");
            diff = 1;
        }
    } else
        m_inputEdit->insert("^");

    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 1 + diff);
    }
}

void SciExpressionBar::enterCotEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("cot(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 3);
        } else {
            m_inputEdit->setCursorPosition(curpos + 4);
        }
    }
}

void SciExpressionBar::enter10xEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    QString sRegNum = "[0-9eℯπ,.]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos > 0 && rx.exactMatch(m_inputEdit->text().at(curpos - 1))) {
        m_inputEdit->insert("*10^");
    } else
        m_inputEdit->insert("10^");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    rx.setPattern("[0-9eℯπ,)]");
    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            if (rx.exactMatch(m_inputEdit->text().at(curpos - 1)) | rx.exactMatch(exp.at(curpos - 1)))
                m_inputEdit->setCursorPosition(curpos + 3);
            else
                m_inputEdit->setCursorPosition(curpos + 2);
        } else {
            if (rx.exactMatch(m_inputEdit->text().at(curpos - 1)))
                m_inputEdit->setCursorPosition(curpos + 4);
            else
                m_inputEdit->setCursorPosition(curpos + 3);
        }
    } else {
        if (curpos > 0 && rx.exactMatch(m_inputEdit->text().at(curpos - 1)))
            m_inputEdit->setCursorPosition(curpos + 4);
        else
            m_inputEdit->setCursorPosition(curpos + 3);
    }
}

/**
 * @brief SciExpressionBar:://模(绝对值)
 */
void SciExpressionBar::enterModulusEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("abs(0)");
        return;
    }
    bool hasselect = (m_inputEdit->getSelection().selected != "");
    QString oldText = m_inputEdit->text();
    QString exp = m_inputEdit->text();
    // 20200316百分号选中部分格式替代
    replaceSelection(m_inputEdit->text());
    int curPos = m_inputEdit->cursorPosition();
    if (m_inputEdit->text() == QString()) {
        m_inputEdit->setText("abs(");
        return;
    }
    int epos = m_inputEdit->text().indexOf("e");
    QString sRegNum = "[0-9,.e]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curPos == 0 && hasselect == false) {
        return;
    }
    if ((curPos == 0 && hasselect == true) ||
            (m_inputEdit->text().length() > curPos && rx.exactMatch(m_inputEdit->text().at(curPos)))) {
        return;
    }
    if (epos > -1 && epos == curPos - 1) {
        return;
    }
    // start edit for task-13519
    //        QString sRegNum1 = "[^0-9,.×÷)]";
    QString sRegNum1 = "[^0-9,.)!]";
    rx.setPattern(sRegNum1);
    if (rx.exactMatch(exp.at(curPos - 1)))
        m_inputEdit->setText(oldText);
    else {
        QString newtext = m_inputEdit->text();
        int percentpos = m_inputEdit->cursorPosition();
        if (m_inputEdit->text().at(curPos - 1) == "!")
            percentpos = percentpos - 1;
        int operatorpos =
            newtext.lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.e]")), percentpos - 1);

        bool nooperator = false;
        if (operatorpos > 0 && newtext.at(operatorpos - 1) == "e")
            operatorpos =
                newtext.mid(0, operatorpos - 1)
                .lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.e]")), percentpos - 1);
        if (operatorpos < 0) {
            operatorpos++;
            nooperator = true;
        }
        QString exptext;  //%表达式
        if (newtext.at(percentpos - 1) == ')') {
            if (operatorpos > 0 && newtext.at(operatorpos - 1) == '(') {
                m_inputEdit->setText(oldText);
                m_inputEdit->setCursorPosition(percentpos);
                return;
            }
            do {
                operatorpos = newtext.lastIndexOf('(', operatorpos - 1);
                if (operatorpos <= 0) {
                    break;
                }
            } while (newtext.mid(operatorpos, newtext.size() - operatorpos).count('(') !=
                     newtext.mid(operatorpos, percentpos - operatorpos).count(')'));
            exptext = newtext.mid(operatorpos,
                                  percentpos - operatorpos);  //截取%表达式
        } else {
            exptext = newtext.mid(operatorpos + (nooperator == true ? 0 : 1),
                                  percentpos - operatorpos + (nooperator == true ? 1 : 0) - 1);
            //截取%表达式
        }
//        QString express = symbolComplement(exptext);
        if (exptext.count("(") == exptext.count(")")) {
            m_inputEdit->setCursorPosition(curPos - exptext.length());
            m_inputEdit->insert("abs(");
            int afterinsertpos = m_inputEdit->cursorPosition();
            m_inputEdit->setCursorPosition(afterinsertpos + exptext.length());
            m_inputEdit->insert(")");
        }
    }
    rx.setPattern("sRegNum1");
}

void SciExpressionBar::enterLogEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("lg(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 3);
    }
}

void SciExpressionBar::enterRandEvent()
{
    m_inputEdit->clear();
    m_isResult = false;
    m_isUndo = false;
    QString str;
    for (int i = 0; i < SCIPREC; i++) {
        int n = qrand() % 10;
        str.append(QString::number(n));
    }
    str = "0." + str;
    m_isResult = false;
    m_inputEdit->setText(str);
}

void SciExpressionBar::enterLnEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("ln(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 3);
    }
}

void SciExpressionBar::enterArcsinEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("arcsin(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 6);
        } else {
            m_inputEdit->setCursorPosition(curpos + 7);
        }
    }
}

void SciExpressionBar::enterSqrtEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("sqrt(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 4);
        } else {
            m_inputEdit->setCursorPosition(curpos + 5);
        }
    }
}

void SciExpressionBar::enterArccosEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("arccos(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 6);
        } else {
            m_inputEdit->setCursorPosition(curpos + 7);
        }
    }
}

void SciExpressionBar::enterCbrtEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("cbrt(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 4);
        } else {
            m_inputEdit->setCursorPosition(curpos + 5);
        }
    }
}

void SciExpressionBar::enterArctanEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("arctan(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 6);
        } else {
            m_inputEdit->setCursorPosition(curpos + 7);
        }
    }
}

void SciExpressionBar::enterYrootEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0yroot");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0yroot");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0yroot");
            diff = 2;
        } else {
            m_inputEdit->insert("0yroot");
            diff = 1;
        }
    } else
        m_inputEdit->insert("yroot");

    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 4 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 5 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 5 + diff);
    }
}

void SciExpressionBar::enterArccotEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("arccot(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 6);
        } else {
            m_inputEdit->setCursorPosition(curpos + 7);
        }
    }
}

void SciExpressionBar::enter2xEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    QString sRegNum = "[0-9eℯπ,.]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos > 0 && rx.exactMatch(m_inputEdit->text().at(curpos - 1))) {
        m_inputEdit->insert("*2^");
    } else
        m_inputEdit->insert("2^");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    rx.setPattern("[0-9eℯπ,)]");
    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            if (rx.exactMatch(m_inputEdit->text().at(curpos - 1)) | rx.exactMatch(exp.at(curpos - 1)))
                m_inputEdit->setCursorPosition(curpos + 2);
            else
                m_inputEdit->setCursorPosition(curpos + 1);
        } else {
            if (rx.exactMatch(m_inputEdit->text().at(curpos - 1)))
                m_inputEdit->setCursorPosition(curpos + 3);
            else
                m_inputEdit->setCursorPosition(curpos + 2);
        }
    } else {
        if (curpos > 0 && rx.exactMatch(m_inputEdit->text().at(curpos - 1)))
            m_inputEdit->setCursorPosition(curpos + 3);
        else
            m_inputEdit->setCursorPosition(curpos + 2);
    }
}

void SciExpressionBar::enterlogyxEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0log");
        return;
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    /*
     * 当光标位置的前一位是运算符时，在函数方法前面补0,当函数的运算优先级小于等于
     * 前一位运算符时，则补（0
     */
    int diff = 0; //补数字后光标位移的距离
    QString sRegNum = "[＋－×÷/(^]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curpos == 0) {
        m_inputEdit->insert("0log");
        diff = 1;
    } else if (rx.exactMatch(exp.at(curpos - 1))) {
        if (exp.at(curpos - 1) == "^") {
            m_inputEdit->insert("(0log");
            diff = 2;
        } else {
            m_inputEdit->insert("0log");
            diff = 1;
        }
    } else
        m_inputEdit->insert("log");

    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2 + diff);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3 + diff);
        }
    } else {
        m_inputEdit->setCursorPosition(curpos + 3 + diff);
    }
}

void SciExpressionBar::enterexEvent()
{
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("e^(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos + 2);
        } else {
            m_inputEdit->setCursorPosition(curpos + 3);
        }
    }
}

/**
 * @brief SciExpressionBar::相反数
 */
void SciExpressionBar::enterOppositeEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        return;
    }
    m_isResult = false;
    m_isUndo = false;
    bool hasselect = (m_inputEdit->getSelection().selected != "");
    QString oldText = m_inputEdit->text();
    QString exp = m_inputEdit->text();
    // 20200316百分号选中部分格式替代
    replaceSelection(m_inputEdit->text());
    int curPos = m_inputEdit->cursorPosition();
    if (m_inputEdit->text() == QString()) {
        m_inputEdit->setText("(-");
        return;
    }
    int epos = m_inputEdit->text().indexOf("e");
    QString sRegNum = "[0-9,.e]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    if (curPos == 0 && hasselect == false) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    if ((curPos == 0 && hasselect == true) ||
            (m_inputEdit->text().length() > curPos && rx.exactMatch(m_inputEdit->text().at(curPos)))) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    if (epos > -1 && epos == curPos - 1) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curPos);
        return;
    }
    // start edit for task-13519
    //        QString sRegNum1 = "[^0-9,.×÷)]";
    QString sRegNum1 = "[^0-9,.)πℯ]";
    QRegExp rx1;
    rx1.setPattern(sRegNum1);
    if (rx1.exactMatch(exp.at(curPos - 1)))
        m_inputEdit->setText(oldText);
    else {
        QString newtext = m_inputEdit->text();
        int percentpos = m_inputEdit->cursorPosition();
        int operatorpos =
            newtext.lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.eπℯ]")), percentpos - 1);

        bool nooperator = false;
        if (operatorpos > 0 && newtext.at(operatorpos - 1) == "e")
            operatorpos =
                newtext.mid(0, operatorpos - 1)
                .lastIndexOf(QRegularExpression(QStringLiteral("[^0-9,.eπℯ]")), percentpos - 1);
        if (operatorpos < 0) {
            operatorpos++;
            nooperator = true;
        }
        QString exptext;  //%表达式
        if (newtext.at(percentpos - 1) == ')') {
            if (operatorpos > 0 && newtext.at(operatorpos - 1) == '(') {
                m_inputEdit->setText(oldText);
                m_inputEdit->setCursorPosition(percentpos);
                return;
            }
            do {
                operatorpos = newtext.lastIndexOf('(', operatorpos - 1);
                if (operatorpos <= 0) {
                    break;
                }
            } while (newtext.mid(operatorpos, newtext.size() - operatorpos).count('(') !=
                     newtext.mid(operatorpos, percentpos - operatorpos).count(')'));
            exptext = newtext.mid(operatorpos,
                                  percentpos - operatorpos);  //截取%表达式
        } else {
            exptext = newtext.mid(operatorpos + (nooperator == true ? 0 : 1),
                                  percentpos - operatorpos + (nooperator == true ? 1 : 0) - 1);
            //截取%表达式
        }
//        QString express = symbolComplement(exptext);
        if (exptext.count("(") == exptext.count(")")) {
            m_inputEdit->setCursorPosition(curPos - exptext.length());
            m_inputEdit->insert("(-");
            int afterinsertpos = m_inputEdit->cursorPosition();
            m_inputEdit->setCursorPosition(afterinsertpos + exptext.length());
            m_inputEdit->insert(")");
        }
    }
}

void SciExpressionBar::copyResultToClipboard()
{
    if (m_inputEdit->text().isEmpty())
        return;
    // QApplication::clipboard()->setText(m_inputEdit->selectedText());

    SSelection selection = m_inputEdit->getSelection();
    QApplication::clipboard()->setText(selection.selected); //将选中项放入粘贴板

//    const QString expression = formatExpression(m_inputEdit->text());
//    m_evaluator->setExpression(expression);
//    Quantity ans = m_evaluator->evalUpdateAns();

//    if (m_evaluator->error().isEmpty()) {
//        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
//            return;

//        //edit 20200413 for bug--19653
//        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(SCIPREC));
//        QString formatResult = Utils::formatThousandsSeparators(result);
//        formatResult = formatResult.replace('-', "－").replace('+', "＋");
//        // m_inputEdit->setAnswer(formatResult, ans);

//        QApplication::clipboard()->setText(formatResult);
//    } else {
//        QApplication::clipboard()->setText(m_inputEdit->text());
//    }
}

void SciExpressionBar::copyClipboard2Result()
{
//    if (!m_hisLink.isEmpty() && m_hisLink.last().linkedItem == -1) {
//        m_hisLink.last().linkedItem = m_listModel->rowCount(QModelIndex());
//        m_hisLink.last().isLink = true;
//        m_listDelegate->setHisLinked(m_hisLink.last().linkedItem);
//        m_isLinked = false;
//        if (m_hisLink.size() > 9) {
//            m_hisLink.removeFirst();
//            m_listDelegate->removeLine(0);
//        }
//    }
//    //    clearSelectSymbol();   //fix for bug-14117
//    if (m_isUndo) {
//        int row = m_listModel->rowCount(QModelIndex());
//        if (row != 0) {
//            QString text =
//                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
//            QString result = text.split("＝").last();
//            if (result == m_inputEdit->text()) {
//                historicalLinkageIndex his;
//                his.linkageTerm = row - 1;
//                his.linkageValue = result;
//                his.linkedItem = row;
//                m_hisLink.append(his);
//                m_listDelegate->setHisLink(row - 1);
//                m_listDelegate->setHisLinked(row);
//            }
//        }
//    }
    QString oldText = m_inputEdit->text(); //未粘贴操作的text
    int curpos = m_inputEdit->cursorPosition(); //未粘贴操作的光标位
    replaceSelection(oldText);
    QString exp = m_inputEdit->text();
    QString text = QApplication::clipboard()->text();
    text = text.replace('+', QString::fromUtf8("＋"))
           .replace('-', QString::fromUtf8("－"))
           .replace("_", QString::fromUtf8("－"))
           .replace('*', QString::fromUtf8("×"))
           .replace('x', QString::fromUtf8("×"))
           .replace('X', QString::fromUtf8("×"))
           .replace(QString::fromUtf8("＊"), QString::fromUtf8("×"))
           .replace(QString::fromUtf8("（"), "(")
           .replace(QString::fromUtf8("）"), ")")
           .replace(QString::fromUtf8("。"), ".")
           .replace(QString::fromUtf8("——"), QString::fromUtf8("－"))
           .replace(QString::fromUtf8("％"), "%")
           .replace('/', QString::fromUtf8("÷")); //对粘贴板中的内容进行英替中

    //匹配函数方法
    QStringList list = text.split(QRegExp("[0-9＋－×÷/()%^!e.,]")); //正则表达式中为科学模式下可存在的非字母;函数中;无法被复制
    for (int i = 0; i < list.size(); i++) {
        QString item = list[i];
        for (int j = 0; j < m_funclist.size(); j++) {
            if (item.toLower().contains(m_funclist[j])) {
                item.replace(item, m_funclist[j]); //包含函数名的取出;item中若存在两个函数名，只可以替代最前面的函数名
                break;
            }
            if (j == m_funclist.size() - 1)
                item.replace(item, QString());
        }
        text.replace(list[i], item);
    }
//    text = pasteFaultTolerance(text);
    m_inputEdit->insert(text);

    QString faulttolerance = pointFaultTolerance(m_inputEdit->text());
    faulttolerance = m_inputEdit->symbolFaultTolerance(faulttolerance);
    if (faulttolerance != m_inputEdit->text())
        m_inputEdit->setText(faulttolerance); //如果经过容错处理的表达式有改变，重新设置表达式，不设置光标
    if (m_inputEdit->text() == exp) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curpos);
        qDebug() << "Invalid content"; //提示是否复制了无效内容,复制的内容全是字母等
    }
    if (!m_inputEdit->text().isEmpty())
        emit clearStateChanged(false);
    m_isResult = false;
    m_isUndo = false;
}

void SciExpressionBar::allElection()
{
    m_inputEdit->selectAll();
    SSelection selection;
    selection.selected = m_inputEdit->text();
    m_inputEdit->setSelection(selection);
}

void SciExpressionBar::shear()
{
    QString text = m_inputEdit->text();
    QString selectText = m_inputEdit->selectedText();
    selectText = selectText.replace(",", "");
    QApplication::clipboard()->setText(selectText);
    int start = m_inputEdit->selectionStart();
    int length = m_inputEdit->selectionLength();
    text.remove(start, length);
    m_inputEdit->setText(text);
    addUndo();
    m_isUndo = false;
}

void SciExpressionBar::handleTextChanged(const QString &text)
{
    Q_UNUSED(text);
    m_isAllClear = false;
    m_isContinue = true;
}

bool SciExpressionBar::cursorPosAtEnd()
{
    return m_inputEdit->cursorPosition() == m_inputEdit->text().length();
}

QString SciExpressionBar::formatExpression(const QString &text)
{
    return QString(text)
           .replace(QString::fromUtf8("＋"), "+")
           .replace(QString::fromUtf8("－"), "-")
           .replace(QString::fromUtf8("×"), "*")
           .replace(QString::fromUtf8("÷"), "/")
           .replace(QString::fromUtf8(","), "")
           .replace(QString::fromUtf8("π"), QLatin1String("pi"))
           .replace(QString::fromUtf8("ℯ"), QLatin1String("E")); //edit jingzhou 20200721,用E代替e，区分于科学计数法
}

void SciExpressionBar::revisionResults(const QModelIndex &index)
{
//    clearLinkageCache(m_inputEdit->text(), false);
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString(" ＝ "), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = historic.at(0);
//    m_hisRevision = index.row();
    m_inputEdit->setText(expression);
//    m_Selected = m_hisRevision;
    m_isResult = false;
    // fix addundo for history revision
    m_isUndo = false;
    addUndo();

    emit clearStateChanged(false);
}

void SciExpressionBar::hisRevisionResults(const QModelIndex &index, Quantity ans)
{
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString(" ＝ "), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(SCIPREC));
//    m_hisRevision = index.row();
    m_inputEdit->setAnswer(expression, ans);
    m_listModel->updataList(text, -1, true);
//    m_Selected = m_hisRevision;
    m_isResult = false;
    // fix addundo for history revision
    m_isUndo = false;
    addUndo();

    emit clearStateChanged(false);
}

bool SciExpressionBar::judgeinput()
{
    QString sRegNum = "[A-Za-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    SSelection selection = m_inputEdit->getSelection();

    if (selection.selected != "") {
        //光标不在开头且光标左侧是字母或者光标右侧是字母
        if ((selection.curpos > 0 &&
                rx.exactMatch(m_inputEdit->text().at(selection.curpos - 1)))
                || rx.exactMatch(m_inputEdit->text().at(selection.curpos + selection.selected.size()))) {
            int funpos = -1;
            int rightfunpos = -1;
            for (int i = 0; i < m_funclist.size(); i++) {
                //记录光标左侧离光标最近的函数位
                funpos = m_inputEdit->text().lastIndexOf(m_funclist[i], selection.curpos - 1);
                if (funpos != -1 && (funpos <= selection.curpos) && (selection.curpos < funpos + m_funclist[i].length()))
                    return false; //选中左侧在函数中

            }
            for (int j = 0; j < m_funclist.size(); j++) {
                //记录离光标最近的右侧函数位
                rightfunpos = m_inputEdit->text().lastIndexOf(m_funclist[j], selection.curpos + selection.selected.size() - 1);
                if (rightfunpos != -1 && (rightfunpos + m_funclist[j].length() > selection.curpos + selection.selected.size()))
                    return false; //选中右侧在函数中
            }
        }
        return true;
    } else {
        if (m_inputEdit->cursorPosition() > 0 && rx.exactMatch(m_inputEdit->text().at(m_inputEdit->cursorPosition() - 1))) {
            for (int i = 0; i < m_funclist.size(); i++) {
                //记录光标左侧离光标最近的函数位
                int funpos = m_inputEdit->text().lastIndexOf(m_funclist[i], m_inputEdit->cursorPosition() - 1);
                if (funpos != -1 && (funpos < m_inputEdit->cursorPosition()) && (m_inputEdit->cursorPosition() < funpos + m_funclist[i].length()))
                    return false; //光标在函数中
            }
        }
        return true;
    }
}

void SciExpressionBar::computationalResults(const QString &expression, QString &result)
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
        m_inputEdit->setAnswer(result, ans);

        if (result != m_inputEdit->text()) {
            m_isContinue = false;
        }
    } else {
        result = tr("Expression error");
        m_inputEdit->setText(result);
    }
}

QString SciExpressionBar::completedBracketsCalculation(QString &text)
{
    int leftBrack = text.count("(");
    int rightBrack = text.count(")");
    QString newText = text;
    if (leftBrack > rightBrack) {
        for (int i = 0; i < leftBrack - rightBrack; i++)
            newText += ")";
    }

    return newText;
}

void SciExpressionBar::initConnect()
{
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, this,
            &SciExpressionBar::revisionResults);
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, m_inputEdit,
            &InputEdit::hisexpression);
    connect(m_inputEdit, &InputEdit::textChanged, this, &SciExpressionBar::handleTextChanged);
    connect(m_inputEdit, &InputEdit::keyPress, this, &SciExpressionBar::keyPress);
    connect(m_inputEdit, &InputEdit::equal, this, &SciExpressionBar::enterEqualEvent);
    connect(m_inputEdit, &InputEdit::cut, this, &SciExpressionBar::shear);
    connect(m_inputEdit, &InputEdit::copy, this, &SciExpressionBar::copyResultToClipboard);
    connect(m_inputEdit, &InputEdit::paste, this, &SciExpressionBar::copyClipboard2Result);
    connect(m_inputEdit, &InputEdit::deleteText, this, &SciExpressionBar::enterClearEvent);
    connect(m_inputEdit, &InputEdit::selectAllText, this, &SciExpressionBar::allElection);
    connect(m_inputEdit, &InputEdit::undo, this, &SciExpressionBar::Undo);
    connect(m_inputEdit, &InputEdit::redo, this, &SciExpressionBar::Redo);
    connect(m_inputEdit, &InputEdit::setResult, this, &SciExpressionBar::setResultFalse);
}

QString SciExpressionBar::symbolComplement(const QString exp)
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

/**
 * @brief 粘贴容错（20200716，粘贴时不再使用此函数）
 * @param exp 复制的内容
 * @return 粘贴后的text
 */
QString SciExpressionBar::pasteFaultTolerance(QString exp)
{
    exp = m_inputEdit->text().insert(m_inputEdit->cursorPosition(), exp);
    exp = Utils::reformatSeparators(QString(exp).remove(',').remove(QString::fromUtf8("，")));
    exp = pointFaultTolerance(exp); //避免粘贴后的text有两个.
    for (int i = 0; i < exp.size(); ++i) {
        while (exp[i].isNumber()) {
            if (exp[i] == "0" && exp[i + 1] != "." && (i == 0 || exp[i - 1] != ".") &&
                    (i == 0 || !exp[i - 1].isNumber()) && (exp.size() == 1 || exp[i + 1].isNumber())) {
                exp.remove(i, 1); //0的容错处理，例:将0123的0去除 //在输入符号时也会进行此操作
                --i;
            }
            ++i;
        }
        if (exp[i] == "." && (i == 0 || !exp[i - 1].isNumber())) {
            exp.insert(i, "0");
            ++i;
        }
    }
    //匹配函数方法
    QStringList list = exp.split(QRegExp("[0-9＋－×÷/()%^!e.,]"));
    for (int i = 0; i < list.size(); i++) {
        QString item = list[i];
        for (int j = 0; j < m_funclist.size(); j++) {
            if (item.toLower().contains(m_funclist[j])) {
                item.replace(item, m_funclist[j]);
                break;
            }
            if (j == m_funclist.size() - 1)
                item.replace(item, QString());
        }
        exp.replace(list[i], item);
    }
    return exp;
}

QString SciExpressionBar::pointFaultTolerance(const QString &text)
{
    QString oldText = text;
    // QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
    QString reformatStr = oldText.replace('+', QString::fromUtf8("＋"))
                          .replace('-', QString::fromUtf8("－"))
                          .replace("_", QString::fromUtf8("－"))
                          .replace('*', QString::fromUtf8("×"))
                          .replace('x', QString::fromUtf8("×"))
                          .replace('X', QString::fromUtf8("×"))
                          .replace(QString::fromUtf8("＊"), QString::fromUtf8("×"))
                          .replace(QString::fromUtf8("（"), "(")
                          .replace(QString::fromUtf8("）"), ")")
                          .replace(QString::fromUtf8("。"), ".")
                          .replace(QString::fromUtf8("——"), QString::fromUtf8("－"))
                          .replace(QString::fromUtf8("％"), "%")
                          /*.replace('/', QString::fromUtf8("÷"))*/; //对内容进行英替中
    QStringList list = reformatStr.split(QRegExp("[＋－×÷/(]")); //20200717去掉),否则下方)小数点容错无法进入
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        int firstPoint = item.indexOf(".");
        if (firstPoint == -1)
            continue;
        if (firstPoint == 0) {
            item.insert(firstPoint, "0"); //小数点在数字前，进行补0;例:.123->0.123;此处未对reformatStr进行操作，导致只有两个.时才会进行补0
            ++firstPoint;
            // oldText.replace(list[i], item);
        } else {
            if (item.at(firstPoint - 1) == ")" || item.at(firstPoint - 1) == "%") {
                item.remove(firstPoint, 1);
                item.insert(firstPoint, "0."); //20200717)及%后小数点补0;与小数点输入处理一致
                reformatStr.replace(list[i], item);
            }
        }
        if (item.count(".") > 1) {
            item.remove(".");
            item.insert(firstPoint, ".");
            reformatStr.replace(list[i], item); //去除多余.
        }
    }
    for (int i = 0; i < reformatStr.size(); ++i) {
        if (reformatStr[i] == "." && (i == 0 || !reformatStr[i - 1].isNumber())) {
            reformatStr.insert(i, "0"); //补0操作，例:1+.2->1+0.2
            ++i;
        }
    }

    return reformatStr;
}

void SciExpressionBar::clearSelectSymbol()
{
    SSelection select = m_inputEdit->getSelection();
    if (select.selected.size() == 1 && (select.selected == "＋" || select.selected == "－" ||
                                        select.selected == "×" || select.selected == "÷")) {
        select.selected = "";
        QString exp = m_inputEdit->text();
        exp.remove(select.curpos, 1);
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(select.curpos);
    }
}

void SciExpressionBar::expressionCheck()
{
    QString exp = m_inputEdit->text();
    int cur = m_inputEdit->cursorPosition();
    //光标前的分隔符
    int separator = 0;

    for (int i = 0; i < exp.size(); ++i) {
        if (exp[i] == ",") {
            exp.remove(i, 1);
            --i;
            if (i + 1 < cur) {
                ++separator;
                --cur;
            }
        }
    }
    for (int i = 0; i < exp.size(); ++i) {
        while (exp[i].isNumber()) {
            // fix for delete 0 behind "."
            if (exp[i] == "0" && exp[i + 1] != "." && (i == 0 || exp[i - 1] != ".") &&
                    (i == 0 || !exp[i - 1].isNumber()) && (exp.size() == 1 || exp[i + 1].isNumber())) {
                exp.remove(i, 1);
                --i;
                if (i + 1 < cur)
                    --cur;
            }
            ++i;
        }
        if (exp[i] == "." && (i == 0 || !exp[i - 1].isNumber())) {
            exp.insert(i, "0");
            ++i;
            if (i < cur)
                ++cur;
        }
    }
    m_inputEdit->setText(exp);
    m_inputEdit->setCursorPosition(cur + separator);
}

void SciExpressionBar::Undo()
{
    if (m_undo.isEmpty())
        return;
    //    clearLinkageCache();
    m_redo.append(m_undo.last());
    m_inputEdit->setRedoAction(true);
    m_undo.removeLast();
    m_isUndo = true;
    //20200619 从结果撤销到上一次表达式按下数字键不清空输入框
    if (m_isResult)
        m_isResult = false;
    // 20200319修复选中某一数字按下相同数字无法清除选中内容的问题
    if (!m_undo.isEmpty()) {
        if (m_undo.size() > 1) {
            for (int i = m_undo.size() - 1; i > 0; i--) {
                if (m_undo.at(i) == m_inputEdit->text() && m_undo.at(i - 1) == m_inputEdit->text())
                    m_undo.pop_back();
            }
        }
        m_inputEdit->setText(m_undo.last());
    } else {
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

void SciExpressionBar::addUndo()
{
    // 20200319修复选中某一数字按下相同数字无法清除选中内容的问题
    //    if (!m_undo.isEmpty() && m_inputEdit->text() == m_undo.last())
    //        return;
    m_undo.append(m_inputEdit->text());
    m_redo.clear();
    m_inputEdit->setUndoAction(true);
    SSelection selection;
    m_inputEdit->setSelection(selection);
}

void SciExpressionBar::Redo()
{
    if (m_redo.isEmpty())
        return;
    //    clearLinkageCache();
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

void SciExpressionBar::initTheme(int type)
{
    //edit for bug-21476
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_listDelegate->setThemeType(typeIn);
}

void SciExpressionBar::clearSelection()
{
    SSelection select = m_inputEdit->getSelection();
    if (!select.selected.isEmpty()) {
        int start = select.curpos - select.selected.length();
        QString exp = m_inputEdit->text();
        exp = exp.remove(start, select.selected.length());
        select.clearText = exp;
    }
}

void SciExpressionBar::setSelection()
{
    SSelection select = m_inputEdit->getSelection();
    if (select.selected.isEmpty())
        return;
    if (m_inputEdit->text() == select.clearText)
        m_inputEdit->setText(select.oldText);
    select.selected = "";
}

void SciExpressionBar::getSelection()
{
    if (!m_inputEdit->selectedText().isEmpty()) {
        int start = m_inputEdit->selectionStart();
        QString exp = m_inputEdit->text();
        exp = exp.remove(start, m_inputEdit->selectionLength());
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(start);
    }
}

void SciExpressionBar::setResultFalse()
{
    m_isResult = false;
}

void SciExpressionBar::replaceSelection(QString text)
{
    QString seloldtext = text;
    SSelection selection = m_inputEdit->getSelection();
    int selcurPos = m_inputEdit->cursorPosition();
    if (selection.selected != "") {
        text.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(text);
        if (selcurPos > selection.curpos &&
                selcurPos <= selection.curpos + selection.selected.size())
            selcurPos = selection.curpos;
        // 20200313选中部分光标置位问题修复
        if (seloldtext.mid(0, selcurPos).remove(QRegExp("[,]")).length() ==
                m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[,]")).length())
            m_inputEdit->setCursorPosition(selcurPos);
        else if (seloldtext.mid(0, selcurPos).remove(QRegExp("[,]")).length() >
                 m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[,]")).length())
            m_inputEdit->setCursorPosition(selcurPos + 1);
        else
            m_inputEdit->setCursorPosition(selcurPos - 1);
    }
    // reset selection
    selection = SSelection();
    m_inputEdit->setSelection(selection);
}

bool SciExpressionBar::isOperator(const QString &text)
{
    if (text == QString::fromUtf8("＋") || text == QString::fromUtf8("－") ||
            text == QString::fromUtf8("×") || text == QString::fromUtf8("÷")) {
        return true;
    } else {
        return false;
    }
}

void SciExpressionBar::moveLeft()
{
    QString sRegNum = "[A-Za-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    int funpos = -1;
    int i;
    if (m_inputEdit->cursorPosition() > 0 && rx.exactMatch(m_inputEdit->text().at(m_inputEdit->cursorPosition() - 1))) {
        for (i = 0; i < m_funclist.size(); i++) {
            funpos = m_inputEdit->text().lastIndexOf(m_funclist[i], m_inputEdit->cursorPosition() - 1);
            if (funpos != -1 && funpos + m_funclist[i].length() == m_inputEdit->cursorPosition())
                break;
            else
                funpos = -1;
        }
        if (funpos != -1) {
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - m_funclist[i].length());
        } else {
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - 1);
        }
    } else {
        m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() - 1);
    }
    m_inputEdit->setFocus();
}

void SciExpressionBar::moveRight()
{
    QString sRegNum = "[A-Za-z]";
    QRegExp rx;
    rx.setPattern(sRegNum);
    int funpos = -1;
    int i;
    if (!cursorPosAtEnd() && rx.exactMatch(m_inputEdit->text().at(m_inputEdit->cursorPosition()))) {
        for (i = 0; i < m_funclist.size(); i++) {
            funpos = m_inputEdit->text().indexOf(m_funclist[i], m_inputEdit->cursorPosition());
            if (funpos != -1 && funpos == m_inputEdit->cursorPosition())
                break;
            else
                funpos = -1;
        }
        if (funpos != -1) {
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() + m_funclist[i].length());
        } else {
            m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() + 1);
        }
    } else {
        m_inputEdit->setCursorPosition(m_inputEdit->cursorPosition() + 1);
    }
    m_inputEdit->setFocus();
}
//20200414 bug20294鼠标点击取消focus
InputEdit *SciExpressionBar::getInputEdit()
{
    return m_inputEdit;
}
