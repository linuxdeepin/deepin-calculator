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

#include "basickeypad.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

const BasicKeypad::KeyDescription BasicKeypad::keyDescriptions[] = {
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

static QPushButton* createSpecialKeyButton(BasicKeypad::Buttons key) {
    IconButton *button = new IconButton(10, 13);

    if (key == BasicKeypad::Key_Div) {
        button->setIcon(":/images/div_normal.svg");
    } else if (key == BasicKeypad::Key_Mult) {
        button->setIcon(":/images/mult_normal.svg");
    } else if (key == BasicKeypad::Key_Min) {
        button->setIcon(":/images/min_normal.svg");
    } else if (key == BasicKeypad::Key_Plus) {
        button->setIcon(":/images/plus_normal.svg");
    } else if (key == BasicKeypad::Key_Backspace) {
        button->setIconStateSizes(23, 26);
        button->setIcon(QString(":/images/delete_%1_normal.svg")
                        .arg(DThemeManager::instance()->theme()));
    }

    return button;
}

BasicKeypad::BasicKeypad(QWidget *parent)
    : QWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    m_layout->setMargin(0);
    m_layout->setSpacing(1);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &BasicKeypad::handleThemeChanged);
}

BasicKeypad::~BasicKeypad()
{
}

QPushButton* BasicKeypad::button(Buttons key)
{
    return m_keys.value(key).first;
}

void BasicKeypad::animate(Buttons key)
{
    TextButton *btn = static_cast<TextButton *>(button(key));
    btn->animate();
}

void BasicKeypad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        QPushButton *button;

        if (desc->text.isEmpty()) {
            button = createSpecialKeyButton(desc->button);
        } else {
            button = new TextButton(desc->text);
        }

        m_layout->addWidget(button, desc->row, desc->column);
        const QPair<QPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);

        connect(button, &QPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        m_mapper->setMapping(button, desc->button);
    }
}

void BasicKeypad::initUI()
{
    QHashIterator<Buttons, QPair<QPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy(Qt::NoFocus);
    }

    button(Key_Div)->setObjectName("SymbolButton");
    button(Key_Mult)->setObjectName("SymbolButton");
    button(Key_Min)->setObjectName("SymbolButton");
    button(Key_Plus)->setObjectName("SymbolButton");
    button(Key_Equals)->setObjectName("EqualButton");
}

void BasicKeypad::handleThemeChanged()
{
    IconButton *btn = static_cast<IconButton *>(button(Key_Backspace));
    btn->setIcon(QString(":/images/delete_%1_normal.svg").arg(DThemeManager::instance()->theme()));
}
