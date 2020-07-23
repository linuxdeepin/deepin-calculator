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

#include "mainwindow.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QShortcut>
#include <QProcess>
#include <QDesktopWidget>
#include <QApplication>
#include <QPropertyAnimation>
#include <DWidgetUtil>
#include <DPalette>

#include "dtitlebar.h"
#include "dthememanager.h"
#include "dhidpihelper.h"
#include "utils.h"

DGUI_USE_NAMESPACE

const int HISTORY_SHOW_LEAST_WIDTH = 800; //最小显示历史记录的宽度
const QSize STANDARD_SIZE = QSize(344, 560); //标准模式的固定大小
const QSize SCIENTIFIC_MIN_SIZE = QSize(430, 540); //科学模式的最小size
const QSize SCIENTIFIC_MAX_SIZE = QSize(800, 540); //科学模式的最大size

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    m_settings = DSettings::instance(this);
    m_mainLayout = new QStackedLayout;
    m_tbMenu = new DMenu(this);
    QIcon t_icon = QIcon::fromTheme("deepin-calculator");
    titlebar()->setIcon(t_icon);
    titlebar()->setMenu(m_tbMenu);
    titlebar()->setTitle("");

    //缺翻译
    m_simpleAction = new QAction(tr("Standard"), this);
    m_scAction = new QAction(tr("Scientific"), this);
    m_hisAction = new QAction(tr("view historical record"), this);

    m_pActionGroup = new QActionGroup(nullptr); //实现互斥checked
    m_pActionGroup->addAction(m_simpleAction);
    m_pActionGroup->addAction(m_scAction);
    m_simpleAction->setCheckable(true);
    m_scAction->setCheckable(true);

#ifdef ENABLE_SCIENTIFIC
    m_modeshowmenu = new DMenu(tr("show"));
    m_tbMenu->addAction(m_hisAction);
    m_tbMenu->addSeparator(); //添加分隔符
    m_modeshowmenu->addAction(m_simpleAction);
    m_modeshowmenu->addAction(m_scAction);
    m_tbMenu->addMenu(m_modeshowmenu);
#endif

    initModule();
    initTheme();

//    titlebar()->addWidget(m_historyBtn, Qt::AlignRight);
//    m_historyBtn->setToolTip(tr("History"));

    setWindowTitle(tr("Calculator"));

    if (m_firstInitMode == 0)
        m_basicModule->setFocus();
    else
        m_scientificModule->setFocus();
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &MainWindow::initTheme);
    connect(m_simpleAction, &QAction::triggered, this, &MainWindow::switchToSimpleMode);
    connect(m_scAction, &QAction::triggered, this, &MainWindow::switchToScientificMode);
    connect(m_hisAction, &QAction::triggered, this, [ = ]() {
        if (m_mainLayout->currentIndex() == (m_firstInitMode == 0 ? 1 : 0)) {
            if (m_settings->getOption("history").toInt() == 0) {
                if (width() < HISTORY_SHOW_LEAST_WIDTH)
                    resize(HISTORY_SHOW_LEAST_WIDTH, this->height());
                showHistoryWidget();
                emit windowChanged(width(), height(), false);
            } else {
                hideHistoryWidget(true);
                if (width() == HISTORY_SHOW_LEAST_WIDTH)
                    resize(width() - 1, this->height());
                emit windowChanged(width(), height(), true);
            }
        } else
            hideHistoryWidget(false);
    });

}

MainWindow::~MainWindow()
{
}

void MainWindow::initTheme()
{
    int type = DGuiApplicationHelper::instance()->paletteType();
    QString path;
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        DPalette titlePa = titlebar()->palette();
        titlePa.setColor(DPalette::Light, QColor(240, 240, 240));
        titlePa.setColor(DPalette::Dark, QColor(240, 240, 240));
        titlePa.setColor(DPalette::Base, QColor(240, 240, 240));
        titlebar()->setPalette(titlePa);
        path = QString(":/assets/images/%1/").arg("light");
    } else {
        DPalette titlePa = titlebar()->palette();
        QColor normalbackground = QColor(0, 0, 0);
        normalbackground.setAlphaF(0.1);
        titlePa.setColor(DPalette::Light, normalbackground);
        titlePa.setColor(DPalette::Dark, normalbackground);
        titlePa.setColor(DPalette::Base, normalbackground);
//        titlePa.setColor(DPalette::Light, QColor(37, 37, 37));
//        titlePa.setColor(DPalette::Dark, QColor(37, 37, 37));
//        titlePa.setColor(DPalette::Base, QColor(37, 37, 37));
        titlebar()->setPalette(titlePa);
        path = QString(":/assets/images/%1/").arg("dark");
    }
}

