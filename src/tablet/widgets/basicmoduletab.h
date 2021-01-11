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

#ifndef BASICMODULETAB_H
#define BASICMODULETAB_H

#include <QStackedLayout>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "tablet/control/textbuttontab.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/control/basickeypadtab.h"
#include "tablet/control/memorykeypadtab.h"
#include "widgets/inputedit.h"
#include "expressionbartab.h"
#include "tablet/views/memorywidgettab.h"
#include "tablet/memorypublictab.h"
#include "scihistorywidgettab.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

class BasicModuleTab : public DWidget
{
    Q_OBJECT

public:
    BasicModuleTab(QWidget *parent = nullptr);
    ~BasicModuleTab();

    void setKeyPress(QKeyEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void checkLineEmpty();

signals:
    void sciMemTab();
    void clearbtnShow(bool show);

public slots:
    //memory func
    void mAvailableEvent();
    void mUnAvailableEvent();
    void titleBarClean();
private slots:
    void initTheme(int type);
private:
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void handleKeypadButtonPressByspace(int);
    void handleClearStateChanged(bool isAllClear);
    void setBasicTabOrder();

protected:
    //void paintEvent(QPaintEvent *);

private:
    ExpressionBarTab *m_expressionBar;
    BasicKeypadTab *m_basicKeypad;
    MemoryKeypadTab *m_memoryKeypad;
    bool m_avail = false; //内存中有数据
    bool m_isinsidefun = false;//鼠标点击在函数widget内
    bool m_memCalbtn; //m+,m-,ms可用
    bool m_memRCbtn;//mr,mc可用
    SciHistoryWidgetTab *m_scihiswidget;
    MemoryPublicTab *m_memoryPublic;
};

#endif
