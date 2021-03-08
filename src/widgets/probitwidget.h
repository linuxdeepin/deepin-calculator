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

#ifndef PROBITWIDGET_H
#define PROBITWIDGET_H

#include <DWidget>
#include <DLabel>
#include <DGuiApplicationHelper>

#include "../control/bitbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 一块二进制键盘控件
 */
class ProBitWidget : public DWidget
{
    Q_OBJECT

public:
    explicit ProBitWidget(QWidget *parent = nullptr);
    ~ProBitWidget();
    void setButtonEnabled(bool enabled);
    BitButton *getbutton(int i);

private:
    BitButton *m_firstbtn;
    BitButton *m_secondbtn;
    BitButton *m_thirdbtn;
    BitButton *m_fourthbtn;
    DLabel *m_label;
};

#endif // PROBITWIDGET_H
