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

#ifndef BASICMODULE_H
#define BASICMODULE_H

#include <QWidget>
#include <QStackedLayout>
#include <QGridLayout>

#include "../widgets/textbutton.h"
#include "../widgets/iconbutton.h"
#include "../widgets/basickeypad.h"
#include "expressionbar.h"

class BasicModule : public QWidget
{
    Q_OBJECT

public:
    BasicModule(QWidget *parent = nullptr);
    ~BasicModule();

    void switchToSimpleKeypad();
    void switchToScientificKeypad();

private:
    void initTheme();
    void handleEditKeyPress(QKeyEvent *);
    void handleKeypadButtonPress(int);
    void handleClearStateChanged(bool isAllClear);

protected:
    void paintEvent(QPaintEvent *);

private:
    ExpressionBar *m_expressionBar;
    BasicKeypad *m_basicKeypad;
    QStackedLayout *m_keypadLayout;
    QColor m_expBarColor;
    QColor m_expBarSepColor;
    QColor m_btnSepColor;
};

#endif
