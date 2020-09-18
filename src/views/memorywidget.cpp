/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* Maintainer: xiajing <xiajing@uniontech.com>
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

#include "memorywidget.h"

#include <QDebug>
#include <QFont>
#include <QModelIndex>
#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QEvent>
#include <QPen>
#include <QContextMenuEvent>
#include <QMenu>
#include <QClipboard>
#include <DGuiApplicationHelper>

#include "utils.h"

const int GLOBALPREC = 78; //全局精度
const int STANDARD_MWIDGET_HEIGHT = 260; //标准模式memorywidget高度
const int SCIENTIFIC_MWIDGET_HEIGHT = 420; //科学模式memorywidget高度
const int STANDARD_ITEM_WIDTH = 344; //科学模式宽度
const int SCIENTIFIC_ITEM_WIDTH = 360; //标准模式最小宽度
const int STANDARD_FORMAT_PREC = 15; //标准模式科学计数位数
const int SCIENTIFIC_FORMAT_PREC = 31; //科学模式科学计数位数
const int MAXSIZE = 500; //内存保存最大数
const int NOMEMORYHEIGHT = 320; //无内存时item高度

MemoryWidget::MemoryWidget(int mode, QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new MemoryListWidget(this))
    , m_clearbutton(new IconButton(this, 1))
    , m_isempty(true)
    , m_memoryDelegate(new MemoryItemDelegate(this))
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    m_label = new QLabel(this);
    m_calculatormode = mode;
    m_evaluator = Evaluator::instance();
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout *layH = new QHBoxLayout();

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);

    m_listwidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
    mode == 0 ? m_listwidget->setFixedHeight(STANDARD_MWIDGET_HEIGHT) : m_listwidget->setFixedHeight(SCIENTIFIC_MWIDGET_HEIGHT);
    m_itemwidth = (mode == 0) ? STANDARD_ITEM_WIDTH : SCIENTIFIC_ITEM_WIDTH;
    m_precision = (mode == 0) ? STANDARD_FORMAT_PREC : SCIENTIFIC_FORMAT_PREC;
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel); //鼠标滚轮滚动一次一个像素
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); //设置垂直滚条
    m_listwidget->setAutoScroll(false); //鼠标在视口边缘时是否自动滚动内容
    m_listwidget->setSelectionRectVisible(false); //选择矩形框是否可见
    m_listwidget->setItemDelegate(m_memoryDelegate);
    m_label->hide();
    memoryclean();
    lay->addStretch();
    layH->addStretch();

    layH->addWidget(m_clearbutton);
    layH->addSpacing(10);
    if (mode == 1)
        m_clearbutton->hide();
    connect(m_clearbutton, &TextButton::space, this, [ = ]() {
        emit memorycleansignal();
        setFocus();
    }); //focus下空格按下
    connect(m_clearbutton, &DPushButton::clicked, this, [ = ]() {
        emit memorycleansignal();
        setFocus();
    });
    lay->addLayout(layH);
    lay->addSpacing(6);
    this->setLayout(lay);
    connect(m_listwidget, &MemoryListWidget::itemselected, this, [ = ](int row) {
        if (!m_isempty) {
            QPair<QString, Quantity> p;
            MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
            //        p.first = m_listwidget->item(row)->data(Qt::DisplayRole).toString();
            p.first = w1->textLabel();
            p.second = m_list.at(row);
            if (m_listwidget->item(row)->flags() != Qt::NoItemFlags)
                emit itemclick(p);
        }
    });
    connect(m_listwidget, &MemoryListWidget::space, this, [ = ] {
        if (!m_isempty) //只有listwidget在focus状态才会触发keypress,所以此处未进行hasfocus判断
        {
            QPair<QString, Quantity> p;
            int row = m_listwidget->currentRow();
            MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
            //        p.first = m_listwidget->item(row)->data(Qt::DisplayRole).toString();
            p.first = w1->textLabel();
            p.second = m_list.at(row);
            if (m_listwidget->item(row)->flags() != Qt::NoItemFlags)
                emit itemclick(p);
        }
    });
    connect(m_listwidget, &MemoryListWidget::focus, this, [ = ](int direction) {
        switch (direction) { //只有listwidget在focus状态才会触发keypress,所以此处未进行hasfocus判断
        case 0:
            if (m_listwidget->currentRow() > 0) {
                m_listwidget->setCurrentRow(m_listwidget->currentRow() - 1);
                m_currentrow = m_listwidget->currentRow();
                m_listwidget->scrollToItem(m_listwidget->item(m_listwidget->currentRow())); //滚动条跟随焦点项
            }
            break;
        case 1:
            if (m_listwidget->currentRow() < (m_listwidget->count() - 1)) {
                m_listwidget->setCurrentRow(m_listwidget->currentRow() + 1);
                m_currentrow = m_listwidget->currentRow();
                m_listwidget->scrollToItem(m_listwidget->item(m_listwidget->currentRow())); //滚动条跟随焦点项
            }
            break;
        default:
            break;
        }
    });
    connect(m_listwidget, &MemoryListWidget::altAndM, this, [ = ]() {
        int row = m_listwidget->currentRow();
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->showTextEditMenuByAltM();
    });

    m_label->setText(tr("Nothing saved in memory"));
    m_label->setAlignment(Qt::AlignCenter); //label字体居右，居上
    m_label->setFixedSize(QSize(m_itemwidth, NOMEMORYHEIGHT));
    m_label->setAttribute(Qt::WA_TranslucentBackground); //label窗体透明
    QFont font;
    font.setPixelSize(16);
    m_label->setFont(font);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true); //label鼠标穿透

    m_listwidget->installEventFilter(this);
    m_clearbutton->installEventFilter(this);
    this->installEventFilter(this);
    if (m_calculatormode == 0)
        setTabOrder(m_listwidget, m_clearbutton);
}

