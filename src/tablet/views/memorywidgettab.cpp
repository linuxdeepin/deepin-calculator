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

#include "memorywidgettab.h"

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
#include <QDesktopWidget>
#include <DGuiApplicationHelper>

#include "utils.h"
#include "tablet/memorypublictab.h"

const int STANDARD_MWIDGET_HEIGHT = 970; //标准模式memorywidget高度
const int SCIENTIFIC_MWIDGET_HEIGHT = 905; //科学模式memorywidget高度
const int ITEM_WIDTH = 456;
const int STANDARD_FORMAT_PREC = 15; //标准模式科学计数位数
const int SCIENTIFIC_FORMAT_PREC = 31; //科学模式科学计数位数
const int MAXSIZE = 500; //内存保存最大数
const int NOMEMORYHEIGHT = 970; //无内存时item高度

MemoryWidgetTab::MemoryWidgetTab(int mode, QWidget *parent)
    : QWidget(parent)
    , m_listwidget(new MemoryListWidget(this))
    , m_memoryDelegate(new MemoryItemDelegate(this))
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    m_label = new QLabel(this);
    m_calculatormode = mode;
    m_evaluator = Evaluator::instance();
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *lay = new QVBoxLayout(this);

    lay->setSpacing(0);
    lay->setMargin(0);
    lay->addWidget(m_listwidget);

    m_listwidget->setFrameShape(QFrame::NoFrame); //设置边框类型，无边框
    m_mode = mode;
    if (mode == 0 || mode == 2) {
        m_listwidget->setMinimumHeight(STANDARD_MWIDGET_HEIGHT);
        m_precision = STANDARD_FORMAT_PREC;
    } else {
        m_listwidget->setMinimumHeight(SCIENTIFIC_MWIDGET_HEIGHT);
        m_precision = SCIENTIFIC_FORMAT_PREC;
    }
    if (mode < 2)
        m_itemwidth = ITEM_WIDTH;
    else
        m_itemwidth = this->width();
    m_listwidget->setVerticalScrollMode(QListView::ScrollPerPixel); //鼠标滚轮滚动一次一个像素
    m_listwidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); //设置垂直滚条
    m_listwidget->setAutoScroll(false); //鼠标在视口边缘时是否自动滚动内容
    m_listwidget->setSelectionRectVisible(false); //选择矩形框是否可见
    m_listwidget->setItemDelegate(m_memoryDelegate);
    m_label->hide();
    memoryclean();

    lay->addSpacing(15);
    this->setLayout(lay);


    m_label->setText(tr("Nothing saved in memory"));
    m_label->setAlignment(Qt::AlignCenter); //label字体居右，居上
    m_label->setFixedSize(QSize(m_itemwidth, NOMEMORYHEIGHT));
    m_label->setAttribute(Qt::WA_TranslucentBackground); //label窗体透明
    QFont font;
    font.setPixelSize(16);
    m_label->setFont(font);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents, true); //label鼠标穿透

    m_listwidget->installEventFilter(this);
    this->installEventFilter(this);
}

/**
 * @brief MemoryWidget::generateData
 * 向数字内存中增加一个数字，同时向保存Quantity的list中添加数据，一一对应
 * @param answer
 * 将输入的数字转换为quantity类型
 */
void MemoryWidgetTab::generateData(const Quantity answer, bool ismax)
{
    m_label->hide();
    if (ismax)
        m_listwidget->takeItem(MAXSIZE - 1);
    if (m_memorypublic->isEmpty()) {
        m_listwidget->clear();
    }
    QListWidgetItem *item1 = new QListWidgetItem();
    item1->setFlags(Qt::ItemIsEditable);

    QFont font;
    font.setPixelSize(30);
    item1->setFont(font);
    item1->setSizeHint(QSize(m_itemwidth, 80 + 60 * m_line));
    MemoryItemWidgetTab *widget = new MemoryItemWidgetTab(this, m_mode > 1);
    widget->setFixedSize(QSize(m_itemwidth, 80 + 60 * m_line));

    m_listwidget->insertItem(0, item1);
    m_listwidget->setItemWidget(item1, widget);
    if (answer.isZero()) {
        widget->setTextLabel("0");
    } else {
        const QString result = DMath::format(answer, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
        QString formatResult = Utils::formatThousandsSeparators(result);
        formatResult = setitemwordwrap(formatResult);
        widget->setTextLabel(formatResult);
    }
    connect(widget, &MemoryItemWidgetTab::plusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidgetTab::widgetplus(row);
    });
    connect(widget, &MemoryItemWidgetTab::minusbtnclicked, this, [ = ]() {
        int row = m_listwidget->row(item1);
        emit MemoryWidgetTab::widgetminus(row);
    });
    connect(widget, &MemoryItemWidgetTab::cleanbtnclicked, this, [ = ]() {
//        widget->setFocus(); //memorybutton取消focus状态
        emit widgetclean(m_listwidget->row(item1), m_calculatormode, false);
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    });
    widget->themetypechanged(m_themetype);
    connect(this, &MemoryWidgetTab::themechange, widget, &MemoryItemWidgetTab::themetypechanged);
    connect(widget, &MemoryItemWidgetTab::menuclean, this, [ = ]() { //item菜单MC
        emit widgetclean(m_listwidget->row(item1), m_calculatormode, true);
        m_listwidget->takeItem(m_listwidget->row(item1));
        delete item1;
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    });
    connect(widget, &MemoryItemWidgetTab::menucopy, this, [ = ]() { //item菜单复制
        QClipboard *clipboard = QApplication::clipboard();
        MemoryItemWidgetTab *w1 = static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(item1));
        clipboard->setText(w1->textLabel().remove("\n"));
    });
    connect(widget, &MemoryItemWidgetTab::menuplus, this, [ = ]() { //item菜单M+
        int row = m_listwidget->row(item1);
        emit MemoryWidgetTab::widgetplus(row);
    });
    connect(widget, &MemoryItemWidgetTab::menuminus, this, [ = ]() { //item菜单M-
        int row = m_listwidget->row(item1);
        emit MemoryWidgetTab::widgetminus(row);
    });
}

