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
#include "src/utils.h"
#include "src/math/quantity.h"

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
    , m_listView(new SimpleListView(1))
    , m_listDelegate(new SimpleListDelegate(1, this))
    , m_listModel(new SimpleListModel(1, this))
    , m_buttonbox(new DButtonBox(this))
    , m_clearbutton(new IconButton(this, 1))
    , m_clearbuttonM(new IconButton(this, 1))
    , m_buttonstack(new QStackedWidget(this))
{
    memoryPublic = MemoryPublic::instance(this);
    m_memorywidget = memoryPublic->getwidget(MemoryPublic::scientificright);
    m_stacklayout = new QStackedLayout();
    QVBoxLayout *m_Vlayout = new QVBoxLayout(this);
    QHBoxLayout *m_Hlayout = new QHBoxLayout();
    m_Hlayout->addSpacing(LEFT_SPACE);
    m_Hlayout->addWidget(m_buttonbox, 0, Qt::AlignCenter);
    m_buttonbox->setFixedWidth(BUTTONBOX_WIDTH);
    m_Hlayout->addSpacing(MID_SPACE);

    //button stack
    m_Hlayout->addWidget(m_buttonstack);
    m_buttonstack->setFixedSize(CLEARBUTTON_SIZE);
    m_buttonstack->addWidget(m_clearbutton);
    m_buttonstack->addWidget(m_clearbuttonM);
    m_buttonstack->setCurrentIndex(0);
    m_clearbutton->showtooltip(false); //设置历史垃圾桶tooltip
    m_clearbuttonM->showtooltip(true); //设置内存垃圾桶tooltip
    m_clearbutton->setHidden(!(m_isshowH & m_indexH));
    m_isshowM = !memoryPublic->isWidgetEmpty(1);
    m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));

    m_Hlayout->addSpacing(RIGHT_SPACE);
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
    DButtonBoxButton *historybtn = new DButtonBoxButton(tr("History"));
    historybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    DButtonBoxButton *memorybtn = new DButtonBoxButton(tr("Memory"));
    memorybtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    listBtnBox << historybtn << memorybtn;
    m_buttonbox->setButtonList(listBtnBox, true);
    m_buttonbox->setId(historybtn, 0);
    m_buttonbox->setId(memorybtn, 1);
    connect(m_buttonbox->button(0), &QAbstractButton::clicked, this, [ = ]() {
        m_indexH = true;
        m_indexM = false;
        m_stacklayout->setCurrentIndex(0);
        m_buttonstack->setCurrentIndex(0);
        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
    });
    connect(m_buttonbox->button(1), &QAbstractButton::clicked, this, [ = ]() {
        m_indexH = false;
        m_indexM = true;
        m_stacklayout->setCurrentIndex(1);
        m_buttonstack->setCurrentIndex(1);
        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    connect(m_clearbutton, &IconButton::clicked, this, [ = ]() {
        m_listModel->clearItems();
        m_listView->listItemFill(false);
        m_isshowH = false;
        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
        setFocus();
    });
    connect(m_listModel, &SimpleListModel::hisbtnhidden, this, [ = ]() {
        m_listModel->clearItems(); //历史记录无数据信号接收
        m_listView->listItemFill(false);
        m_isshowH = false;
        m_clearbutton->setHidden(!(m_isshowH & m_indexH));
        setFocus();
    });
    connect(m_clearbuttonM, &IconButton::clicked, this, [ = ]() {
        memoryPublic->memoryclean();
        setFocus();
    });
    connect(memoryPublic, &MemoryPublic::filledMem, this, [ = ]() {
        m_isshowM = true; //公共内存中有数据信号接收
        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    connect(memoryPublic, &MemoryPublic::emptyMem, this, [ = ]() {
        m_isshowM = false; //公共内存中无数据信号接收
        m_clearbuttonM->setHidden(!(m_isshowM & m_indexM));
    });
    //防止foucus到输入栏或者buttonbox
    connect(memoryPublic, &MemoryPublic::publicwidgetclean, this, [ = ](int mode) {
        if (mode == 1)
            this->setFocus();
    });
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
 * @brief 返回memorywidget
 */
MemoryWidget *SciHistoryWidget::getMemoryWidget()
{
    return m_memorywidget;
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
    m_clearbuttonM->setIconUrl(path + "empty_normal.svg", path + "empty_hover.svg", path + "empty_press.svg", 1);
    memoryPublic->setThemeType(typeIn);
    m_listDelegate->setThemeType(typeIn);
}

/**
 * @brief 历史记录中有内容
 */
void SciHistoryWidget::historyfilled()
{
    if (m_isshowH == false)
        m_listModel->deleteItem(1);
    m_listView->listItemFill(true);
    m_isshowH = true;
    m_clearbutton->setHidden(!(m_isshowH & m_indexH));
}
