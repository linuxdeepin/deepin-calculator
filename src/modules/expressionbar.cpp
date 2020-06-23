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

#include "expressionbar.h"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QTimer>
#include <DGuiApplicationHelper>
#include "../utils.h"

ExpressionBar::ExpressionBar(QWidget *parent)
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
    m_hisRevision = -1;
    m_linkageIndex = -1;
    m_isLinked = false;
    m_isUndo = false;
    m_Selected = -1;
    m_meanexp = true;
    // init inputEdit attributes.
    m_inputEdit->setFixedHeight(55);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);
//    m_inputEdit->setFocus();

    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    DPalette pl = this->palette();
    pl.setColor(DPalette::Light, QColor(0, 0, 0, 0));
    pl.setColor(DPalette::Dark, QColor(0, 0, 0, 0));
    pl.setColor(DPalette::Base, QColor(0, 0, 0, 0));
    this->setPalette(pl);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);

    setFixedHeight(160);
    initConnect();
}

ExpressionBar::~ExpressionBar() {}

void ExpressionBar::setContinue(bool isContinue)
{
    m_isContinue = isContinue;
}

void ExpressionBar::enterNumberEvent(const QString &text)
{
    //    if (m_isLinked)
    //        clearLinkageCache();
    if (m_inputNumber && m_hisRevision == -1) {
        m_inputEdit->clear();
        m_isResult = false;
    }
    //20200615 按等于号后退格结果再输入十位数十位消失问题
    if (m_isResult) {
        m_inputEdit->clear();
        m_isResult = false;
        //        clearLinkageCache();
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

    // 20200401 修改symbolFaultTolerance执行位置
    replaceSelection(m_inputEdit->text());
    m_inputEdit->insert(text);
    int nowcur = m_inputEdit->cursorPosition();
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
    m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
    m_inputEdit->setCursorPosition(nowcur);
    emit clearStateChanged(false);
}

void ExpressionBar::enterSymbolEvent(const QString &text)
{
    QString symbol = text;
    symbol.replace('/', QString::fromUtf8("÷"));
    QString oldText = m_inputEdit->text();
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
    //    clearSelectSymbol();   //fix for bug-14117
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text =
                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm = row - 1;
                his.linkageValue = result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    m_isUndo = false;
    // QString oldText = m_inputEdit->text();
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
            if (exp.mid(0, curPos).remove(QRegExp("[＋－×÷,.%()e]")) ==
                    m_inputEdit->text().mid(0, curPos).remove(QRegExp("[＋－×÷,.%()e]"))) {
                QString sRegNum = "[＋－×÷]";
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

void ExpressionBar::enterPercentEvent()
{
    if (m_inputEdit->text().isEmpty()) {
        m_inputEdit->setText("0");
        return;
    }
    bool hasselect = (m_inputEdit->getSelection().selected != "");
    QString oldText = m_inputEdit->text();
    QString exp = m_inputEdit->text();
    // 20200316百分号选中部分格式替代
    replaceSelection(m_inputEdit->text());
    int curPos = m_inputEdit->cursorPosition();
    if (m_inputEdit->text() == QString()) {
        m_inputEdit->setText("0");
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
        m_inputEdit->insert("%");
        QString newtext = m_inputEdit->text();
        int percentpos = m_inputEdit->text().indexOf('%');
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
                                  percentpos - operatorpos + 1);  //截取%表达式
        } else {
            exptext = newtext.mid(operatorpos + (nooperator == true ? 0 : 1),
                                  percentpos - operatorpos + (nooperator == true ? 1 : 0));
            //截取%表达式
        }
        exptext = m_inputEdit->expressionPercent(exptext);
        qDebug() << "exptext" << exptext;
        QString express = symbolComplement(exptext);
        const QString expression = formatExpression(express);
        m_evaluator->setExpression(expression);
        Quantity ans = m_evaluator->evalUpdateAns();
        if (m_evaluator->error().isEmpty()) {
            if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
                return;
            //edit 20200413 for bug--19653
            const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(15));
            QString formatResult = Utils::formatThousandsSeparators(result);
            formatResult = formatResult.replace(QString::fromUtf8("＋"), "+")
                           .replace(QString::fromUtf8("－"), "-")
                           .replace(QString::fromUtf8("×"), "*")
                           .replace(QString::fromUtf8("÷"), "/");
            QString tStr = exptext.replace(QString::fromUtf8(","), "");
            if (formatResult != tStr) {
                if ((percentpos + 1) < newtext.size() &&
                        (newtext.at(percentpos + 1).isDigit() || newtext.at(percentpos + 1) == '.')) {
                    if (operatorpos > 0 &&
                            (newtext.at(percentpos + 1).isDigit() ||
                             newtext.at(percentpos + 1) == '.') &&
                            (newtext.at(operatorpos - 1).isDigit() ||
                             newtext.at(operatorpos - 1) == '.') &&
                            newtext.at(percentpos - 1) == ')') {
                        m_inputEdit->setText(newtext.mid(0, operatorpos) + QString::fromUtf8("×") +
                                             formatResult + QString::fromUtf8("×") +
                                             newtext.right(newtext.size() - percentpos - 1));
                        m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult, ans,
                                                      operatorpos);
                        m_inputEdit->setCursorPosition(operatorpos + 1 + formatResult.size());
                    } else {
                        m_inputEdit->setText(
                            newtext.mid(0, operatorpos + ((nooperator == true ||
                                                           newtext.at(percentpos - 1) == ')')
                                                          ? 0
                                                          : 1)) +
                            formatResult + QString::fromUtf8("×") +
                            newtext.right(newtext.size() - percentpos - 1));
                        m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult, ans,
                                                      operatorpos);
                        m_inputEdit->setCursorPosition(
                            operatorpos +
                            ((nooperator == true || newtext.at(operatorpos) == '(') ? 0 : 1) +
                            formatResult.size());
                    }
                } else if (operatorpos > 0 &&
                           (newtext.at(operatorpos - 1).isDigit() ||
                            newtext.at(operatorpos - 1) == '.') &&
                           newtext.at(operatorpos) == '(') {
                    if (newtext.at(percentpos - 1) == ')') {
                        m_inputEdit->setText(newtext.mid(0, operatorpos) + QString::fromUtf8("×") +
                                             formatResult +
                                             newtext.right(newtext.size() - percentpos - 1));
                    } else {
                        m_inputEdit->setText(newtext.mid(0, operatorpos + 1) + formatResult +
                                             newtext.right(newtext.size() - percentpos - 1));
                    }
                    m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult, ans,
                                                  operatorpos);
                    m_inputEdit->setCursorPosition(operatorpos + 1 + formatResult.size());
                } else if (nooperator == false) {
                    QString opera = newtext.at(operatorpos);
                    if (newtext.at(operatorpos) == '(' && newtext.at(percentpos - 1) == ')')
                        opera = QString();
                    m_inputEdit->setText(newtext.mid(0, operatorpos) + opera + formatResult +
                                         newtext.right(newtext.size() - percentpos - 1));
                    m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult, ans,
                                                  operatorpos);
                    m_inputEdit->setCursorPosition(operatorpos + opera.size() +
                                                   formatResult.size());
                } else {
                    m_inputEdit->setText(newtext.mid(0, operatorpos) + formatResult +
                                         newtext.right(newtext.size() - percentpos - 1));
                    m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult, ans,
                                                  operatorpos);
                    m_inputEdit->setCursorPosition(operatorpos + formatResult.size());
                }
                //                m_inputEdit->setPercentAnswer(m_inputEdit->text(), formatResult,
                //                ans, operatorpos);
            }
        } else {
            m_inputEdit->setText(oldText);
            m_inputEdit->setCursorPosition(percentpos);
            return;
        }
    }
    // end edit for task-13519
    m_listView->scrollToBottom();
    m_isContinue = true;
    m_isUndo = false;
    m_isResult = false;
}