/**
 * @brief MemoryWidget::generateData
 * 向数字内存中增加一个数字，同时向保存Quantity的list中添加数据，一一对应
 * @param answer
 * 将输入的数字转换为quantity类型
 */
void MemoryWidget::generateData(Quantity answer)
{
//    m_listwidget->setFocusPolicy(Qt::TabFocus); //内存中有内容时TabFocus
    m_label->hide();
    //500 memory number limit
    if (m_list.count() == MAXSIZE) {
        m_list.pop_back();
        m_listwidget->takeItem(MAXSIZE - 1);
    }
    if (m_isempty == true) {
        m_listwidget->clear();
    }
    m_isempty = false;
    emit mListAvailable();
    QListWidgetItem *item1 = new QListWidgetItem();
    item1->setFlags(Qt::ItemIsEditable);
//    item1->setTextAlignment(Qt::AlignRight | Qt::AlignTop);
    QFont font;
    font.setPixelSize(30);
    item1->setFont(font);
    item1->setSizeHint(QSize(m_itemwidth, 40 + 45 * m_line));
    MemoryItemWidget *widget = new MemoryItemWidget(this);
    widget->setFixedSize(QSize(m_itemwidth, 40 + 45 * m_line));

    m_listwidget->insertItem(0, item1);
    m_listwidget->setItemWidget(item1, widget);
    if (answer == Quantity(0)) {
        widget->setTextLabel("0");
    } else {
        const QString result = DMath::format(answer, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        widget->setTextLabel(formatResult);
    }
    m_list.insert(0, answer); //对于新增数据，同步在list中加入对应的Quantity
    connect(widget, &MemoryItemWidget::plusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
//        widget->setFocus(); //memorybutton取消focus状态
        emit MemoryWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::minusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
//        widget->setFocus(); //memorybutton取消focus状态
        emit MemoryWidget::widgetminus(row);
    });
    connect(widget, &MemoryItemWidget::cleanbtnclicked, this, [ = ]() {
//        widget->setFocus(); //memorybutton取消focus状态
        emit widgetclean(m_listwidget->row(item1), m_calculatormode, false);
        m_list.removeAt(m_listwidget->row(item1));
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    });
    widget->themetypechanged(m_themetype);
    connect(this, &MemoryWidget::themechange, widget, &MemoryItemWidget::themetypechanged);
    connect(widget, &MemoryItemWidget::menuclean, this, [ = ]() { //item菜单MC
        emit widgetclean(m_listwidget->row(item1), m_calculatormode, true);
        m_list.removeAt(m_listwidget->row(item1));
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    });
    connect(widget, &MemoryItemWidget::menucopy, this, [ = ]() { //item菜单复制
        QClipboard *clipboard = QApplication::clipboard();
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(item1));
        clipboard->setText(w1->textLabel().remove("\n"));
    });
    connect(widget, &MemoryItemWidget::menuplus, this, [ = ]() { //item菜单M+
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetplus(row);
    });
    connect(widget, &MemoryItemWidget::menuminus, this, [ = ]() { //item菜单M-
        int row = m_listwidget->row(item1);
        emit MemoryWidget::widgetminus(row);
    });
}

