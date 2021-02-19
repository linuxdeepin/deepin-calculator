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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dsettings.h"
#include "control/iconbutton.h"
#include "widgets/basicmodule.h"
#include "widgets/scientificmodule.h"
#include "widgets/programmodule.h"
#include "control/iconbutton.h"

#include <QStackedLayout>
#include <QMenu>
#include <DMainWindow>

#define ENABLE_SCIENTIFIC 1

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void initTheme();
    void initModule();
    void switchToSimpleMode();
    void switchToScientificMode();
    void switchToProgrammerMode();

private:
    DSettingsAlt *m_settings;
    QStackedLayout *m_mainLayout;
    DMenu *m_tbMenu;
    DMenu *m_modeshowmenu;
    QAction *m_simpleAction;
    QAction *m_scAction;
    QAction *m_programmerAction;
    QActionGroup *m_pActionGroup;

    BasicModule *m_basicModule = nullptr;
    scientificModule *m_scientificModule = nullptr;
    ProgramModule *m_programmerModule = nullptr;

    bool m_isinit = true;//初始化
    int programmerOldBase = 10;//切换前程序员计算器的进制
};

#endif
