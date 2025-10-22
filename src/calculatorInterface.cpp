// Copyright (C) 2017 ~ 2025 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "calculatorInterface.h"
#include "mainwindow.h"
#include "dsettings.h"

#include <QDebug>
#include <QDBusConnection>

CalculatorInterface::CalculatorInterface(MainWindow *parent)
    : QDBusAbstractAdaptor(parent)
    , m_mainWindow(parent)
{
    qDebug() << "CalculatorInterface created";
    // Register D-Bus object
    QDBusConnection::sessionBus().registerObject("/", parent, QDBusConnection::ExportAdaptors);
}

CalculatorInterface::~CalculatorInterface()
{
    qDebug() << "CalculatorInterface destroyed";
}

bool CalculatorInterface::showWindow()
{
    qDebug() << "D-Bus: showWindow() called";
    if (m_mainWindow) {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
        return true;
    }
    return false;
}

bool CalculatorInterface::hideWindow()
{
    qDebug() << "D-Bus: hideWindow() called";
    if (m_mainWindow) {
        m_mainWindow->hide();
        return true;
    }
    return false;
}

bool CalculatorInterface::raiseWindow()
{
    qDebug() << "D-Bus: raiseWindow() called";
    if (m_mainWindow) {
        m_mainWindow->show();
        m_mainWindow->raise();
        m_mainWindow->activateWindow();
        return true;
    }
    return false;
}

bool CalculatorInterface::quitWindow()
{
    qDebug() << "D-Bus: quitWindow() called";
    if (m_mainWindow) {
        m_mainWindow->close();
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToSpecialMode(int mode)
{
    qDebug() << "D-Bus: switchToSpecialMode() called with mode:" << mode;
    if (m_mainWindow) {
        switch (mode) {
            case 0:
                switchToStandardMode();
                break;
            case 1:
                switchToScientificMode();
                break;
            case 2:
                switchToProgrammerMode();
                break;
            default:
                qWarning() << "D-Bus: Invalid mode:" << mode;
                return false;
        }
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToStandardMode()
{
    qDebug() << "D-Bus: switchToStandardMode() called";
    if (m_mainWindow) {
        m_mainWindow->switchToSimpleMode();
        // Set the action checked state using accessor method
        QAction *simpleAction = m_mainWindow->simpleAction();
        if (simpleAction) {
            simpleAction->setChecked(true);
        }
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToScientificMode()
{
    qDebug() << "D-Bus: switchToScientificMode() called";
    if (m_mainWindow) {
        m_mainWindow->switchToScientificMode();
        // Set the action checked state using accessor method
        QAction *scAction = m_mainWindow->scientificAction();
        if (scAction) {
            scAction->setChecked(true);
        }
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToProgrammerMode()
{
    qDebug() << "D-Bus: switchToProgrammerMode() called";
    if (m_mainWindow) {
        m_mainWindow->switchToProgrammerMode();
        // Set the action checked state using accessor method
        QAction *programmerAction = m_mainWindow->programmerAction();
        if (programmerAction) {
            programmerAction->setChecked(true);
        }
        return true;
    }
    return false;
}

int CalculatorInterface::getCurrentMode()
{
    qDebug() << "D-Bus: getCurrentMode() called";
    if (m_mainWindow) {
        DSettingsAlt *settings = DSettingsAlt::instance();
        return settings->getOption("mode").toInt();
    }
    return -1;
}
