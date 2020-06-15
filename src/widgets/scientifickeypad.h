#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QSignalMapper>
#include <DGuiApplicationHelper>
#include <DSuggestButton>
#include "textbutton.h"
#include "iconbutton.h"
#include "equalbutton.h"
#include "memorybutton.h"
#include <QDebug>
#include <DWidget>
#include <DLabel>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class ScientificKeyPad : public DWidget
{
    Q_OBJECT

public:
    enum Buttons {
        Key_FE = 26, Key_MC, Key_MR, Key_Mplus, Key_Mmin, Key_MS,
        Key_deg, Key_page, Key_PI, Key_e, Key_Mod, Key_Backspace,
        Key_sin, Key_x2, Key_Derivative, Key_Factorials, Key_exp, Key_Clear,
        Key_cos, Key_x3, Key_left, Key_right, Key_Percent, Key_Div,
        Key_tan, Key_xy, Key_7, Key_8, Key_9, Key_Mult,
        Key_cot, Key_10x, Key_4, Key_5, Key_6, Key_Min,
        Key_Modulus, Key_log, Key_1, Key_2, Key_3, Key_Plus,
        Key_Rand, Key_ln, Key_opposite, Key_0, Key_Point, Key_Equals,
        Key_arcsin, Key_arccos, Key_arctan, Key_arccot,
        Key_sqrt2, Key_sqrt3, Key_ysqrtx, Key_2x, Key_logyx, Key_ex
    };

    ScientificKeyPad(QWidget *parent = nullptr);
    ~ScientificKeyPad();

    DPushButton *button(Buttons key);
    DPushButton *funbutton(Buttons key);
    DPushButton *tributton(Buttons key);
//    DSuggestButton *button();
    void animate(Buttons key);
    void animate();

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

public slots:
    void initButtons();
    void buttonThemeChanged(int type);
    void turnPage(int key);
    void getdeg(int deg);
    void bracketsNum(int direction, QString num); //direction 0-左括号 1-右括号
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
    QWidget *page2;
    QVBoxLayout *m_vlayout;
    QGridLayout *m_gridlayout1;
    QGridLayout *m_gridlayout2;
    QSignalMapper *m_mapper;
    DLabel *leftBracket;
    DLabel *rightBracket;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription1 *>> m_keys1;
    QPoint m_mousepoint;
    int m_themetype;
    int m_deg = 3; //1-deg  2-rad  3-grad
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};
