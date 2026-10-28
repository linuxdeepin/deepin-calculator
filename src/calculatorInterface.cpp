// Copyright (C) 2017 ~ 2025 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "calculatorInterface.h"
#include "ddlog.h"
#include "mainwindow.h"
#include "dsettings.h"

#include <QDebug>
#include <QDBusConnection>

CalculatorInterface::CalculatorInterface(MainWindow *parent)
    : QDBusAbstractAdaptor(parent)
    , m_mainWindow(parent)
{
    qCDebug(appLog) << "CalculatorInterface created";
    // Register D-Bus object
    QDBusConnection::sessionBus().registerObject("/", parent, QDBusConnection::ExportAdaptors);
}

CalculatorInterface::~CalculatorInterface()
{
    qCDebug(appLog) << "CalculatorInterface destroyed";
}

bool CalculatorInterface::showWindow()
{
    qCDebug(appLog) << "D-Bus: showWindow() called";
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
    qCDebug(appLog) << "D-Bus: hideWindow() called";
    if (m_mainWindow) {
        m_mainWindow->hide();
        return true;
    }
    return false;
}

bool CalculatorInterface::raiseWindow()
{
    qCDebug(appLog) << "D-Bus: raiseWindow() called";
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
    qCDebug(appLog) << "D-Bus: quitWindow() called";
    if (m_mainWindow) {
        m_mainWindow->close();
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToSpecialMode(int mode)
{
    qCDebug(appLog) << "D-Bus: switchToSpecialMode() called with mode:" << mode;
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
                qCWarning(appLog) << "D-Bus: Invalid mode:" << mode;
                return false;
        }
        return true;
    }
    return false;
}

bool CalculatorInterface::switchToStandardMode()
{
    qCDebug(appLog) << "D-Bus: switchToStandardMode() called";
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
    qCDebug(appLog) << "D-Bus: switchToScientificMode() called";
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
    qCDebug(appLog) << "D-Bus: switchToProgrammerMode() called";
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
    qCDebug(appLog) << "D-Bus: getCurrentMode() called";
    if (m_mainWindow) {
        DSettingsAlt *settings = DSettingsAlt::instance();
        return settings->getOption("mode").toInt();
    }
    return -1;
}
