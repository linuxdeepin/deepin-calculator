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

#include "mainwindowtab.h"

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

const int BUTTONBOX_WIDTH = 160;
const int BUTTONBOX_HEIGHT = 36;

MainWindowTab::MainWindowTab(QWidget *parent)
    : DMainWindow(parent)
{
    m_settings = DSettingsAlt::instance(this);
    m_mainLayout = new QStackedLayout();
    QIcon t_icon = QIcon::fromTheme("deepin-calculator");
    titlebar()->setIcon(t_icon);
    titlebar()->setTitle("");

    //删除按钮
    m_clearbutton = new IconButtonTab(this, 1, false);
    m_clearbutton->hide();
    titlebar()->addWidget(m_clearbutton, Qt::AlignRight);

    DButtonBox *buttonbox = new DButtonBox(this);
    DButtonBoxButton *simplebtn = new DButtonBoxButton(tr("Standard"), this);
    DButtonBoxButton *scientificbtn = new DButtonBoxButton(tr("Scientific"), this);

    buttonbox->setFixedSize(BUTTONBOX_WIDTH, BUTTONBOX_HEIGHT);
    buttonbox->setFocusPolicy(Qt::NoFocus);
    QList<DButtonBoxButton *> listBtnBox;
    simplebtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    simplebtn->setFocusPolicy(Qt::TabFocus);
    scientificbtn->setFixedSize(BUTTONBOX_WIDTH / 2, BUTTONBOX_HEIGHT);
    listBtnBox << simplebtn << scientificbtn;
    buttonbox->setFocusPolicy(Qt::NoFocus);
    buttonbox->setButtonList(listBtnBox, true);
    buttonbox->setId(simplebtn, 0);
    buttonbox->setId(scientificbtn, 1);

    //暂时无程序员模式，防止与pc模式的冲突
    int checkbtnID = m_settings->getOption("mode").toInt();
    checkbtnID = checkbtnID > 1 ? 0 : checkbtnID;
    buttonbox->button(checkbtnID)->setChecked(true);

    titlebar()->addWidget(buttonbox, Qt::AlignLeft | Qt::AlignCenter);

    initModule();
    initTheme();

    setWindowTitle(tr("Calculator"));

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &MainWindowTab::initTheme);
    connect(simplebtn, &QAbstractButton::clicked, this, &MainWindowTab::switchToSimpleMode);
    connect(scientificbtn, &QAbstractButton::clicked, this, &MainWindowTab::switchToScientificMode);
}

MainWindowTab::~MainWindowTab()
{
//    PerformanceMonitor::closeAPPFinish();
}

void MainWindowTab::initTheme()
{
    int type = DGuiApplicationHelper::instance()->themeType();
    QString path;
    if (type == 1) {
        DPalette titlePa = titlebar()->palette();
        titlePa.setColor(DPalette::Light, QColor(240, 240, 240));
        titlePa.setColor(DPalette::Dark, QColor(240, 240, 240));
        titlePa.setColor(DPalette::Base, QColor(240, 240, 240));
        titlebar()->setPalette(titlePa);
        path = QString(":/assets/images/%1/").arg("light");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_normal.svg", path + "empty_normal.svg", 1);
    } else {
        DPalette titlePa = titlebar()->palette();
        QColor normalbackground = QColor(0, 0, 0);
        normalbackground.setAlphaF(0.1);
        titlePa.setColor(DPalette::Light, normalbackground);
        titlePa.setColor(DPalette::Dark, normalbackground);
        titlePa.setColor(DPalette::Base, normalbackground);
        titlebar()->setPalette(titlePa);
        path = QString(":/assets/images/%1/").arg("dark");
        m_clearbutton->setIconUrl(path + "empty_normal.svg", path + "empty_normal.svg", path + "empty_normal.svg", 1);
    }
}