void ExpressionBar::enterPointEvent()
{
    //    if (m_isLinked)
    //        clearLinkageCache();
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    if (curpos == 0) {
        m_inputEdit->insert("0.");
    } else {
        if (exp.at(curpos - 1) == ".")
            return;
        if (exp.at(curpos - 1) != ")" && exp.at(curpos - 1) != "%") {
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
        }
    }
    exp = pointFaultTolerance(m_inputEdit->text());
    exp = m_inputEdit->symbolFaultTolerance(exp);
    if (exp != m_inputEdit->text())
        m_inputEdit->setText(exp);
    m_isUndo = false;
    m_isResult = false;
}

void ExpressionBar::enterBackspaceEvent()
{
    //    if (m_isResult)
    //        clearLinkageCache();
    // m_inputEdit->backspace();
    SSelection selection = m_inputEdit->getSelection();
    int selcurPos = m_inputEdit->cursorPosition();
    if (selection.selected != "") {
        QString text = m_inputEdit->text();
        QString seloldtext = text;
        text.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(text);
        // 20200401 symbolFaultTolerance
        m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
        if (selcurPos > selection.curpos &&
                selcurPos <= selection.curpos + selection.selected.size())
            selcurPos = selection.curpos;
        // 20200316选中部分光标置位问题修复
        if (seloldtext.mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()e]")).length() ==
                m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()e]")).length())
            m_inputEdit->setCursorPosition(selcurPos);
        else if (seloldtext.mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()e]")).length() >
                 m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()e]")).length())
            m_inputEdit->setCursorPosition(selcurPos + 1);
        else
            m_inputEdit->setCursorPosition(selcurPos - 1);
        // 20200401 选中部分左侧为分隔符按退格的光标处理
        int curpos = m_inputEdit->cursorPosition();
