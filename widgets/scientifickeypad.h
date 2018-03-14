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

#ifndef SCIENTIFICKEYPAD_H
#define SCIENTIFICKEYPAD_H

#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include "textbutton.h"
#include "iconbutton.h"

class ScientificKeypad : public QWidget
{
    Q_OBJECT

public:
    enum Buttons {
        Key_Clear, Key_Percent, Key_Backspace, Key_Div,
        Key_7, Key_8, Key_9, Key_Mult,
        Key_4, Key_5, Key_6, Key_Min,
        Key_1, Key_2, Key_3, Key_Plus,
        Key_0, Key_Point, Key_Brackets, Key_Equals
    };

    ScientificKeypad(QWidget *parent = nullptr);
    ~ScientificKeypad();

    static const struct KeyDescription {
        QString text;
        Buttons button;
        int row;
        int column;
    } keyDescriptions[];

private:
    void initButtons();

private:
    QGridLayout *m_layout;
    QHash<Buttons, QPair<QPushButton *, const KeyDescription *>> m_keys;
};

#endif
