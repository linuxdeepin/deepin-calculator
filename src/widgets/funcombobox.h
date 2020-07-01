#ifndef FUNCOMBOBOX_H
#define FUNCOMBOBOX_H
/*******************************************************************************
 1. @类名:    FunCombobox
 2. @作者:    夏菁
 3. @日期:    2020-07-01
 4. @说明:    科学计算器其他函数组合框（未使用）
*******************************************************************************/
#include <QWidget>
#include <DComboBox>
#include <DPushButton>
#include <DListWidget>
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

class FunCombobox : public DComboBox
{
    Q_OBJECT

public:
    enum Buttons {
        Key_abs = 95, Key_down, Key_up,
        Key_rand, Key_dms, Key_deg
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
    QWidget *widget;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
};

#endif // FUNCOMBOBOX_H
