#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QWidget>
#include <DComboBox>
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
#include "textbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class ComboBox : public DComboBox
{
    Q_OBJECT

public:
    enum Buttons {
        Key_Combo1page = 67, Key_sin, Key_cos, Key_tan,
        Key_hyp1, Key_sec, Key_csc, Key_cot,
        Key_arcsin, Key_arccos, Key_arctan,
        Key_arcsec, Key_arccsc, Key_arccot,
        Key_sinh, Key_cosh, Key_tanh,
        Key_sech, Key_csch, Key_coth,
        Key_arcsinh, Key_arccosh, Key_arctanh,
        Key_arcsech, Key_arccsch, Key_arccoth,
        Key_Combo2page, Key_hyp2
    };

    ComboBox(DComboBox *parent = nullptr);
    ~ComboBox();

    DPushButton *button(Buttons key);
    DSuggestButton *button();
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
    void buttonThemeChanged(int type);
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
    QWidget *widget;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription1 *>> m_keys1;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription2 *>> m_keys2;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription3 *>> m_keys3;
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};

#endif // COMBOBOX_H