void MainWindow::initModule()
{
    int mode = m_settings->getOption("mode").toInt();
    QWidget *centralWidget = new QWidget;

    centralWidget->setLayout(m_mainLayout);
    setCentralWidget(centralWidget);

//    m_basicModule = new BasicModule;
//    m_scientificModule = new scientificModule;

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
//    m_mainLayout->addWidget(m_basicModule);
//    m_mainLayout->addWidget(m_scientificModule);

    m_isinit = true;
    switch (mode) {
    case 0:
        m_basicModule = new BasicModule;
        m_mainLayout->addWidget(m_basicModule);
        m_firstInitMode = 0;
        m_isStandInit = true;
        m_simpleAction->setChecked(true);
        switchToSimpleMode();
        break;
    case 1:
        m_scientificModule = new scientificModule;
        m_mainLayout->addWidget(m_scientificModule);
        m_firstInitMode = 1;
        m_isSciInit = true;
        m_scAction->setChecked(true);
        switchToScientificMode();
//        resize(m_settings->getOption("windowWidth").toInt(), m_settings->getOption("windowHeight").toInt());
        resize(SCIENTIFIC_MIN_SIZE);
        break;
    default:
        m_basicModule = new BasicModule;
        m_mainLayout->addWidget(m_basicModule);
        m_firstInitMode = 0;
        m_isStandInit = true;
        m_simpleAction->setChecked(true);
        switchToSimpleMode();
        break;
    }
    m_isinit = false;
}

void MainWindow::switchToSimpleMode()
{
    m_hisAction->setVisible(false);
    if (!m_isStandInit) {
        m_basicModule = new BasicModule;
        m_mainLayout->addWidget(m_basicModule);
        m_isStandInit = true;
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    }
    if (m_settings->getOption("mode") != 0 || m_isinit) {
        m_lastscisize = m_isinit ? STANDARD_SIZE : this->size();
        m_settings->setOption("mode", 0);
        m_mainLayout->setCurrentIndex((m_firstInitMode == 0 ? 0 : 1));
        m_basicModule->checkLineEmpty();
        hideHistoryWidget(false);
    }
}

void MainWindow::switchToScientificMode()
{
    m_hisAction->setVisible(true);
    if (!m_isSciInit) {
        m_scientificModule = new scientificModule;
        m_mainLayout->addWidget(m_scientificModule);
        m_isSciInit = true;
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    }
    connect(this, &MainWindow::windowChanged, m_scientificModule, &scientificModule::getWindowChanged);
    if (m_settings->getOption("mode") != 1 || m_isinit) {
        m_settings->setOption("mode", 1);
        m_mainLayout->setCurrentIndex((m_firstInitMode == 0 ? 1 : 0));
        m_scientificModule->checkLineEmpty();
        setMinimumSize(SCIENTIFIC_MIN_SIZE);
//        setMaximumSize(QApplication::desktop()->screenGeometry().size());
        setMaximumSize(SCIENTIFIC_MAX_SIZE);
        hideHistoryWidget(false);
        resize(m_lastscisize);
    }
}

void MainWindow::showHistoryWidget()
{
    m_settings->setOption("history", 1);
    m_scientificModule->showOrHideHistory(false);
}

void MainWindow::hideHistoryWidget(bool hissetting)
{
    //从科学到简易时b=false，其余为true
    if (hissetting == true)
        m_settings->setOption("history", 0);
    if (m_isSciInit)
        m_scientificModule->showOrHideHistory(true);
    switch (m_settings->getOption("mode").toInt()) {
    case 0:
        setFixedSize(STANDARD_SIZE);
        break;
    case 1:
        setMinimumSize(SCIENTIFIC_MIN_SIZE);
        setMaximumSize(SCIENTIFIC_MAX_SIZE);
        if (m_isinit)
            this->setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
        break;
    default:
        setFixedSize(STANDARD_SIZE);
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (m_mainLayout->currentIndex() == (m_firstInitMode == 0 ? 0 : 1))
        m_basicModule->setKeyPress(event);
    else
        m_scientificModule->setKeyPress(event);
    return;
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    m_settings->setOption("windowX", event->pos().x());
    m_settings->setOption("windowY", event->pos().y());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (event->size().width() < HISTORY_SHOW_LEAST_WIDTH) {
        hideHistoryWidget(true);
        emit windowChanged(event->size().width(), event->size().height(), true);
    } else {
        showHistoryWidget();
        emit windowChanged(event->size().width(), event->size().height(), false);
    }
    m_settings->setOption("windowWidth", event->size().width());
    m_settings->setOption("windowHeight", event->size().height());
    DMainWindow::resizeEvent(event);
}

