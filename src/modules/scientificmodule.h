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

#ifndef SCIENTIFICMODEL_H
#define SCIENTIFICMODEL_H

#include <QStackedLayout>
#include <QGridLayout>
#include <QTimer>
#include <DWidget>
#include <DGuiApplicationHelper>

#include "src/widgets/textbutton.h"
#include "src/widgets/iconbutton.h"
#include "src/widgets/basickeypad.h"
#include "src/widgets/memorykeypad.h"
#include "src/widgets/tricombobox.h"
#include "src/widgets/scientifickeypad.h"
#include "src/widgets/inputedit.h"
#include "scihistorywidget.h"
#include "sciexpressionbar.h"
#include "memorywidget.h"
#include "memorypublic.h"

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

/**
 * @brief 科学模式界面
 */
class scientificModule : public DWidget
{
    Q_OBJECT

public:
    scientificModule(QWidget *parent = nullptr);
    ~scientificModule();
    void setKeyPress(QKeyEvent *e);

    void mousePressEvent(QMouseEvent *event);
    void showOrHideHistory(bool b);
    void checkLineEmpty();
signals:
    void changedeg(int deg);
    void getWindowChanged(int width, int height, bool hishide);

public slots:
    //memory func
    void mAvailableEvent();
    void mUnAvailableEvent();
private slots:
    void initTheme(int type);
private:
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void equalButtonPress();
    void handleClearStateChanged(bool isAllClear);
    void handleDegChanged();
    void handleFEStateChanged(bool isdown);
    void handlePageStateChanged();

protected:
    //void paintEvent(QPaintEvent *);

private:
    SciExpressionBar *m_sciexpressionBar;
    ScientificKeyPad *m_scikeypadwidget;
    bool m_insidewidget;
    bool m_avail = false;
    int m_deg = 1;//1-deg 2-rad 3-grad
    bool m_FEisdown = false;
    bool m_Pageisdown = false;
    bool m_isinsidefun = false;//鼠标点击在函数widget内
    bool m_memCalbtn; //m+,m-,ms
    bool m_memRCbtn;//mr,mc
    SciHistoryWidget *m_scihiswidget;
    MemoryPublic *m_memoryPublic;
};

#endif // SCIENTIFICMODEL_H