void MemoryWidgetTab::mousePressEvent(QMouseEvent *event)
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
bool MemoryWidgetTab::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_listwidget && event->type() == QEvent::KeyPress) {
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
void MemoryWidgetTab::focusInEvent(QFocusEvent *event)
{
    if (!m_memorypublic->isEmpty() && event->reason() == Qt::TabFocusReason) {
        m_listwidget->setCurrentRow(m_currentrow);
        m_listwidget->setFocus();
    }
    QWidget::focusInEvent(event);
}

void MemoryWidgetTab::keyPressEvent(QKeyEvent *event)
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
void MemoryWidgetTab::memoryAnsChanged(int row, const Quantity answer)
{
    const QString result = DMath::format(answer, Quantity::Format::General() + Quantity::Format::Precision(m_precision));
    QString formatResult = Utils::formatThousandsSeparators(result);
    formatResult = setitemwordwrap(formatResult, row);
    MemoryItemWidget *w1 = static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(row)));
    w1->setTextLabel(formatResult);
}

/**
 * @brief MemoryWidget::memoryclean
 * 清空内存列表，同步清空quantity list
 */
void MemoryWidgetTab::memoryclean()
{
    m_listwidget->setFocusPolicy(Qt::NoFocus); //内存无内容时NoFocus
    m_label->show();
    m_listwidget->clear();
    QListWidgetItem *item1 = new QListWidgetItem(m_listwidget);

    m_listwidget->insertItem(0, item1);
    if (m_calculatormode == 0 || m_calculatormode == 2)
        m_listwidget->item(0)->setSizeHint(QSize(m_itemwidth, m_listwidget->frameRect().height())); //标准模式
    else
        m_listwidget->item(0)->setSizeHint(QSize(m_itemwidth, SCIENTIFIC_MWIDGET_HEIGHT)); //科学模式
    m_listwidget->item(0)->setFlags(Qt::NoItemFlags);
    emptymemoryfontcolor();
}

/**
 * @brief 内存中没有数据的字体颜色
 */
void MemoryWidgetTab::emptymemoryfontcolor()
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
QPair<QString, Quantity> MemoryWidgetTab::getfirstnumber()
{
    QPair<QString, Quantity> p1;
    if (!m_memorypublic->isEmpty()) {
//        QString str = m_listwidget->item(0)->data(Qt::EditRole).toString();
        MemoryItemWidgetTab *w1 = static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(0)));
        QString str = w1->textLabel();
        p1.first = str.remove("\n");
        p1.second = m_memorypublic->getList().at(0);
        return p1;
    } else {
        p1.first = QString();
        p1.second = Quantity(0);
        return p1;
    }
}

MemoryListWidget *MemoryWidgetTab::getMemoryWidget()
{
    return m_listwidget;
}

/**
 * @brief MemoryWidget::expressionempty
 * 当输入栏为空时，内存列表中的加减按钮置灰
 * @param b
 * true = 空，false = 输入栏有内容
 */
void MemoryWidgetTab::expressionempty(bool b)
{
    if (!m_memorypublic->isEmpty()) {
        for (int i = 0; i < m_listwidget->count(); i++) {
            static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(i)))->isexpressionempty(b);
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
void MemoryWidgetTab::widgetcleanslot(int row, int mode, bool ismenu)
{
    if (m_calculatormode != mode) {
        delete m_listwidget->takeItem(row);
        if (m_listwidget->count() == 0) {
            memoryclean();
        }
    } else {
        if (row + 1 < m_listwidget->count() && !ismenu)
            static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(row + 1)))->setNextItemHover();
    }
}

