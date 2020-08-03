/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     xiajing <xiajing@uniontech.com>
*
* Maintainer: jingzhou <jingzhou@uniontech.com>
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

#include "memorykeypad.h"

#include <QTimer>
#include <DPalette>
#include <DImageButton>

#include "dthememanager.h"

const MemoryKeypad::KeyDescription MemoryKeypad::keyDescriptions[] = {
    {"MC", Key_MC, 1, 0, 1, 1},       {"MR", Key_MR, 1, 1, 1, 1},
    {"M+", Key_Mplus, 1, 2, 1, 1},    {"M-", Key_Mminus, 1, 3, 1, 1},
    {"MS", Key_MS, 1, 4, 1, 1},       {"M˅", Key_Mlist, 1, 5, 1, 1},
};

MemoryKeypad::MemoryKeypad(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    this->setFixedHeight(35);
    m_layout->setMargin(0);
    m_layout->setSpacing(2);  //按钮比ui大2pix,此处比ui小2pix
    m_layout->setContentsMargins(0, 0, 0, 0);

    initButtons();
    this->setContentsMargins(12, 0, 13, 0);

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
}

MemoryKeypad::~MemoryKeypad()
{
}

void MemoryKeypad::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

/**
 * @brief 根据枚举值返回相应button
 * @param key 按钮名
 */
DPushButton *MemoryKeypad::button(Buttons key)
{
    return m_keys.value(key).first;
}

/**
 * @brief 按钮点击动画效果
 */
void MemoryKeypad::animate(Buttons key)
{
    MemoryButton *btn = static_cast<MemoryButton *>(button(key));
    btn->animate();
}

/**
 * @brief 初始化button
 */
void MemoryKeypad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;
        button = new MemoryButton(desc->text);
        QFont font = button->font();
        font.setFamily("Noto Sans");
        button->setFont(font);

        m_layout->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                            Qt::AlignTop);
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue); //key为枚举值，value.first为DPushButton *, value.second为const KeyDescription *

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();}); //点击及焦点移除时update
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &MemoryKeypad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &MemoryKeypad::moveRight);
        m_mapper->setMapping(button, desc->button); //多个按钮绑定到一个mapper上
    }
}
