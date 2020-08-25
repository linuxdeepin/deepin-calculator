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

#ifndef FUNCOMBOBOX_H
#define FUNCOMBOBOX_H

#include <QWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QSignalMapper>
#include <DGuiApplicationHelper>
#include <DSuggestButton>
#include <DComboBox>
#include <DPushButton>
#include <DListWidget>

#include "src/control/textbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 科学计算器其他函数组合框（未使用）
 */
class FunCombobox : public DComboBox
{
    Q_OBJECT

public:
    /**
     * @brief 函数组合框内按键
     */
    enum Buttons {
        Key_abs = 95, Key_down, Key_up, //绝对值, 向下取整, 向下取整
        Key_rand, Key_dms, Key_deg //随机数, 度分秒, 角度
    };

    FunCombobox(DComboBox *parent = nullptr);
    ~FunCombobox();

    DPushButton *button(Buttons key);
//    DSuggestButton *button();
    void animate(Buttons key);
    void animate();
    void updateData();

    static const struct KeyDescription {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } keyDescriptions[];

public slots:
    void initButtons();
//    void buttonThemeChanged(int type);
Q_SIGNALS:
    void buttonPressed(int);
    void equalPressed();
    void moveLeft();
    void moveRight();

private:
    void initUI();

private:
    QStackedLayout *m_stacklayout;
    QHBoxLayout *m_hlayout;
    QGridLayout *m_gridlayout1;
    DListWidget *m_listwidget;
    QWidget *m_widget;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
};

#endif // FUNCOMBOBOX_H