//        if (selection.curpos > 0 && text.at(selection.curpos - 1) == ",") {
//            curpos = curpos - 1;
//            m_inputEdit->setCursorPosition(curpos);
//        }
        // fix for pointfault tolerance 16022
        QTimer::singleShot(5000, this, [ = ] {
            m_inputEdit->setText(pointFaultTolerance(m_inputEdit->text()));
            m_inputEdit->setCursorPosition(curpos);
        });
        // end fix
        return;
    }
    QString text = m_inputEdit->text();
    int cur = m_inputEdit->cursorPosition();
    if (text.size() > 0 && cur > 0 && text[cur - 1] == ",") {
        text.remove(cur - 2, 2);
        m_inputEdit->setText(text);
        // 20200401 symbolFaultTolerance
        m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(m_inputEdit->text()));
        m_inputEdit->setCursorPosition(cur - 2);
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

void ExpressionBar::enterClearEvent()
{
    if (m_isAllClear) {
        m_listModel->clearItems();
        m_listView->reset();
        m_isAllClear = false;
        m_isLinked = false;    //20200619 清空历史记录时将联动参数置为false
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
        //        clearLinkageCache();
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
        QString tmp =
    m_listModel->index(index).data(SimpleListModel::ExpressionRole).toString(); if
    (tmp.indexOf("＝") != -1)
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
        formatResult = formatResult.replace(QString::fromUtf8("＋"),
    "+").replace(QString::fromUtf8("－"), "-") .replace(QString::fromUtf8("×"),
    "*").replace(QString::fromUtf8("÷"), "/") .replace(QString::fromUtf8(","), "");

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
    QString oldtext = m_inputEdit->text();
    if (m_inputEdit->text().isEmpty())
        return;
    if (!m_isLinked)
        clearLinkageCache(m_inputEdit->text(), true);
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
    // 20200403 bug-18971 表达式错误时输数字加等于再重新输入表达式历史记录错误表达式未被替换
    // 20200407 超过16位小数未科学计数
    if (m_evaluator->error().isEmpty() && (oldtext.indexOf(QRegExp("[＋－×÷.,%()e]")) != -1)) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;
        //edit 20200413 for bug--19653
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(15));
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
        if (StrToComp == oldtext)
            return;
        //end edit 20200518 for bug-26628
        // 20200402 需求3.2.1.6当输入的数字中有千位符，点击等号视为前后一致，不计入表达式
        if (formatResult != tStr) {
            m_listModel->updataList(m_inputEdit->text() + "＝" + formatResult, m_hisRevision);
            m_inputEdit->setAnswer(formatResult, ans);
            newResult = formatResult;
        }
        m_isContinue = false;
    } else {
        // 20200403 bug-18971 表达式错误时输数字加等于再重新输入表达式历史记录错误表达式未被替换
        if (!m_evaluator->error().isEmpty()) {
            m_listModel->updataList(m_inputEdit->text() + "＝" + tr("Expression error"),
                                    m_hisRevision);
            m_meanexp = false; // 20200409 输入错误表达式后，修改历史记录,再次输入表达式后高亮未取消
        } else {
            m_meanexp = false;
            return;
        }
        if (m_hisRevision == -1)
            m_hisRevision = m_listModel->rowCount(QModelIndex()) - 1;
        else {
            for (int i = 0; i < m_hisLink.size(); ++i) {
                if (m_hisLink[i].linkageTerm == m_hisRevision) {
                    // m_listDelegate->removeLine(m_hisLink[i].linkageTerm,
                    // m_hisLink[i].linkedItem);
                    m_listDelegate->removeLine(i);
                    m_hisLink.removeAt(i);
                    --i;
                }
            }
        }
    }

    QString selectedresult = QString();
    if (m_isLinked) {
        if (m_hisRevision == -1) {
            m_isLinked = false;
            m_listView->scrollToBottom();
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
                QString text = m_listModel->index(m_hisRevision)
                               .data(SimpleListModel::ExpressionRole)
                               .toString();
                QString linkedExp = text.split("＝").first();
                int length = m_hisLink[i].linkageValue.length();
                if (linkedExp.left(length) != m_hisLink[i].linkageValue ||
                        (!isOperator(linkedExp.at(length)) && (linkedExp.at(length) != "("))) {
                    m_hisLink.remove(i);
                    m_listDelegate->removeHisLink();
                    break;
                }
                if (newResult.isEmpty())
                    break;
                newResult = newResult.replace("+", QString::fromUtf8("＋"))
                            .replace("-", QString::fromUtf8("－"));
                m_hisLink[i].linkageValue = newResult;
                QString newText = newResult + linkedExp.right(linkedExp.length() - length);
                m_inputEdit->setText(newText);
                settingLinkage();
            }
        }
        if (m_Selected != -1) {
            selectedresult = m_listModel->index(m_Selected)
                             .data(SimpleListModel::ExpressionRole)
                             .toString()
                             .split("＝")
                             .last();
            m_inputEdit->setText(selectedresult);
        }
        // 20200403 表达式错误重新输入新表达式输入框无结果
        //        if (m_evaluator->error().isEmpty())
        //            m_inputEdit->clear();
    }
    // 20200403 bug-18971 表达式错误时输数字加等于再重新输入表达式历史记录错误表达式未被替换
    if (m_evaluator->error().isEmpty() && (oldtext.indexOf(QRegExp("[＋－×÷,.%()e]")) != -1))
        m_hisRevision = -1;
    m_listView->scrollToBottom();
    m_isLinked = false;
    m_isResult = true;
    m_isUndo = false;
}

