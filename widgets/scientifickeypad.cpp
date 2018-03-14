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

#include "scientifickeypad.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

const ScientificKeypad::KeyDescription ScientificKeypad::keyDescriptions[] = {
    { "C", Key_Clear, 1, 0 },
    { "%", Key_Percent, 1, 1 },
    { "", Key_Backspace, 1, 2 },
    { "÷", Key_Div, 1, 3 },
    { "sin", Key_1, 1, 4},
    { "cos", Key_1, 1, 5},
    { "tan", Key_1, 1, 6},

    { "7", Key_7, 2, 0 },
    { "8", Key_8, 2, 1 },
    { "9", Key_9, 2, 2 },
    { "×", Key_Mult, 2, 3 },
    { "arcsin", Key_Mult, 2, 4 },
    { "arccos", Key_Mult, 2, 5 },
    { "arctan", Key_Mult, 2, 6 },

    { "4", Key_4, 3, 0 },
    { "5", Key_5, 3, 1 },
    { "6", Key_6, 3, 2 },
    { "－", Key_Min, 3, 3 },
    { "√", Key_Mult, 3, 4 },
    { "lg", Key_Mult, 3, 5 },
    { "ln", Key_Mult, 3, 6 },

    { "1", Key_1, 4, 0 },
    { "2", Key_2, 4, 1 },
    { "3", Key_3, 4, 2 },
    { "＋", Key_Plus, 4, 3 },
    { "π", Key_Min, 4, 4 },
    { "e", Key_Min, 4, 5 },
    { "!", Key_Min, 4, 6 },

    { "0", Key_0, 5, 0 },
    { ".", Key_Point, 5, 1 },
    { "√", Key_Brackets, 5, 2 },
    { "=", Key_Equals, 5, 3 },
    { "(", Key_Equals, 5, 4 },
    { ")", Key_Equals, 5, 5 },
    { "^", Key_Equals, 5, 6 }
};

ScientificKeypad::ScientificKeypad(QWidget *parent)
    : QWidget(parent),
      m_layout(new QGridLayout(this))
{
    m_layout->setMargin(0);
    m_layout->setSpacing(1);

    initButtons();
}

ScientificKeypad::~ScientificKeypad()
{
}

void ScientificKeypad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        QPushButton *button;

        if (desc->button == Key_Backspace) {
            button = new IconButton;
            (static_cast<IconButton *>(button))->setIcon(QString(":/images/delete_%1_normal.svg")
                                                         .arg(DThemeManager::instance()->theme()));
        } else {
            button = new TextButton(desc->text);
        }

        m_layout->addWidget(button, desc->row, desc->column);
        const QPair<QPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);
    }
}
