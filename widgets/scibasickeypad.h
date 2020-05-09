#ifndef SCIBASICKEYPAD_H
#define SCIBASICKEYPAD_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QSignalMapper>
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

class SciBasicKeyPad : public DWidget
{
    Q_OBJECT

public:
    enum Buttons {
        Key_PI = 26, Key_e, Key_Clear, Key_Backspace,
        Key_Derivative, Key_Modulus, Key_exp, Key_mod,
        Key_Left, Key_Right, Key_Factorials, Key_Div,
        Key_7, Key_8, Key_9, Key_Mult,
        Key_4, Key_5, Key_6, Key_Min,
        Key_1, Key_2, Key_3, Key_Plus,
        Key_Percent, Key_0, Key_Point, Key_Equal,
        Key_2page, Key_x2, Key_x3, Key_xy, Key_10x, Key_log, Key_ln,
        Key_sqrt2, Key_sqrt3, Key_yx, Key_2x, Key_logyx, Key_ex
    };

    SciBasicKeyPad(QWidget *parent = nullptr);
    ~SciBasicKeyPad();

    DPushButton *button(Buttons key);
    DSuggestButton *button();
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
signals:
    void buttonPressed(int);
    void equalPressed();
    void moveLeft();
    void moveRight();

private:
    void initUI();

private:
    DPushButton *tri;
    DPushButton *fun;
    QStackedLayout *m_stacklayout;
    QHBoxLayout *m_hlayout;
    QVBoxLayout *m_vlayout;
    QGridLayout *m_gridlayout1;
    QGridLayout *m_gridlayout2;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription1 *>> m_keys1;
    ComboBox *m_triCombobox;
    FunCombobox *m_funCombobox;
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};

#endif // SCIBASICKEYPAD_H