void MemoryWidget::mousePressEvent(QMouseEvent *event)
{
    QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
    m_mousepoint = pEvent->pos();

    QRect rect(this->frameGeometry());
    if (rect.contains(m_mousepoint) == true)
        emit insidewidget();
    QWidget::mousePressEvent(event);
}

/**
 * @brief 利用事件过滤器防止打开内存界面时focus到titlebar上
 */
bool MemoryWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (m_calculatormode == 0) {
        if (event->type() == QEvent::FocusOut && !(m_listwidget->hasFocus() || m_clearbutton->hasFocus() || this->hasFocus())) {
            emit basicPressEscape();
        }
        if (obj == this && event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast < QKeyEvent *>(event); //将事件转化为键盘事件
            if (key_event->key() == Qt::Key_Tab) {
                if (!m_isempty) {
                    m_listwidget->setCurrentRow(m_currentrow);
                    m_listwidget->setFocus();
                } else {
                    setFocus();
                }
                return true;
            }
        }
        if (obj == m_listwidget || obj == m_clearbutton) {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent *key_event = static_cast < QKeyEvent *>(event); //将事件转化为键盘事件
                if (key_event->key() == Qt::Key_Tab) {
                    if (m_listwidget->hasFocus()) {
//                        focusNextChild();//焦点移动
                        m_clearbutton->setFocus(Qt::TabFocusReason);
                    } else if (m_clearbutton->hasFocus()) {
//                        focusNextChild();//焦点移动
                        m_listwidget->setFocus(Qt::TabFocusReason);
                    }
                    return true;
                }
                if (key_event->key() == Qt::Key_Escape) {
                    emit basicPressEscape();
                    return true;
                }
            }
        }
    }
    if (m_calculatormode == 1 && obj == m_listwidget && event->type() == QEvent::KeyPress) {
        QKeyEvent *key_Event = static_cast<QKeyEvent *>(event);
        if (key_Event->key() == Qt::Key_Tab) {
            if (m_listwidget->hasFocus()) {
                emit scimemtab();
                m_listwidget->clearFocus();
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief 设置焦点到该widget时转移至listwidget
 */
void MemoryWidget::focusInEvent(QFocusEvent *event)
{
    if (!m_isempty && event->reason() == Qt::TabFocusReason) {
        m_listwidget->setCurrentRow(m_currentrow);
        m_listwidget->setFocus();
    }
    QWidget::focusInEvent(event);
}

void MemoryWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        emit basicPressEscape();
    else
        return;
}

/**
 * @brief MemoryWidget::memoryplus
 * 用于从数字键盘或快捷键的方式，对内存列表中的第一个进行加法运算，memoryminus为减法
 * @param answer
 */
void MemoryWidget::memoryplus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
        //内存中不为空时在第一条内存中加输入框数字；否则内存为空，添加一条内存数据
        QString exp = QString(DMath::format(m_list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        w1->setTextLabel(formatResult);
        m_list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(answer);
    }
}

/**
 * @brief 用于从数字键盘或快捷键的方式，对内存列表中的第一个进行M-
 */
void MemoryWidget::memoryminus(Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (m_isempty == false) {
//        QString exp = QString(m_listwidget->item(0)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(m_list.value(0), Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
//        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        w1->setTextLabel(formatResult);
        m_list.replace(0, ans);
    } else {
        m_listwidget->clear();
        generateData(Quantity(0));
        QString exp = QString("0-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
//        m_listwidget->item(0)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        w1->setTextLabel(formatResult);
        m_list.replace(0, ans);
    }
}

/**
 * @brief MemoryWidget::memoryclean
 * 清空内存列表，同步清空quantity list
 */
void MemoryWidget::memoryclean()
{
    m_listwidget->setFocusPolicy(Qt::NoFocus); //内存无内容时NoFocus
    m_label->show();
    m_listwidget->clear();
    m_list.clear();
    QListWidgetItem *item1 = new QListWidgetItem(m_listwidget);

    m_listwidget->insertItem(0, item1);
    if (m_calculatormode == 0)
        m_listwidget->item(0)->setSizeHint(QSize(m_itemwidth, m_listwidget->frameRect().height())); //标准模式
    else
        m_listwidget->item(0)->setSizeHint(QSize(m_itemwidth, NOMEMORYHEIGHT)); //科学模式
    m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
    emptymemoryfontcolor();
    m_isempty = true;
    m_clearbutton->hide();
    emit mListUnavailable();
}

/**
 * @brief 内存中没有数据的字体颜色
 */
void MemoryWidget::emptymemoryfontcolor()
{
    DPalette pl = this->palette();
    if (m_themetype == 1) {
        QColor color(85, 85, 85);
        color.setAlphaF(0.4);
        pl.setColor(DPalette::WindowText, QColor(color));
    } else {
        QColor color(192, 198, 212);
        color.setAlphaF(0.4);
        pl.setColor(DPalette::WindowText, QColor(color));
    }
    m_label->setPalette(pl);
}

/**
 * @brief MR
 */
QPair<QString, Quantity> MemoryWidget::getfirstnumber()
{
    QPair<QString, Quantity> p1;
    if (m_isempty == false) {
//        QString str = m_listwidget->item(0)->data(Qt::EditRole).toString();
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        QString str = w1->textLabel();
        p1.first = str.remove("\n");
        p1.second = m_list.at(0);
        return p1;
    } else {
        p1.first = QString();
        p1.second = Quantity(0);
        return p1;
    }
}

/**
 * @brief MemoryWidget::widgetplusslot
 * 用于从列表中item里的按钮，对指定行号的内存数据进行增加，widgetminusslot为减法
 * @param row
 * 指定的行号
 * @param answer
 * 输入栏中的数的quantity类型
 */
void MemoryWidget::widgetplusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0)) {
//        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    } else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "+(" + formatResultmem + ")");
        QString exp = QString(DMath::format(m_list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC)) + "+(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult, row);
//        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->setTextLabel(formatResult);
        m_list.replace(row, ans);
    }
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M-
 */
void MemoryWidget::widgetminusslot(int row, Quantity answer)
{
    const QString resultmem = DMath::format(answer, Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC));
    QString formatResultmem = Utils::formatThousandsSeparators(resultmem);
    formatResultmem = formatResultmem.replace('-', "－").replace('+', "＋");
    if (answer == Quantity(0)) {
        //        m_listwidget->item(row)->setData(Qt::DisplayRole, m_listwidget->item(row)->data(Qt::EditRole));
    } else {
//        QString exp = QString(m_listwidget->item(row)->data(Qt::EditRole).toString() + "-(" + formatResultmem + ")");
        QString exp = QString(DMath::format(m_list.value(row), Quantity::Format::Fixed() + Quantity::Format::Precision(GLOBALPREC)) + "-(" + formatResultmem + ")");
        m_evaluator->setExpression(formatExpression(exp));
        Quantity ans = m_evaluator->evalUpdateAns();
        const QString result = DMath::format(ans, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult, row);
//        m_listwidget->item(row)->setData(Qt::DisplayRole, formatResult);
        MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->setTextLabel(formatResult);
        m_list.replace(row, ans);
    }
}

