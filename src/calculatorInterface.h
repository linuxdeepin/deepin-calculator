// Copyright (C) 2017 ~ 2025 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CALCULATORINTERFACE_H
#define CALCULATORINTERFACE_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusVariant>

class MainWindow;

class CalculatorInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.Calculator")

public:
    explicit CalculatorInterface(MainWindow *parent);
    virtual ~CalculatorInterface();

public Q_SLOTS:
    bool showWindow();
    bool hideWindow();
    bool raiseWindow();
    bool quitWindow();
    bool switchToSpecialMode(int mode);
    bool switchToStandardMode();
    bool switchToScientificMode();
    bool switchToProgrammerMode();
    int getCurrentMode();

Q_SIGNALS:
    void modeChanged(int mode);

private:
    MainWindow *m_mainWindow;
};

#endif // CALCULATORINTERFACE_H
