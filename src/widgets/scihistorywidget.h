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

#ifndef SCIHISTORYWIDGET_H
#define SCIHISTORYWIDGET_H

#include <QStackedLayout>
#include <QStackedWidget>
#include <QApplication>
#include <DButtonBox>
#include <DIconButton>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "../control/textbutton.h"
#include "../control/iconbutton.h"
#include "../control/basickeypad.h"
#include "../control/memorykeypad.h"
#include "inputedit.h"

#include "expressionbar.h"
#include "../views/memorywidget.h"
#include "../memorypublic.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

/**
 * @brief 科学计算器历史记录界面
 */
class SciHistoryWidget : public DWidget
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
    explicit SciHistoryWidget(QWidget *parent = nullptr);
    ~SciHistoryWidget();
//    void paintEvent(QPaintEvent *event);
    void focusOnButtonbox();
    void memoryFunctions(memOperate operate = generateData, Quantity answer = Quantity(), int row = -1);
    void resetFocus();

    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
//    void focusInEvent(QFocusEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public:
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;

signals:
    void hisbtnClicked();

public slots:
    void themeChanged(int type);
    void historyfilled();

private:
    QStackedLayout *m_stacklayout;
    MemoryWidget *m_memorywidget;
    DButtonBox *m_buttonbox;
    DButtonBoxButton *m_historybtn;
    DButtonBoxButton *m_memorybtn;
    IconButton *m_clearbutton; //历史界面清除按钮
    MemoryPublic *memoryPublic;
    //false-hide true-show,index-当前是历史界面/内存界面
    bool m_isshowM = false; //内存中无内容为false
//    bool m_indexM = false;
    bool m_isshowH = false;
//    bool m_indexH = true;
    bool m_iskeypressfocus = false;
};

#endif // SCIHISTORYWIDGET_H
