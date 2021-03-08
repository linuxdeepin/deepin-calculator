/*
* Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
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

#include "probitwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

const QSize SIZE = QSize(76, 46);

ProBitWidget::ProBitWidget(QWidget *parent)
    : DWidget(parent),
      m_firstbtn(new BitButton(this)),
      m_secondbtn(new BitButton(this)),
      m_thirdbtn(new BitButton(this)),
      m_fourthbtn(new BitButton(this)),
      m_label(new DLabel(this))
{
    setFixedSize(SIZE);
    QHBoxLayout *hlay = new QHBoxLayout();
    QVBoxLayout *vlay = new QVBoxLayout(this);
    hlay->addWidget(m_firstbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_secondbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_thirdbtn);
    hlay->addSpacing(1);
    hlay->addWidget(m_fourthbtn);
    hlay->addSpacing(1);
    hlay->setMargin(0);
    hlay->setSpacing(4);
    hlay->setContentsMargins(0, 0, 0, 0);
    m_label->setFixedSize(70, 20);
    m_label->setAlignment(Qt::AlignRight);

    QFont font;
    font.setPixelSize(14);
    font.setFamily("Noto Sans");
    m_label->setFont(font);
    vlay->addLayout(hlay);
    vlay->addWidget(m_label);
    vlay->setMargin(0);
    vlay->setSpacing(0);
    vlay->setContentsMargins(0, 0, 0, 0);
}

ProBitWidget::~ProBitWidget()
{

}

/**
 * @brief 设置当前widget，button可用状态
 * @param enabled true-可用　false-不可用
 */
void ProBitWidget::setButtonEnabled(bool enabled)
{
    m_firstbtn->setEnabled(enabled);
    m_secondbtn->setEnabled(enabled);
    m_thirdbtn->setEnabled(enabled);
    m_fourthbtn->setEnabled(enabled);
}

/**
 * @brief ProBitWidget::getbutton
 * @param i：第几个按钮
 * @return 对应按钮
 */
BitButton *ProBitWidget::getbutton(int i)
{
    switch (i) {
    case 3:
        return m_fourthbtn;
    case 2:
        return m_thirdbtn;
    case 1:
        return m_secondbtn;
    case 0:
        return m_firstbtn;
    default:
        return m_firstbtn;
    }
}
