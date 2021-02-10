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

#ifndef MEMHISWIDGET_H
#define MEMHISWIDGET_H

#include <QStackedWidget>
#include <QApplication>
#include <DButtonBox>
#include <DIconButton>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "../control/iconbutton.h"
#include "../views/memorywidget.h"
#include "../views/simplelistdelegate.h"
#include "../views/simplelistmodel.h"
#include "../views/simplelistview.h"
#include "../memorypublic.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

/**
 * @brief 科学模式历史记录及内存记录界面
 */
class MemHisWidget : public DWidget
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
    explicit MemHisWidget(QWidget *parent = nullptr);
    ~MemHisWidget();
    void focusOnButtonbox(Qt::FocusReason Reason);
    void memoryFunctions(memOperate operate = generateData, Quantity answer = Quantity(), int row = -1);
    void resetFocus();
    void historyfilled();

    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    void hisIsFilled(bool isfilled);
    void hideWidget();

public slots:
    void themeChanged(int type);
    void iconChanged(int type, int id);

private:
    QStackedWidget *m_stackWidget;
    SimpleListView *m_listView;
    SimpleListDelegate *m_listDelegate;
    SimpleListModel *m_listModel;
    MemoryWidget *m_memoryWidget;
    MemoryPublic *m_memoryPublic;
    DButtonBox *m_buttonBox;
    DButtonBoxButton *m_memoryBtn;
    DButtonBoxButton *m_historyBtn;
    IconButton *m_clearButton; //记录清除按钮
    bool m_isshowM = false; //内存中无内容为false
    bool m_isshowH = false;
    int m_themeType = 0; //当前主题
};

#endif // MEMHISWIDGET_H
