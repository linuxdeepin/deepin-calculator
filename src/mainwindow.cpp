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
#include "dtitlebar.h"
#include "dthememanager.h"
#include "dhidpihelper.h"
#include "utils.h"

#include <DWidgetUtil>
#include <DPalette>

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QShortcut>
#include <QProcess>
#include <QDesktopWidget>
#include <QApplication>
#include <QPropertyAnimation>

DGUI_USE_NAMESPACE

const int HISTORY_SHOW_LEAST_WIDTH = 800; //最小显示历史记录的宽度
const QSize STANDARD_SIZE = QSize(344, 560); //标准模式的固定大小
const QSize SCIENTIFIC_MIN_SIZE = QSize(430, 580); //科学模式的最小size

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
{
    m_settings = DSettings::instance(this);
    m_mainLayout = new QStackedLayout;
    m_tbMenu = new DMenu(this);
    m_historyBtn = new IconButton(this, 2);
    //titlebar()->setBackgroundTransparent(true);
    QIcon t_icon = QIcon::fromTheme("deepin-calculator");
//    QPixmap pixmap = t_icon.pixmap(24,24);
//    QIcon newIcon(pixmap);
    titlebar()->setIcon(t_icon);
    titlebar()->setMenu(m_tbMenu);
    titlebar()->setTitle("");

    // init titlebar menu.
    //m_themeAction = new QAction(tr("Dark theme"), this);
    //m_themeAction->setCheckable(true);
    m_simpleAction = new QAction(tr("Standard"), this);
    m_scAction = new QAction(tr("Scientific"), this);
    //m_tbMenu->addAction(m_themeAction);

#ifdef ENABLE_SCIENTIFIC
    m_tbMenu->addAction(m_simpleAction);
    m_tbMenu->addAction(m_scAction);
#endif

    //m_tbMenu->addSeparator();

    initModule();
    initTheme();

    titlebar()->addWidget(m_historyBtn, Qt::AlignRight);
    m_historyBtn->setToolTip(tr("History"));

    setWindowTitle(tr("Calculator"));

    m_basicModule->setFocus();
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &MainWindow::initTheme);
    connect(m_simpleAction, &QAction::triggered, this, &MainWindow::switchToSimpleMode);
    connect(m_scAction, &QAction::triggered, this, &MainWindow::switchToScientificMode);
    connect(m_historyBtn, &IconButton::isclicked, this, [ = ]() {
        if (m_mainLayout->currentIndex() == 1) {
            if (m_settings->getOption("history").toInt() == 0) {
                if (width() < HISTORY_SHOW_LEAST_WIDTH)
                    resize(HISTORY_SHOW_LEAST_WIDTH, this->height());
                showHistoryWidget();
                emit windowChanged(width(), height(), false);
            } else {
                hideHistoryWidget(true, false);
                if (width() == HISTORY_SHOW_LEAST_WIDTH)
                    resize(width() - 1, this->height());
                emit windowChanged(width(), height(), true);
            }
        } else
            hideHistoryWidget(false, false);
    });
    connect(this, &MainWindow::windowChanged, m_scientificModule, &scientificModule::getWindowChanged);
}

MainWindow::~MainWindow()
{
}

void MainWindow::isOrderToShow()
{
    m_rightToShow = true;
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
        m_historyBtn->setIconUrl(path + "memory_normal.svg", path + "memory_hover.svg", path + "memory_press.svg");
    } else {
        DPalette titlePa = titlebar()->palette();
        titlePa.setColor(DPalette::Light, QColor(37, 37, 37));
        titlePa.setColor(DPalette::Dark, QColor(37, 37, 37));
        titlePa.setColor(DPalette::Base, QColor(37, 37, 37));
        titlebar()->setPalette(titlePa);
        path = QString(":/assets/images/%1/").arg("dark");
        m_historyBtn->setIconUrl(path + "memory_normal.svg", path + "memory_hover.svg", path + "memory_press.svg");
    }
}

void MainWindow::initModule()
{
    int mode = m_settings->getOption("mode").toInt();
    QWidget *centralWidget = new QWidget;

    centralWidget->setLayout(m_mainLayout);
    setCentralWidget(centralWidget);

    m_basicModule = new BasicModule;
    m_scientificModule = new scientificModule;

    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(m_basicModule);
    m_mainLayout->addWidget(m_scientificModule);

    m_isinit = true;
    switch (mode) {
    case 0:
        switchToSimpleMode();
        break;
    case 1:
        switchToScientificMode();
        resize(m_settings->getOption("windowWidth").toInt(), m_settings->getOption("windowHeight").toInt());
        break;
    default:
        switchToSimpleMode();
        break;
    }
    m_isinit = false;
}

void MainWindow::switchToSimpleMode()
{
    if (m_settings->getOption("mode") != 0 || m_isinit) {
        m_lastscisize = this->size();
        m_settings->setOption("mode", 0);
        m_mainLayout->setCurrentIndex(0);
        m_basicModule->checkLineEmpty();
        hideHistoryWidget(false, true);
    }
}

void MainWindow::switchToScientificMode()
{
    if (m_settings->getOption("mode") != 1 || m_isinit) {
        m_settings->setOption("mode", 1);
        m_mainLayout->setCurrentIndex(1);
        m_scientificModule->checkLineEmpty();
        setMinimumSize(SCIENTIFIC_MIN_SIZE);
        setMaximumSize(QApplication::desktop()->screenGeometry().size());
        hideHistoryWidget(false, true);
        resize(m_lastscisize);
    }
}

void MainWindow::showHistoryWidget()
{
    m_settings->setOption("history", 1);
    m_scientificModule->showOrHideHistory(false);
}

void MainWindow::hideHistoryWidget(bool hissetting, bool modechange)
{
    //从科学到简易时b=false，其余为true
    if (hissetting == true)
        m_settings->setOption("history", 0);
    m_scientificModule->showOrHideHistory(true);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(250);
    animation->setStartValue(0);
    animation->setEndValue(1);
    switch (m_settings->getOption("mode").toInt()) {
    case 0:
        setFixedSize(STANDARD_SIZE);
        if (m_rightToShow) {
            this->setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
            this->show();
            if (modechange)
                animation->start();
        }
        break;
    case 1:
        setMinimumSize(SCIENTIFIC_MIN_SIZE);
        setMaximumSize(QApplication::desktop()->screenGeometry().size());
        if (m_rightToShow) {
            this->setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
            this->show();
            if (modechange)
                animation->start();
        }
        break;
    default:
        setFixedSize(STANDARD_SIZE);
        this->setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
        this->show();
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (m_mainLayout->currentIndex() == 0)
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
        hideHistoryWidget(true, false);
        emit windowChanged(event->size().width(), event->size().height(), true);
    } else {
        showHistoryWidget();
        emit windowChanged(event->size().width(), event->size().height(), false);
    }
    m_settings->setOption("windowWidth", event->size().width());
    m_settings->setOption("windowHeight", event->size().height());
    if (event->size().width() <= HISTORY_SHOW_LEAST_WIDTH && m_settings->getOption("mode").toInt() != 0)
        m_historyBtn->show();
    else
        m_historyBtn->hide();
    DMainWindow::resizeEvent(event);
}