void MemoryWidgetTab::resetWidgetSize(QSize size)
{
    if (m_mode < 2) {
        m_itemwidth = ITEM_WIDTH * size.width() / 1920;
        m_label->setFixedSize(QSize(m_itemwidth, NOMEMORYHEIGHT));
        if (!m_memorypublic->isEmpty()) {
            for (int i = 0; i < m_listwidget->count(); i++) {
                m_listwidget->item(i)->setSizeHint(QSize(m_itemwidth, 80 + 60 * m_line));
                static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(i)))->setFixedSize(QSize(m_itemwidth, 80 + 60 * m_line));
            }
        } else {
            memoryclean();
        }
    } else {
        m_itemwidth = QApplication::desktop()->screenGeometry().width();
        m_label->setFixedSize(QSize(m_itemwidth, NOMEMORYHEIGHT));
        if (!m_memorypublic->isEmpty()) {
            for (int i = 0; i < m_listwidget->count(); i++) {
                m_listwidget->item(i)->setSizeHint(QSize(m_itemwidth, 80 + 60 * m_line));
                static_cast<MemoryItemWidget *>(m_listwidget->itemWidget(m_listwidget->item(i)))->setFixedSize(QSize(m_itemwidth, 80 + 60 * m_line));
            }
        } else {
            memoryclean();
        }
    }
}

void MemoryWidgetTab::setMemoryPublic(MemoryPublicTab *pub)
{
    m_memorypublic = pub;
    initConnect();
}

void MemoryWidgetTab::initConnect()
{
    connect(m_memorypublic, &MemoryPublicTab::generateDataSig, this, &MemoryWidgetTab::generateData);
    connect(m_memorypublic, &MemoryPublicTab::memorycleanSig, this, &MemoryWidgetTab::memoryclean);
    connect(m_memorypublic, &MemoryPublicTab::memoryAnsSig, this, &MemoryWidgetTab::memoryAnsChanged);
    connect(m_memorypublic, &MemoryPublicTab::widgetcleanSig, this, &MemoryWidgetTab::widgetcleanslot);
    connect(m_memorypublic, &MemoryPublicTab::setThemeTypeSig, this, &MemoryWidgetTab::setThemeType);

    connect(m_listwidget, &MemoryListWidget::itemselected, this, [ = ](int row) {
        if (!m_memorypublic->isEmpty()) {
            QPair<QString, Quantity> p;
            MemoryItemWidgetTab *w1 = static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(row)));
            //        p.first = m_listwidget->item(row)->data(Qt::DisplayRole).toString();
            p.first = w1->textLabel();
            p.second = m_memorypublic->getList().at(row);
            if (m_listwidget->item(row)->flags() != Qt::NoItemFlags)
                emit itemclick(p);
        }
    });
    connect(m_listwidget, &MemoryListWidget::space, this, [ = ] {
        if (!m_memorypublic->isEmpty()) //只有listwidget在focus状态才会触发keypress,所以此处未进行hasfocus判断
        {
            QPair<QString, Quantity> p;
            int row = m_listwidget->currentRow();
            MemoryItemWidgetTab *w1 = static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(row)));
            //        p.first = m_listwidget->item(row)->data(Qt::DisplayRole).toString();
            p.first = w1->textLabel();
            p.second = m_memorypublic->getList().at(row);
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
        MemoryItemWidgetTab *w1 = static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(row)));
        w1->showTextEditMenuByAltM();
    });
}

/**
 * @brief 替换text中的中文字符为英文字符
 */
QString MemoryWidgetTab::formatExpression(const QString &text)
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
QString MemoryWidgetTab::setitemwordwrap(const QString &text, int row)
{
    QString result = text;
    result.replace('-', "－").replace('+', "＋");
    int index = result.indexOf("E");
    m_line = 1;
    if (m_calculatormode == 0 || m_calculatormode == 2) {
        if (index > 0 && result.left(index).length() > 13) {
            result.insert(index, "\n");
            m_line = 2;
        } else if (index <= 0 && result.length() > 21) {
            result.insert(20, "\n");
            m_line = 2;
        }
    } else {
        if (index > 15) {
            result.insert(index, "\n");
            m_line = 2;
            if (index > 18 && index < 33) {
                result.remove("\n");
                result.insert(18, "\n");
                m_line = 2;
            } else {
                result.remove("\n");
                result.insert(18, "\n");
                result.insert(index + 1, "\n");
                m_line = 3;
            }
        } else if (index <= 0 && result.length() > 19) {
            result.insert(18, "\n");
            m_line = 2;
        }
    }
    if (m_listwidget->item(row)) {
        //设置item高度
        m_listwidget->item(row)->setSizeHint(QSize(m_itemwidth, 80 + 60 * m_line));
        m_listwidget->itemWidget(m_listwidget->item(row))->setFixedSize(QSize(m_itemwidth, 80 + 60 * m_line));
    }
    static_cast<MemoryItemWidgetTab *>(m_listwidget->itemWidget(m_listwidget->item(row)))->setLineHight(m_line); //设置item中label高度
    return result;
}

/**
 * @brief 根据主题变换垃圾桶及内存中没有数据字体颜色
 */
void MemoryWidgetTab::setThemeType(int type)
{
    m_listwidget->update(); //刷新widget
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    m_themetype = typeIn;
    emit themechange(m_themetype);
    if (m_memorypublic->isEmpty()) {
        emptymemoryfontcolor(); //更改内存中没有数据字体颜色
    }
}

MemoryWidgetTab::~MemoryWidgetTab()
{
}
