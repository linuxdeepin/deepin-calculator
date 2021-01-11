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

#include "scihistorywidgettab.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>

#include "dthememanager.h"
#include "utils.h"
#include "math/quantity.h"

const int WIDGET_WIDTH = 456; //历史记录侧宽度
const int BUTTONBOX_WIDTH = 96; //buttonbox宽度
const int BUTTONBOX_HEIGHT = 40; //buttonbox高度
const int LEFT_SPACE = 180; //buttonbox左侧宽度
const int RIGHT_SPACE = 180; //垃圾桶右侧宽度
const int SPACE_BETWEEN_BUTTONANDVIEW = 20; //button与下侧widget之间空隙

SciHistoryWidgetTab::SciHistoryWidgetTab(int mode, QWidget *parent)
    : DWidget(parent)
{
    memoryPublic = MemoryPublicTab::instance(this);
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    this->setFixedWidth(WIDGET_WIDTH);
    m_mode = mode;

    if (mode == 0) {
        m_memorywidget = memoryPublic->getwidget(MemoryPublicTab::standardleft);
        m_memorywidget->setFocusPolicy(Qt::TabFocus);
        m_Vlayout->addWidget(m_memorywidget);
        m_isshowM = !memoryPublic->isWidgetEmpty(0);

        connect(memoryPublic, &MemoryPublicTab::filledMem, this, [ = ]() {
            m_isshowM = true; //公共内存中有数据信号接收
            emit clearbtnShow(m_isshowM);
            m_memorywidget->setFocusPolicy(Qt::TabFocus);
        });
        connect(memoryPublic, &MemoryPublicTab::emptyMem, this, [ = ]() {
            m_isshowM = false; //公共内存中无数据信号接收
            emit clearbtnShow(m_isshowM);
            m_memorywidget->setFocusPolicy(Qt::NoFocus);
        });
    } else {
        m_buttonbox = new DButtonBox(this);
        m_historybtn = new DButtonBoxButton(QIcon(), {}, this);
        m_memorybtn = new DButtonBoxButton(QIcon(), {}, this);
        m_listView = new SimpleListViewTab(1, this);
        m_listDelegate = new SimpleListDelegateTab(1, this);
        m_listModel = new SimpleListModel(1, this);
        m_memorywidget = memoryPublic->getwidget(MemoryPublicTab::scientificright);
        m_memorywidget->setFocusPolicy(Qt::TabFocus);
        m_stacklayout = new QStackedWidget(this);
        QHBoxLayout *m_Hlayout = new QHBoxLayout();
        m_Hlayout->addSpacing(LEFT_SPACE);
        m_Hlayout->addWidget(m_buttonbox);
        m_buttonbox->setFixedWidth(BUTTONBOX_WIDTH);

        //button stack
        m_Hlayout->addSpacing(RIGHT_SPACE);

        m_isshowM = !memoryPublic->isWidgetEmpty(1);

        m_Vlayout->addSpacing(15);
        m_Vlayout->addLayout(m_Hlayout);
        m_Vlayout->addSpacing(SPACE_BETWEEN_BUTTONANDVIEW);

        //mem & his stack
        m_Vlayout->addWidget(m_stacklayout);
        m_stacklayout->addWidget(m_listView);
        m_listView->setModel(m_listModel);
        m_listView->setItemDelegate(m_listDelegate);
        m_stacklayout->addWidget(m_memorywidget);
        m_stacklayout->setCurrentIndex(0);

        //    m_Vlayout->setSpacing(0);

        QList<DButtonBoxButton *> listBtnBox;
        m_historybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
        m_historybtn->setIconSize(QSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT));
        m_historybtn->setFocusPolicy(Qt::TabFocus);
        m_memorybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
        m_memorybtn->setIconSize(QSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT));

        listBtnBox << m_historybtn << m_memorybtn;
        m_memorybtn->setFocusPolicy(Qt::TabFocus);
        m_historybtn->installEventFilter(this);
        m_memorybtn->installEventFilter(this);
        m_buttonbox->setFocusPolicy(Qt::NoFocus);
        m_buttonbox->setButtonList(listBtnBox, true);
        m_buttonbox->setId(m_historybtn, 0);
        m_buttonbox->setId(m_memorybtn, 1);
        m_buttonbox->button(0)->setChecked(true);
        iconChanged(m_themeType, 0);

        connect(m_buttonbox->button(0), &QAbstractButton::clicked, this, [ = ]() {
            if (!m_buttonbox->button(0)->hasFocus())
                emit hisbtnClicked();
            m_stacklayout->setCurrentIndex(0);
            iconChanged(m_themeType, 0);
            emit clearbtnShow(m_isshowH);
        });
        connect(m_buttonbox->button(1), &QAbstractButton::clicked, this, [ = ]() {
            if (!m_buttonbox->button(1)->hasFocus())
                emit hisbtnClicked();
            m_stacklayout->setCurrentIndex(1);
            iconChanged(m_themeType, 1);
            emit clearbtnShow(m_isshowM);
        });

        connect(m_listModel, &SimpleListModel::hisbtnhidden, this, [ = ]() {
            m_listModel->clearItems(); //历史记录无数据信号接收
            m_listView->setFocusPolicy(Qt::NoFocus);
            m_listView->listItemFill(false);
            m_isshowH = false;
            if (m_buttonbox->checkedId() == 0)
                emit clearbtnShow(m_isshowH);
        });
        connect(memoryPublic, &MemoryPublicTab::filledMem, this, [ = ]() {
            m_isshowM = true; //公共内存中有数据信号接收
            if (m_buttonbox->checkedId() == 1)
                emit clearbtnShow(m_isshowM);
            m_memorywidget->setFocusPolicy(Qt::TabFocus);
        });
        connect(memoryPublic, &MemoryPublicTab::emptyMem, this, [ = ]() {
            m_isshowM = false; //公共内存中无数据信号接收
            if (m_buttonbox->checkedId() == 1)
                emit clearbtnShow(m_isshowM);
            m_memorywidget->setFocusPolicy(Qt::NoFocus);
        });
    }
    m_Vlayout->setMargin(0);
    m_Vlayout->setContentsMargins(0, 0, 0, 20);
    this->setLayout(m_Vlayout);
}

