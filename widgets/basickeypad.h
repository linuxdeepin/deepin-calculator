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

#ifndef BASICKEYPAD_H
#define BASICKEYPAD_H

#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include "textbutton.h"
#include "iconbutton.h"
#include "buttons.h"

class BasicKeypad : public QWidget
{
    Q_OBJECT

public:

    BasicKeypad(QWidget *parent = nullptr);
    ~BasicKeypad();

    QPushButton *button(Buttons key);
    void animate(Buttons key);

signals:
    void buttonPressed(int);

protected:
    void initButtons();
    void initUI();
    void handleThemeChanged();

protected:
    QGridLayout *m_layout;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<QPushButton *, const KeyDescription *>> m_keys;

private:
    const KeyDescription keyDescriptions[20]={
        { "C", Key_Clear, 1, 0 },
        { "%", Key_Percent, 1, 1 },
        { "", Key_Backspace, 1, 2 },
        { "", Key_Div, 1, 3 },

        { "7", Key_7, 2, 0 },
        { "8", Key_8, 2, 1 },
        { "9", Key_9, 2, 2 },
        { "", Key_Mult, 2, 3 },

        { "4", Key_4, 3, 0 },
        { "5", Key_5, 3, 1 },
        { "6", Key_6, 3, 2 },
        { "", Key_Min, 3, 3 },

        { "1", Key_1, 4, 0 },
        { "2", Key_2, 4, 1 },
        { "3", Key_3, 4, 2 },
        { "", Key_Plus, 4, 3 },

        { "0", Key_0, 5, 0 },
        { ".", Key_Point, 5, 1 },
        { "( )", Key_Brackets, 5, 2 },
        { "=", Key_Equals, 5, 3 }
    };
};

#endif
