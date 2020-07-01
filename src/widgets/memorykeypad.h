#ifndef MEMORYKEYPAD_H
#define MEMORYKEYPAD_H
/*******************************************************************************
 1. @类名:    MemoryKeypad
 2. @作者:    夏菁
 3. @日期:    2020-07-01
 4. @说明:    简易计算器内存按键栏
*******************************************************************************/
#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>
#include <DGuiApplicationHelper>
#include <DSuggestButton>
#include "textbutton.h"
#include "iconbutton.h"
#include "memorybutton.h"
#include "equalbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class MemoryKeypad : public DWidget
{
    Q_OBJECT

public:
    enum Buttons {Key_MC = 20, Key_MR, Key_Mplus, Key_Mminus, Key_MS, Key_Mlist};

    MemoryKeypad(QWidget *parent = nullptr);
    ~MemoryKeypad();

    DPushButton *button(Buttons key);
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
public slots:
    void initButtons();
    void buttonThemeChanged(int type);
signals:
    void buttonPressed(int);
    void equalPressed();
    void moveLeft();
    void moveRight();

private:
    void initUI();

private:
    QGridLayout *m_layout;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
    //DSuggestButton *m_equal;
    //EqualButton *m_equal;
};

#endif // MEMORYKEYPAD_H