SciHistoryWidgetTab::~SciHistoryWidgetTab() {}

/**
 * @brief 现实历史记录侧时调用，设置buttonbox状态
 */
void SciHistoryWidgetTab::focusOnButtonbox()
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
void SciHistoryWidgetTab::memoryFunctions(SciHistoryWidgetTab::memOperate operate, Quantity answer, int row)
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

void SciHistoryWidgetTab::resetFocus()
{
    if (m_mode == 1) {
        m_isshowH ? m_listView->setFocusPolicy(Qt::TabFocus) : m_listView->setFocusPolicy(Qt::NoFocus);
        m_isshowM ? m_memorywidget->setFocusPolicy(Qt::TabFocus) : m_memorywidget->setFocusPolicy(Qt::NoFocus);
        if (m_buttonbox->checkedId() == 0) {
            emit clearbtnShow(m_isshowH);
        } else {
            emit clearbtnShow(m_isshowM);
        }
    } else {
        m_isshowM ? m_memorywidget->setFocusPolicy(Qt::TabFocus) : m_memorywidget->setFocusPolicy(Qt::NoFocus);
    }
}

void SciHistoryWidgetTab::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void SciHistoryWidgetTab::keyPressEvent(QKeyEvent *e)
{
    if (m_mode == 1) {
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
}

bool SciHistoryWidgetTab::eventFilter(QObject *obj, QEvent *event)
{
    if (m_mode == 1) {
        if (obj == m_memorybtn && event->type() == QEvent::FocusIn) {
            QFocusEvent *focus_Event = static_cast<QFocusEvent *>(event);
            if (focus_Event->reason() == Qt::TabFocusReason && !m_iskeypressfocus) {
                m_historybtn->setFocus();
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief 确保第一次焦点在历史记录按钮
 */
//void SciHistoryWidgetTab::focusInEvent(QFocusEvent *event)
//{
//    m_historybtn->setFocus();
//    QWidget::focusInEvent(event);
//}

/**
 * @brief 根据主题变换更换垃圾桶切图
 */
void SciHistoryWidgetTab::themeChanged(int type)
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

    memoryPublic->setThemeType(typeIn);
    m_listDelegate->setThemeType(typeIn);
    m_themeType = typeIn;
    if (m_mode == 1)
        iconChanged(m_themeType, m_buttonbox->checkedId());
}

/**
 * @brief 历史记录中有内容tab
 */
void SciHistoryWidgetTab::historyfilled()
{
    if (m_isshowH == false)
        m_listModel->deleteItem(1);
    m_listView->listItemFill(true);
    m_listView->setFocusPolicy(Qt::TabFocus);
    m_isshowH = true;
    if (m_buttonbox->checkedId() == 0)
        emit clearbtnShow(m_isshowH);
}

void SciHistoryWidgetTab::iconChanged(int type, int id)
{
    QString path;
    if (type == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    if (id == 0) {
        m_memorybtn->setIcon(QIcon(path + "icon_memory_checked.svg"));
        m_historybtn->setIcon(QIcon(path + "icon_history_normal.svg"));
    } else {
        m_memorybtn->setIcon(QIcon(path + "icon_memory_normal.svg"));
        m_historybtn->setIcon(QIcon(path + "icon_history_checked.svg"));
    }
}

/**
 * @brief SciHistoryWidgetTab::cleanButtonEvent
 * 仅在科学模式下，根据当前内存/历史界面进行清除
 */
void SciHistoryWidgetTab::cleanButtonEvent()
{
    if (m_buttonbox->checkedId() == 0) {
        m_listModel->clearItems();
        m_listView->listItemFill(false);
        m_isshowH = false;
        if (m_buttonbox->checkedId() == 0)
            emit clearbtnShow(m_isshowH);
        m_listView->setFocusPolicy(Qt::NoFocus);
    } else {
        memoryPublic->memoryclean();
        m_isshowM = false;
        if (m_buttonbox->checkedId() == 1)
            emit clearbtnShow(m_isshowM);
        m_memorywidget->setFocusPolicy(Qt::NoFocus);
    }
}