void ExpressionBar::enterBracketsEvent()
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
    //    clearSelectSymbol();   //fix for bug-14117
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text =
                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm = row - 1;
                his.linkageValue = result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString oldText = m_inputEdit->text();
    int currentPos = m_inputEdit->cursorPosition();
    m_inputEdit->insert("()");
    // 20200401 symbolFaultTolerance
    QString formatexp = m_inputEdit->symbolFaultTolerance(m_inputEdit->text());
    if (oldText.mid(0, currentPos).count(",") == m_inputEdit->text().mid(0, currentPos).count(","))
        m_inputEdit->setCursorPosition(currentPos + 1);
    else
        m_inputEdit->setCursorPosition(currentPos);
    if (formatexp == oldText)
        m_inputEdit->setText(formatexp);
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
            } else if (leftLeftParen - leftRightParen + (rightLeftParen - rightrightParen) < 0)
    {
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
    //    clearSelectSymbol();   //fix for bug-14117
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text =
                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm = row - 1;
                his.linkageValue = result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert("(");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    QString formatexp = m_inputEdit->symbolFaultTolerance(m_inputEdit->text());
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
    if (formatexp == exp)
        m_inputEdit->setText(formatexp);
}

void ExpressionBar::enterRightBracketsEvent()
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
    //    clearSelectSymbol();   //fix for bug-14117
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text =
                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm = row - 1;
                his.linkageValue = result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    replaceSelection(m_inputEdit->text());
    QString exp = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    int proNumber = m_inputEdit->text().count(",");
    m_inputEdit->insert(")");
    // 20200401 symbolFaultTolerance
    bool isAtEnd = cursorPosAtEnd();
    QString formatexp = m_inputEdit->symbolFaultTolerance(m_inputEdit->text());
    int newPro = m_inputEdit->text().count(",");
    m_isUndo = false;

    if (!isAtEnd) {
        if (newPro < proNumber && exp.at(curpos) != ",") {
            m_inputEdit->setCursorPosition(curpos);
        } else {
            m_inputEdit->setCursorPosition(curpos + 1);
        }
    }
    if (formatexp == exp)
        m_inputEdit->setText(formatexp);
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