void MainWindowTab::initModule()
{
    int mode = m_settings->getOption("mode").toInt();
    QWidget *centralWidget = new QWidget(this);

    centralWidget->setLayout(m_mainLayout);
    setCentralWidget(centralWidget);

    m_mainLayout->setContentsMargins(0, 0, 0, 0);

    QRect rect = QApplication::desktop()->availableGeometry();
    m_windowsize = QSize(rect.width(), rect.height());
    m_isVer = rect.width() < rect.height();
    m_isinit = true;
    switch (mode) {
    case 0:
        m_basicModule = new BasicModuleTab(this);
        m_mainLayout->addWidget(m_basicModule);
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
        switchToSimpleMode();
        break;
    case 1:
        m_scientificModule = new scientificModuleTab(this);
        m_mainLayout->addWidget(m_scientificModule);
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
        switchToScientificMode();
        break;
    default:
        m_basicModule = new BasicModuleTab(this);
        m_mainLayout->addWidget(m_basicModule);
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
        switchToSimpleMode();
        break;
    }
    m_isinit = false;
}

void MainWindowTab::switchToSimpleMode()
{
    if (!m_basicModule) {
        m_basicModule = new BasicModuleTab(this);
        m_mainLayout->addWidget(m_basicModule);
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    }
    if (m_settings->getOption("mode") != 0 || m_isinit) {
        m_settings->setOption("mode", 0);
        m_mainLayout->setCurrentWidget(m_basicModule);
        connect(m_basicModule, &BasicModuleTab::sciMemTab, this, [ = ]() {
            titlebar()->setFocus();
        }, Qt::UniqueConnection);
        connect(m_clearbutton, &QPushButton::clicked, this, [ = ]() {
            if (m_mainLayout->currentWidget() == m_basicModule)
                m_basicModule->titleBarClean();
        }, Qt::UniqueConnection);
        connect(m_basicModule, &BasicModuleTab::clearbtnShow, this, [ = ](bool show) {
            m_standShow = show;
            if (m_mainLayout->currentWidget() == m_basicModule)
                m_clearbutton->setHidden(!m_standShow);
        }, Qt::UniqueConnection);
        m_basicModule->checkLineEmpty();
    }
    m_basicModule->resetWindowSize(m_windowsize);
}

void MainWindowTab::switchToScientificMode()
{
    if (!m_scientificModule) {
        m_scientificModule = new scientificModuleTab(this);
        m_mainLayout->addWidget(m_scientificModule);
        emit DGuiApplicationHelper::instance()->themeTypeChanged(DGuiApplicationHelper::instance()->themeType());
    }
    if (m_settings->getOption("mode") != 1 || m_isinit) {
        m_settings->setOption("mode", 1);
        m_mainLayout->setCurrentWidget(m_scientificModule);
        connect(m_scientificModule, &scientificModuleTab::sciMemTab, this, [ = ]() {
            titlebar()->setFocus();
        }, Qt::UniqueConnection);
        connect(m_clearbutton, &QPushButton::clicked, this, [ = ]() {
            if (m_mainLayout->currentWidget() == m_scientificModule)
                m_scientificModule->titleBarClean();
        }, Qt::UniqueConnection);
        connect(m_scientificModule, &scientificModuleTab::clearbtnShow, this, [ = ](bool show) {
            m_sciShow = show;
            if (m_mainLayout->currentWidget() == m_scientificModule)
                m_clearbutton->setHidden(!m_sciShow);
        }, Qt::UniqueConnection);
        m_scientificModule->checkLineEmpty();
    }
    m_scientificModule->resetWindowSize(m_windowsize);
}

/**
 * @brief 焦点不在basicmodul时也触发keypress
 */
void MainWindowTab::keyPressEvent(QKeyEvent *event)
{
    if (m_mainLayout->currentWidget() == m_basicModule)
        m_basicModule->setKeyPress(event);
    else
        m_scientificModule->setKeyPress(event);
    return;
}

void MainWindowTab::moveEvent(QMoveEvent *event)
{
    m_settings->setOption("windowX", event->pos().x());
    m_settings->setOption("windowY", event->pos().y());
}

void MainWindowTab::resizeEvent(QResizeEvent *event)
{
    m_settings->setOption("windowWidth", event->size().width());
    m_settings->setOption("windowHeight", event->size().height());
    QRect rect = QApplication::desktop()->availableGeometry();
    m_windowsize = QSize(rect.width(), rect.height());
    if (m_settings->getOption("mode").toInt() != 1) {
        switchToSimpleMode();
    } else {
        switchToScientificMode();
    }
    DMainWindow::resizeEvent(event);
}

void MainWindowTab::closeEvent(QCloseEvent *event)
{
//    PerformanceMonitor::closeAppStart();
    DMainWindow::closeEvent(event);
}

