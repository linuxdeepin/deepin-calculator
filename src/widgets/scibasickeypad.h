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

#ifndef SCIBASICKEYPAD_H
#define SCIBASICKEYPAD_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QSignalMapper>
#include <QDebug>
#include <DPushButton>
#include <DGuiApplicationHelper>
#include <DSuggestButton>

#include "textbutton.h"
#include "iconbutton.h"
#include "equalbutton.h"
#include "tricombobox.h"
#include "funcombobox.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 科学计算器键盘区域界面（有组合框，未使用）
 */
class SciBasicKeyPad : public DWidget
{
    Q_OBJECT

public:
    /**
     * @brief 科学计算器键盘
     */
    enum Buttons {
        Key_PI = 26, Key_e, Key_Clear, Key_Backspace, //π, e, C, 退格
        Key_Derivative, Key_Modulus, Key_exp, Key_mod, //1/x, 绝对值（模）, exp, mod
        Key_Left, Key_Right, Key_Factorials, Key_Div, //(, ), !(阶乘), 除号
        Key_7, Key_8, Key_9, Key_Mult, //7, 8, 9, 乘号
        Key_4, Key_5, Key_6, Key_Min, //4, 5, 6, -
        Key_1, Key_2, Key_3, Key_Plus, //1, 2, 3, +
        Key_Percent, Key_0, Key_Point, Key_Equal, //%, 0, ., =
        Key_2page, Key_x2, Key_x3, Key_xy, Key_10x, Key_log, Key_ln, //page2, x², x³, xʸ, 10ˣ, log, ln
        Key_sqrt2, Key_sqrt3, Key_yx, Key_2x, Key_logyx, Key_ex, //√x, ³√x, ʸ√x, 2ˣ, logᵧx, eˣ
        Key_abs = 95, Key_down, Key_up, //绝对值, 向下取整, 向上取整,
        Key_rand, Key_dms, Key_deg, //随机数, 度分秒, 角度
        Key_Combo1page = 67, Key_sin, Key_cos, Key_tan, //函数组合框的page按钮, sin, cos, tan
        Key_hyp1, Key_sec, Key_csc, Key_cot, //hyp1按钮, sec, csc, cot
        Key_arcsin, Key_arccos, Key_arctan, //arcsin, arccos, arctan
        Key_arcsec, Key_arccsc, Key_arccot, //arcsec, arccsc, arccot
        Key_sinh, Key_cosh, Key_tanh, //sinh, cosh, tanh
        Key_sech, Key_csch, Key_coth, //sech, csch, coth
        Key_arcsinh, Key_arccosh, Key_arctanh, //arcsin, arccosh, arctanh
        Key_arcsech, Key_arccsch, Key_arccoth, //arcsech, arccsch, arccoth
        Key_Combo2page, Key_hyp2 //三角函数组合框的page按钮, hyp2按钮
    };

    SciBasicKeyPad(QWidget *parent = nullptr);
    ~SciBasicKeyPad();

    DPushButton *button(Buttons key);
    DPushButton *funbutton(Buttons key);
    DPushButton *tributton(Buttons key);
//    DSuggestButton *button();
    void animate(Buttons key);
    void animate();
    void mousePressEvent(QMouseEvent *event);

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
    static const struct KeyDescriptionfun {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } keyDescriptionsfun[];
    static const struct KeyDescriptiontri {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } KeyDescriptionstri[];
    static const struct KeyDescriptiontri1 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } KeyDescriptionstri1[];
    static const struct KeyDescriptiontri2 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } KeyDescriptionstri2[];
    static const struct KeyDescriptiontri3 {
        QString text;
        Buttons button;
        int row;
        int column;
        int rowcount;
        int columncount;
    } KeyDescriptionstri3[];
public slots:
    void initButtons();
    void buttonThemeChanged(int type);
    void turnPage(int key);
    void funhide();
signals:
    void buttonPressed(int);
    void equalPressed();
    void moveLeft();
    void moveRight();
    void funshow();
    void funinside();

private:
    void initUI();

private:
    DPushButton *m_tri;
    DPushButton *m_fun;
    QStackedLayout *m_stacklayout;
    QStackedLayout *m_tristacklayout;
    QHBoxLayout *m_hlayout;
    QVBoxLayout *m_vlayout;
    QGridLayout *m_gridlayout1;
    QGridLayout *m_gridlayout2;
    QGridLayout *m_fungridlayout;
    QGridLayout *m_trigridlayout1;
    QGridLayout *m_trigridlayout2;
    QGridLayout *m_trigridlayout3;
    QGridLayout *m_trigridlayout4;
    QWidget *m_funwidget;
    QWidget *m_triwidget;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription1 *>> m_keys1;
    QHash<Buttons, QPair<DPushButton *, const KeyDescriptionfun *>> m_funkeys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescriptiontri *>> m_trikeys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescriptiontri1 *>> m_trikeys1;
    QHash<Buttons, QPair<DPushButton *, const KeyDescriptiontri2 *>> m_trikeys2;
    QHash<Buttons, QPair<DPushButton *, const KeyDescriptiontri3 *>> m_trikeys3;
    ComboBox *m_triCombobox;
    FunCombobox *m_funCombobox;
    QPoint m_mousepoint;
    QWidget *m_hwidget;
    QWidget *m_stackwidget;
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};

#endif // SCIBASICKEYPAD_H
