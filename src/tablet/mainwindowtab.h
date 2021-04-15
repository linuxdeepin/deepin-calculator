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

#ifndef MAINWINDOWTAB_H
#define MAINWINDOWTAB_H

#include <QStackedLayout>
#include <QMenu>
#include <DMainWindow>

#include "tablet/widgets/basicmoduletab.h"
#include "tablet/widgets/scientificmoduletab.h"
#include "tablet/widgets/basicmodulever.h"
#include "tablet/widgets/scientificmodulever.h"
#include "tablet/control/iconbuttontab.h"
#include "dsettings.h"
#include "performancemonitor.h"

DWIDGET_USE_NAMESPACE

class MainWindowTab : public DMainWindow
{
    Q_OBJECT

public:
    MainWindowTab(QWidget *parent = nullptr);
    ~MainWindowTab();

public slots:
    //void onViewShortcut();
protected:
    //void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void initTheme();
    void initModule();
    void switchToSimpleMode();
    void switchToScientificMode();

private:
    DSettingsAlt *m_settings;
    QStackedLayout *m_mainLayout;

    BasicModuleTab *m_basicModule = nullptr;
    scientificModuleTab *m_scientificModule = nullptr;

    IconButtonTab *m_clearbutton;

    bool m_isinit = true;//初始化

    bool m_standShow = false; //标准显示清除按钮
    bool m_sciShow = false; //科学显示清除按钮
    bool m_standVerShow = false; //标准显示清除按钮
    bool m_sciVerShow = false;
    QSize m_windowsize = QSize();

    //横竖屏
    bool m_isVer = false;
};

#endif
