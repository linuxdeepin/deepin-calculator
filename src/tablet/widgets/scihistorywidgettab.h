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

#ifndef SCIHISTORYWIDGETTAB_H
#define SCIHISTORYWIDGETTAB_H

#include <QStackedLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QApplication>
#include <DButtonBox>
#include <DIconButton>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "tablet/control/textbuttontab.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/control/basickeypadtab.h"
#include "tablet/control/memorykeypadtab.h"
#include "tablet/views/memorywidgettab.h"
#include "tablet/memorypublictab.h"
#include "widgets/inputedit.h"
#include "expressionbartab.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

/**
 * @brief 科学计算器历史记录界面
 */
class SciHistoryWidgetTab : public DWidget
{
    Q_OBJECT
public:
    /**
     * @brief 内存运算类型
     */
    enum memOperate {
        generateData, memoryplus, memoryminus, memoryclean, //MS, M+, M-, MC
        widgetplus, widgetminus //内存中的M+, M-
    };
    SciHistoryWidgetTab(int mode = 0, QWidget *parent = nullptr);//mode: 0-标准 1-科学
    ~SciHistoryWidgetTab();
//    void paintEvent(QPaintEvent *event);
    void focusOnButtonbox();
    void memoryFunctions(memOperate operate = generateData, Quantity answer = Quantity(), int row = -1);
    void resetFocus();

    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
//    void focusInEvent(QFocusEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public:
    SimpleListViewTab *m_listView;
    SimpleListDelegateTab *m_listDelegate;
    SimpleListModel *m_listModel;

signals:
    void hisbtnClicked();
    void clearbtnShow(bool show);

public slots:
    void themeChanged(int type);
    void historyfilled();
    void iconChanged(int type, int id);
    void cleanButtonEvent();
    void resetWidgetSize(QSize size);

private:
    QStackedWidget *m_stacklayout;
    MemoryWidgetTab *m_memorywidget;
    DButtonBox *m_buttonbox;
    DButtonBoxButton *m_historybtn;
    DButtonBoxButton *m_memorybtn;
    MemoryPublicTab *memoryPublic;
    //false-hide true-show,index-当前是历史界面/内存界面
    bool m_isshowM = false; //内存中无内容为false
    bool m_isshowH = false;
    bool m_iskeypressfocus = false;
    int m_mode = 0; //当前的模式
    int m_themeType = 0; //主题
};

#endif // SCIHISTORYWIDGET_H
