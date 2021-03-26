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

#include "scihistorywidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>

#include "dthememanager.h"
#include "../utils.h"
#include "../../3rdparty/math/quantity.h"

const int WIDGET_WIDTH = 360; //历史记录侧宽度
const int BUTTONBOX_WIDTH = 200; //buttonbox宽度
const int BUTTONBOX_HEIGHT = 36; //buttonbox高度
const int LEFT_SPACE = 80; //buttonbox左侧宽度
const int MID_SPACE = 37; //buttonbox与垃圾桶间宽度
const int RIGHT_SPACE = 7; //垃圾桶右侧宽度
const QSize CLEARBUTTON_SIZE = QSize(36, 36); //垃圾桶大小
const int SPACE_BETWEEN_BUTTONANDVIEW = 15; //button与下侧widget之间空隙

SciHistoryWidget::SciHistoryWidget(QWidget *parent)
    : DWidget(parent)
    , m_listView(new SimpleListView(1, this))
    , m_listDelegate(new SimpleListDelegate(1, this))
    , m_listModel(new SimpleListModel(1, this))
    , m_buttonbox(new DButtonBox(this))
    , m_historybtn(new DButtonBoxButton(tr("History"), this))
    , m_memorybtn(new DButtonBoxButton(tr("Memory"), this))
    , m_clearbutton(new IconButton(this, 1))
{
    memoryPublic = MemoryPublic::instance();
    m_memorywidget = memoryPublic->getwidget(MemoryPublic::scientificright);
    m_memorywidget->setFocusPolicy(Qt::TabFocus);
    m_stacklayout = new QStackedLayout();
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    QHBoxLayout *m_Hlayout = new QHBoxLayout();
    QWidget *clearwidget = new QWidget(this);
    m_clearbutton->setParent(clearwidget);
    m_clearbutton->setObjectName("clearbtn");
    m_Hlayout->addSpacing(LEFT_SPACE);
    m_Hlayout->addWidget(m_buttonbox);
    m_buttonbox->setFixedWidth(BUTTONBOX_WIDTH);
    m_Hlayout->addSpacing(MID_SPACE);

    //button stack
    m_Hlayout->addWidget(clearwidget);
    clearwidget->setFixedSize(CLEARBUTTON_SIZE);
    m_Hlayout->addSpacing(RIGHT_SPACE);
    m_clearbutton->showtooltip(false); //设置历史垃圾桶tooltip
    m_clearbutton->setHidden(true);
//    m_clearbutton->setHidden(!(m_isshowH & m_indexH));
    m_isshowM = !memoryPublic->isEmpty();
//    m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));

    m_Vlayout->addSpacing(5);
    m_Vlayout->addLayout(m_Hlayout);
    m_Vlayout->addSpacing(SPACE_BETWEEN_BUTTONANDVIEW);

    //mem & his stack
    m_Vlayout->addLayout(m_stacklayout);
    m_stacklayout->addWidget(m_listView);
    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    m_stacklayout->addWidget(m_memorywidget);
    m_stacklayout->setCurrentIndex(0);

    //    m_Vlayout->setSpacing(0);
    m_Vlayout->setMargin(0);
    m_Vlayout->setContentsMargins(0, 0, 0, 11);
    this->setLayout(m_Vlayout);
    this->setFixedWidth(WIDGET_WIDTH);

    QList<DButtonBoxButton *> listBtnBox;
    m_historybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    m_historybtn->setFocusPolicy(Qt::TabFocus);
    m_memorybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    listBtnBox << m_historybtn << m_memorybtn;
    m_memorybtn->setFocusPolicy(Qt::TabFocus);
    m_historybtn->installEventFilter(this);
    m_memorybtn->installEventFilter(this);
    m_buttonbox->setFocusPolicy(Qt::NoFocus);
    m_buttonbox->setButtonList(listBtnBox, true);
    m_buttonbox->setId(m_historybtn, 0);
    m_buttonbox->setId(m_memorybtn, 1);
    connect(m_buttonbox->button(0), &QAbstractButton::clicked, this, [ = ]() {
        if (!m_buttonbox->button(0)->hasFocus())
            emit hisbtnClicked();
        m_clearbutton->showtooltip(false); //设置历史垃圾桶tooltip
//        m_indexH = true;
//        m_indexM = false;
        m_stacklayout->setCurrentIndex(0);
        m_clearbutton->setHidden(!m_isshowH);
//        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
    });
    connect(m_buttonbox->button(1), &QAbstractButton::clicked, this, [ = ]() {
        if (!m_buttonbox->button(1)->hasFocus())
            emit hisbtnClicked();
        m_clearbutton->showtooltip(true); //设置内存垃圾桶tooltip
//        m_indexH = false;
//        m_indexM = true;
        m_stacklayout->setCurrentIndex(1);
        m_clearbutton->setHidden(!m_isshowM);
//        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    connect(m_clearbutton, &IconButton::clicked, this, [ = ]() {
        if (!m_clearbutton->hasFocus())
            emit hisbtnClicked();
        if (m_buttonbox->checkedId() == 0) {
            m_listModel->clearItems();
            m_listView->listItemFill(false);
            m_isshowH = false;
            m_listView->setFocusPolicy(Qt::NoFocus);
        } else {
            memoryPublic->memoryclean();
            m_isshowM = false;
            m_memorywidget->setFocusPolicy(Qt::NoFocus);
        }
        m_clearbutton->setHidden(true);
        m_clearbutton->updateWhenBtnDisable();
//        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
//        setFocus();
    });
    connect(m_clearbutton, &TextButton::space, this, [ = ]() { //清除焦点空格事件
        if (!m_clearbutton->hasFocus())
            emit hisbtnClicked();
        if (m_buttonbox->checkedId() == 0) {
            m_listModel->clearItems();
            m_listView->listItemFill(false);
            m_isshowH = false;
            m_listView->setFocusPolicy(Qt::NoFocus);
        } else {
            memoryPublic->memoryclean();
            m_isshowM = false;
            m_memorywidget->setFocusPolicy(Qt::NoFocus);
        }
        m_clearbutton->setHidden(true);
        m_clearbutton->updateWhenBtnDisable();
//        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
//        setFocus();
    }); //focus下空格按下
    connect(m_listModel, &SimpleListModel::hisbtnhidden, this, [ = ]() {
        m_listModel->clearItems(); //历史记录无数据信号接收
        m_listView->setFocusPolicy(Qt::NoFocus);
        m_listView->listItemFill(false);
        m_isshowH = false;
        if (m_buttonbox->checkedId() == 0)
            m_clearbutton->setHidden(true);
//        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
//        setFocus();
    });
    connect(memoryPublic, &MemoryPublic::generateDataSig, this, [ = ]() {
        m_isshowM = true; //公共内存中有数据信号接收
        m_memorywidget->setFocusPolicy(Qt::TabFocus);
        if (m_buttonbox->checkedId() == 1)
            m_clearbutton->setHidden(false);
//        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    connect(memoryPublic, &MemoryPublic::memorycleanSig, this, [ = ]() {
        m_isshowM = false; //公共内存中无数据信号接收
        m_memorywidget->setFocusPolicy(Qt::NoFocus);
        if (m_buttonbox->checkedId() == 1)
            m_clearbutton->setHidden(true);
//        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    //防止foucus到输入栏或者buttonbox
//    connect(memoryPublic, &MemoryPublic::publicwidgetclean, this, [ = ](int mode) {
//        if (mode == 1)
//            this->setFocus();
//    });
}

SciHistoryWidget::~SciHistoryWidget() {}

/**
 * @brief 现实历史记录侧时调用，设置buttonbox状态
 */
void SciHistoryWidget::focusOnButtonbox()
{
    if (m_stacklayout->currentIndex() == 0) {
        m_buttonbox->button(0)->setChecked(true);
    } else {
        m_buttonbox->button(1)->setChecked(true);
    }
}

/**
 * @brief 对公共内存进行操作
 */
void SciHistoryWidget::memoryFunctions(SciHistoryWidget::memOperate operate, Quantity answer, int row)
{
    switch (operate) {
    case memoryplus:
        memoryPublic->memoryplus(answer);
        break;
    case memoryminus:
        memoryPublic->memoryminus(answer);
        break;
    case memoryclean:
        memoryPublic->memoryclean();
        break;
    case widgetplus:
        memoryPublic->widgetplus(row, answer);
        break;
    case widgetminus:
        memoryPublic->widgetminus(row, answer);
        break;
    default:
        memoryPublic->generateData(answer);
        break;
    }
}

void SciHistoryWidget::resetFocus()
{
    m_isshowH ? m_listView->setFocusPolicy(Qt::TabFocus) : m_listView->setFocusPolicy(Qt::NoFocus);
    m_isshowM ? m_memorywidget->setFocusPolicy(Qt::TabFocus) : m_memorywidget->setFocusPolicy(Qt::NoFocus);
}

void SciHistoryWidget::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void SciHistoryWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left && (m_historybtn->hasFocus() ||  m_memorybtn->hasFocus())) {
        focusNextChild();//焦点移动
        m_historybtn->setFocus();
    } else if (e->key() == Qt::Key_Right && (m_historybtn->hasFocus() ||  m_memorybtn->hasFocus())) {
        m_iskeypressfocus = true;
        focusNextChild();//焦点移动
        m_memorybtn->setFocus();
        m_iskeypressfocus = false;
    }
}