void ExpressionBar::enterExpEvent(int mod)
{
    Q_UNUSED(mod);
    emit turnDeg();
}

void ExpressionBar::copyResultToClipboard()
{
    m_evaluator->unsetVariable(QLatin1String("e"), true);
    if (m_inputEdit->text().isEmpty())
        return;
    // QApplication::clipboard()->setText(m_inputEdit->selectedText());

    const QString expression = formatExpression(m_inputEdit->text());
    m_evaluator->setExpression(expression);
    Quantity ans = m_evaluator->evalUpdateAns();

    if (m_evaluator->error().isEmpty()) {
        if (ans.isNan() && !m_evaluator->isUserFunctionAssign())
            return;

        //edit 20200413 for bug--19653
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(15));
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
    //    clearSelectSymbol();   //fix for bug-14117
    if (m_isUndo) {
        int row = m_listModel->rowCount(QModelIndex());
        if (row != 0) {
            QString text =
                m_listModel->index(row - 1).data(SimpleListModel::ExpressionRole).toString();
            QString result = text.split("＝").last();
            if (result == m_inputEdit->text()) {
                historicalLinkageIndex his;
                his.linkageTerm = row - 1;
                his.linkageValue = result;
                his.linkedItem = row;
                m_hisLink.append(his);
                m_listDelegate->setHisLink(row - 1);
                m_listDelegate->setHisLinked(row);
            }
        }
    }
    m_isResult = false;
    QString oldText = m_inputEdit->text();
    int curpos = m_inputEdit->cursorPosition();
    replaceSelection(oldText);
    QString exp = m_inputEdit->text();
    QString text = QApplication::clipboard()->text();
    if (text.count("。") != 0 && text.count(".") != 0)
        text.replace(QString::fromUtf8("。"), ".");
    text.remove("e");
    text = pasteFaultTolerance(text);
    // m_inputEdit->insert(text);
//    m_inputEdit->setText(text);
    //    clearLinkageCache();
    //edit for bug--23649 20200429
    text.remove(QRegExp("[^0-9＋－×÷,.%()e]"));
    m_inputEdit->setText(m_inputEdit->symbolFaultTolerance(text));
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
    SSelection selection;
    selection.selected = m_inputEdit->text();
    m_inputEdit->setSelection(selection);
}

void ExpressionBar::shear()
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

void ExpressionBar::handleTextChanged(const QString &text)
{
    Q_UNUSED(text);
    m_isAllClear = false;
    m_isContinue = true;
}

bool ExpressionBar::cursorPosAtEnd()
{
    return m_inputEdit->cursorPosition() == m_inputEdit->text().length();
}

QString ExpressionBar::formatExpression(const QString &text)
{
    return QString(text)
           .replace(QString::fromUtf8("＋"), "+")
           .replace(QString::fromUtf8("－"), "-")
           .replace(QString::fromUtf8("×"), "*")
           .replace(QString::fromUtf8("÷"), "/")
           .replace(QString::fromUtf8(","), "");
}