bool MemoryWidget::isWidgetEmpty()
{
    return m_isempty;
}

MemoryListWidget *MemoryWidget::getMemoryWidget()
{
    return m_listwidget;
}

/**
 * @brief MemoryWidget::expressionempty
 * 当输入栏为空时，内存列表中的加减按钮置灰
 * @param b
 * true = 空，false = 输入栏有内容
 */
void MemoryWidget::expressionempty(bool b)
{
    if (!m_isempty) {
        for (int i = 0; i < m_listwidget->count(); i++) {
            static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(i)))->isexpressionempty(b);
        }
    }
}

/**
 * @brief MemoryWidget::widgetcleanslot
 * 内存列表item里的清除按钮，指定行数清除
 * @param row
 * 指定的行数
 * @param mode
 * 用于区分是标准还是科学型的删除，防止同步删除时重复删除
 */
void MemoryWidget::widgetcleanslot(int row, int mode, bool ismenu)
{
    if (m_calculatormode != mode) {
        m_list.removeAt(row);
        delete m_listwidget->takeItem(row);
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    } else {
        if (row + 1 < m_listwidget->count() && !ismenu)
            static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row + 1)))->setNextItemHover();
    }
}

/**
 * @brief 替换text中的中文字符为英文字符
 */
QString MemoryWidget::formatExpression(const QString &text)
{
    return QString(text)
           .replace(QString::fromUtf8("＋"), "+")
           .replace(QString::fromUtf8("－"), "-")
           .replace(QString::fromUtf8("×"), "*")
           .replace(QString::fromUtf8("÷"), "/")
           .replace(QString::fromUtf8(","), "");
}