bool SciHistoryWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_memorybtn && event->type() == QEvent::FocusIn) {
        QFocusEvent *focus_Event = static_cast<QFocusEvent *>(event);
        if (focus_Event->reason() == Qt::TabFocusReason && !m_iskeypressfocus) {
            m_historybtn->setFocus();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief 确保第一次焦点在历史记录按钮
 */
//void SciHistoryWidget::focusInEvent(QFocusEvent *event)
//{
//    m_historybtn->setFocus();
//    QWidget::focusInEvent(event);
//}

/**
 * @brief 根据主题变换更换垃圾桶切图
 */
void SciHistoryWidget::themeChanged(int type)
{
    QString path;
    int typeIn = type;
    if (typeIn == 0) {
        typeIn = DGuiApplicationHelper::instance()->themeType();
    }
    if (typeIn == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
    memoryPublic->setThemeType(typeIn);
    m_listDelegate->setThemeType(typeIn);
}

/**
 * @brief 历史记录中有内容tab
 */
void SciHistoryWidget::historyfilled()
{
    if (m_isshowH == false)
        m_listModel->deleteItem(1);
    m_listView->listItemFill(true);
    m_listView->setFocusPolicy(Qt::TabFocus);
    m_isshowH = true;
    if (m_buttonbox->checkedId() <= 0)
        m_clearbutton->setHidden(false);
//    m_clearbutton->setHidden(!(m_isshowH & m_indexH));
}