void ExpressionBar::revisionResults(const QModelIndex &index)
{
    clearLinkageCache(m_inputEdit->text(), false);
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString("＝"), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = historic.at(0);
    m_hisRevision = index.row();
    m_inputEdit->setText(expression);
    m_Selected = m_hisRevision;
    m_isResult = false;
    // fix addundo for history revision
    m_isUndo = false;
    addUndo();

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

        const QString tResult = DMath::format(ans, Quantity::Format::Fixed() + Quantity::Format::Precision(15));
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

QString ExpressionBar::completedBracketsCalculation(QString &text)
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

void ExpressionBar::historicalLinkage(int index, QString newValue)
{
    for (int i = 0; i < m_hisLink.size(); i++) {
        if (m_hisLink[i].linkageTerm == index && m_hisLink[i].isLink) {
            if (m_hisLink[i].linkedItem == -1)
                return;
            QString text = m_listModel->index(m_hisLink[i].linkedItem)
                           .data(SimpleListModel::ExpressionRole)
                           .toString();
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

                const QString tResult = DMath::format(ans, Quantity::Format::Fixed() + Quantity::Format::Precision(15));
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
            m_hisLink[i].linkageValue = newValue;
            m_listModel->updataList(expression + "＝" + result, m_hisLink[i].linkedItem);
            historicalLinkage(m_hisLink[i].linkedItem, result);
            m_isLinked = false;
        }
    }
}

void ExpressionBar::setLinkState(const QModelIndex index)
{
    int row = index.row();
    for (int i = 0; i < m_hisLink.size(); i++) {
        if (m_hisLink[i].linkageTerm == row)
            m_hisLink[i].isLink = true;
    }
}

// edit 20200318 for fix cleanlinkcache
void ExpressionBar::clearLinkageCache(const QString &text, bool isequal)
{
    if (m_hisLink.isEmpty())
        return;
    QString linkedExp = text.split("＝").first();
    if (m_hisLink.count() > 0 && isequal == true) {
        int length = m_hisLink.last().linkageValue.length();
        if (linkedExp.left(length) != m_hisLink.last().linkageValue ||
                (linkedExp.length() > length && !isOperator(linkedExp.at(length)) &&
                 (linkedExp.at(length) != "("))) {
            m_hisLink.removeLast();
            m_isLinked = false;
            m_listDelegate->removeHisLink();
            m_listDelegate->removeHisLinked();
            //            m_isResult = false;
        }
    } else {
        if (m_hisLink.last().linkedItem == -1) {
            m_hisLink.removeLast();
            m_isLinked = false;
            m_listDelegate->removeHisLink();
//            m_listDelegate->removeHisLinked();
            m_isResult = false;
        }
    }
}

void ExpressionBar::settingLinkage()
{
    m_evaluator->unsetVariable(QLatin1String("e"), true);
    const int hisRecision = m_hisRevision;
    if (hisRecision != -1) {
        // edit 20200403 删除错误表达式
        if (m_Selected != m_hisRevision && m_hisRevision < m_hisLink.count() &&
                m_hisLink[m_hisRevision].linkageValue.count(tr("Expression error")) > 0) {
            m_hisLink.removeAt(m_hisRevision);
            m_listModel->deleteItem(m_hisRevision);
        }
        for (int i = 0; i < m_hisLink.size(); i++) {
            if (m_hisLink[i].linkedItem == hisRecision) {
                if (cancelLink(i + 1) && i != 0)
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
        // hisIndex.isLink = true;
        enterEqualEvent();
        QString exp =
            m_listModel->index(hisRecision).data(SimpleListModel::ExpressionRole).toString();
        hisIndex.linkageValue = exp.split("＝").last();
        m_hisLink.push_back(hisIndex);
    } else {
        // 20200402 需求3.2.1.6当输入的数字中有千位符，点击等号视为前后一致，不计入表达式
        //        if (!m_hisLink.isEmpty() && m_hisLink.last().linkedItem == -1)
        //            return;
        // judgeLinkageAgain();
        enterEqualEvent();
        m_isLinked = true;
        historicalLinkageIndex hisIndex;
        // hisIndex.isLink = true;
        hisIndex.linkageTerm = m_listModel->rowCount(QModelIndex()) - 1;
        QString exp = m_listModel->index(hisIndex.linkageTerm)
                      .data(SimpleListModel::ExpressionRole)
                      .toString();
        hisIndex.linkageValue = exp.split("＝").last();
        m_hisLink.push_back(hisIndex);
    }
    if (m_meanexp == false) {
        m_hisLink.pop_back();
        m_meanexp = true;
    } else {
        m_listDelegate->setHisLink(m_hisLink.last().linkageTerm);
    }
}

void ExpressionBar::judgeLinkageAgain()
{
    if (m_hisLink.isEmpty())
        return;
    QString text = m_inputEdit->text();
    text.replace(",", "");
    QStringList list = text.split(QRegExp("[＋－×÷()]"));
    QString linkValue = m_hisLink.last().linkageValue;
    if (list.at(0) != linkValue) {
        // m_listDelegate->removeLine(m_hisLink.last().linkageTerm,m_hisLink.last().linkedItem);
        m_listDelegate->removeLine(m_hisLink.size() - 1);
        m_hisLink.removeLast();
    }
}

void ExpressionBar::initConnect()
{
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, this,
            &ExpressionBar::revisionResults);
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
            if (exp[i] == "0" && exp[i + 1] != "." && (i == 0 || exp[i - 1] != ".") &&
                    (i == 0 || !exp[i - 1].isNumber()) && (exp.size() == 1 || exp[i + 1].isNumber())) {
                exp.remove(i, 1);
                --i;
            }
            ++i;
        }
        if (exp[i] == "." && (i == 0 || !exp[i - 1].isNumber())) {
            exp.insert(i, "0");
            ++i;
        }
    }
    return exp;
}

