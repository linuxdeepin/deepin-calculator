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

#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QSignalMapper>
#include <DGuiApplicationHelper>
#include <DSuggestButton>
#include <DComboBox>

#include "src/control/textbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 科学计算器三角函数组合框（未使用）
 */
class ComboBox : public DComboBox
{
    Q_OBJECT

public:
    /**
     * @brief 三角函数组合框按钮
     */
    enum Buttons {
        Key_Combo1page = 67, Key_sin, Key_cos, Key_tan, //page1, sin, cos, tan
        Key_hyp1, Key_sec, Key_csc, Key_cot, //hyp1, sec, csc, cot
        Key_arcsin, Key_arccos, Key_arctan, //arcsin, arccos, arctan
        Key_arcsec, Key_arccsc, Key_arccot, //arcsec, arccsc, arccot
        Key_sinh, Key_cosh, Key_tanh, //sinh, cosh, tanh
        Key_sech, Key_csch, Key_coth, //sech, csch, coth
        Key_arcsinh, Key_arccosh, Key_arctanh, //arcsinh, arccosh, arctanh
        Key_arcsech, Key_arccsch, Key_arccoth, //arcsech, arccsch, arccoth
        Key_Combo2page, Key_hyp2 //page2, hyp2
    };

    ComboBox(DComboBox *parent = nullptr);
    ~ComboBox();

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
    static const struct KeyDescription1 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } keyDescriptions1[];
    static const struct KeyDescription2 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } keyDescriptions2[];
    static const struct KeyDescription3 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } keyDescriptions3[];
public slots:
    void initButtons();
//    void buttonThemeChanged(int type);
    void turnPage(int key);
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
    QGridLayout *m_gridlayout2;
    QGridLayout *m_gridlayout3;
    QGridLayout *m_gridlayout4;
    QListWidget *m_listwidget;
    QWidget *m_widget;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription1 *>> m_keys1;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription2 *>> m_keys2;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription3 *>> m_keys3;
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};

#endif // COMBOBOX_H
