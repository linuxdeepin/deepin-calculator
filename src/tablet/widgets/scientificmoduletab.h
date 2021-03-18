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

#ifndef SCIENTIFICMODELTAB_H
#define SCIENTIFICMODELTAB_H

#include <QStackedLayout>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "tablet/control/textbuttontab.h"
#include "tablet/control/iconbuttontab.h"
#include "tablet/control/basickeypadtab.h"
#include "tablet/control/memorykeypadtab.h"
#include "tablet/control/scientifickeypadtab.h"
#include "tablet/views/memorywidgettab.h"
#include "tablet/memorypublictab.h"
#include "widgets/inputedit.h"
#include "scihistorywidgettab.h"
#include "sciexpressionbartab.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

/**
 * @brief 科学模式界面
 */
class scientificModuleTab : public DWidget
{
    Q_OBJECT

public:
    scientificModuleTab(QWidget *parent = nullptr);
    ~scientificModuleTab();
    void setKeyPress(QKeyEvent *e);

    void checkLineEmpty();
signals:
    void changedeg(int deg);
    void sciMemTab();
    void clearbtnShow(bool show);

public slots:
    //memory func
    void mAvailableEvent();
    void mUnAvailableEvent();
    void titleBarClean();
    void resetWindowSize(QSize size);
private slots:
    void initTheme(int type);
private:
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void handleKeypadButtonPressByspace(int);
    void handleClearStateChanged(bool isAllClear);
    void handleDegChanged();
    void handleFEStateChanged(bool isdown);
    void handlePageStateChanged();
    void setScientificTabOrder();

private:
    SciExpressionBarTab *m_sciexpressionBar;
    ScientificKeyPadTab *m_scikeypadwidget;
    bool m_avail = false;
    int m_deg = 1;//1-deg 2-rad 3-grad
    bool m_FEisdown = false; //FE按键是否被按下
    bool m_Pageisdown = false; //2nd按键是否被按下
    bool m_memCalbtn; //m+,m-,ms
    bool m_memRCbtn;//mr,mc
    SciHistoryWidgetTab *m_scihiswidget;
    MemoryPublicTab *m_memoryPublic;
};

#endif // SCIENTIFICMODELTAB_H