QString ExpressionBar::pointFaultTolerance(const QString &text)
{
    QString oldText = text;
    // QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
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
            // oldText.replace(list[i], item);
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
    if (select.selected.size() == 1 && (select.selected == "＋" || select.selected == "－" ||
                                        select.selected == "×" || select.selected == "÷")) {
        select.selected = "";
        QString exp = m_inputEdit->text();
        exp.remove(select.curpos, 1);
        m_inputEdit->setText(exp);
        m_inputEdit->setCursorPosition(select.curpos);
    }
}

void ExpressionBar::expressionCheck()
{
    QString exp = m_inputEdit->text();
    int cur = m_inputEdit->cursorPosition();
    int n = m_inputEdit->cursorPosition();
    int length = exp.length() - n;//原表达式光标右长度
    //光标前的分隔符
    int separator = 0;
    int sum = 0;

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
        int num = 0;
        int a = 0;
        while (exp[i].isNumber()) {
            // fix for delete 0 behind "."
            if (exp[i] == "0" && exp[i + 1] != "." && (i == 0 || exp[i - 1] != ".") &&
                    (i == 0 || !exp[i - 1].isNumber()) && (exp.size() == 1 || exp[i + 1].isNumber())) {
                exp.remove(i, 1);
                --i;
                if (i + 1 < cur)
                    --cur;
                else {
                    a++;//光标后每个数字段被去掉0的个数
                }
            }
            ++i;
            if (i >= cur)
                num++;//光标后每个数字段长度
        }
        sum = sum + ((num - 1) / 3 - (num - a - 1) / 3) + a;
        if (exp[i] == "." && (i == 0 || !exp[i - 1].isNumber())) {
            exp.insert(i, "0");
            ++i;
            if (i < cur)
                ++cur;
        }
    }
    m_inputEdit->setText(exp);
//    m_inputEdit->setCursorPosition(cur + separator);
    m_inputEdit->setCursorPosition(m_inputEdit->text().length() - (length - sum)); //20200525数字前有0时容错删除0光标错位
}

void ExpressionBar::Undo()
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

void ExpressionBar::addUndo()
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

void ExpressionBar::Redo()
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

void ExpressionBar::initTheme(int type)
{
    //edit for bug-21476
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_listDelegate->setThemeType(typeIn);
}

void ExpressionBar::clearSelection()
{
    SSelection select = m_inputEdit->getSelection();
    if (!select.selected.isEmpty()) {
        int start = select.curpos - select.selected.length();
        QString exp = m_inputEdit->text();
        exp = exp.remove(start, select.selected.length());
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
        exp = exp.remove(start, m_inputEdit->selectionLength());
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
            QString linkvalue = m_hisLink[i].linkageValue;
            linkvalue = linkvalue.replace(",", "");
            //            if (list.at(0) != m_hisLink[i].linkageValue) {
            if (list.at(0) != linkvalue) {
                // m_listDelegate->removeLine(m_hisLink[i].linkageTerm,
                // m_hisLink[i].linkedItem);
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
    for (int i = 0; i < m_hisLink.size(); i++) {
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
//20200414 bug20294鼠标点击取消focus
InputEdit *ExpressionBar::getInputEdit()
{
    return m_inputEdit;
}