/**
 * @brief MemoryWidget::setitemwordwrap
 * @param text
 * @param row
 * 分行函数
 * @return
 */
QString MemoryWidget::setitemwordwrap(const QString &text, int row)
{
    QString result = text;
    result.replace('-', "－").replace('+', "＋");
    int index = result.indexOf("E");
    m_line = 1;
    if (m_calculatormode == 0) {
        if (index > 0 && result.left(index).length() > 13) {
            result.insert(index, "\n");
            m_line = 2;
        } else if (index <= 0 && result.length() > 21) {
            result.insert(20, "\n");
            m_line = 2;
        }
        if (m_clearbutton->isHidden() == true) {
            m_clearbutton->show();
            m_clearbutton->updateWhenBtnDisable();
            m_clearbutton->showtooltip(true);
        }
    } else {
        if (index > 15) {
            result.insert(index, "\n");
            m_line = 2;
            if (index > 20) {
                result.remove("\n");
                result.insert(20, "\n");
                m_line = 2;
            }
        } else if (index <= 0 && result.length() > 21) {
            result.insert(20, "\n");
            m_line = 2;
        }
    }
    if (m_listwidget->item(row)) {
        //设置item高度
        m_listwidget->item(row)->setSizeHint(QSize(m_itemwidth, 40 + 45 * m_line));
        m_listwidget->itemWidget(m_listwidget->item(row))->setFixedSize(QSize(m_itemwidth, 40 + 45 * m_line));
    }
    static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)))->setLineHight(m_line); //设置item中label高度
    return result;
}

//void MemoryWidget::nothinginmemory()
//{
//    m_listwidget->clear();
//    list.clear();
//    m_listwidget->addItem(tr("Nothing saved in memory"));
//    QFont m_clearbuttonfont;
//    m_clearbuttonfont.setPixelSize(16);
//    m_listwidget->item(0)->setFont(m_clearbuttonfont);
//    m_listwidget->item(0)->setSizeHint(QSize(itemwidth, m_listwidget->frameRect().height()));
//    m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
//    m_listwidget->item(0)->setTextAlignment(Qt::AlignCenter | Qt::AlignTop);
//    emptymemoryfontcolor();
//    m_isempty = true;
//    m_clearbutton->hide();
//    emit mListUnavailable();
//}

/**
 * @brief 根据主题变换垃圾桶及内存中没有数据字体颜色
 */
void MemoryWidget::setThemeType(int type)
{
    m_listwidget->update(); //刷新widget
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_themetype = typeIn;
    emit themechange(m_themetype);
    QString path;
    if (m_themetype == 1) {
        path = QString(":/assets/images/%1/").arg("light");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
        if (m_isempty) {
            emptymemoryfontcolor(); //更改内存中没有数据字体颜色
        }
    } else {
        path = QString(":/assets/images/%1/").arg("dark");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
        if (m_isempty) {
            emptymemoryfontcolor(); //更改内存中没有数据字体颜色
        }
    }
}

MemoryWidget::~MemoryWidget()
{
}
