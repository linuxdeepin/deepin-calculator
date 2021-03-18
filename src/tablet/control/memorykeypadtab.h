#ifndef MEMORYKEYPADTAB_H
#define MEMORYKEYPADTAB_H

#include "textbuttontab.h"
#include "iconbuttontab.h"
#include "memorybuttontab.h"
#include "equalbuttontab.h"

#include <DGuiApplicationHelper>
#include <DSuggestButton>

#include <QWidget>
#include <QGridLayout>
#include <QSignalMapper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 简易计算器内存按键栏
 */
class MemoryKeypadTab : public DWidget
{
    Q_OBJECT

public:
    /**
     * @brief 内存按键栏
     */
    enum Buttons {Key_MC = 20, Key_MR, Key_Mplus, Key_Mminus, Key_MS}; //MC, MR, M+, M-, MS

    MemoryKeypadTab(QWidget *parent = nullptr);
    ~MemoryKeypadTab();

    void mouseMoveEvent(QMouseEvent *e);

    DPushButton *button(Buttons key);
    DPushButton *button(int key);
    void animate(Buttons key, bool isspace = false);
    bool buttonHasFocus();

    static const struct KeyDescription {
        QString text; //按钮text
        Buttons button; //按钮枚举值名
        int row; //行
        int column; //列
        int rowcount; //所占行数
        int columncount; //所占列数
    } keyDescriptions[];
public slots:
    void initButtons();
    void getFocus(int);
    void resetWidgetSize(QSize size);
signals:
    void buttonPressed(int);
    void buttonPressedbySpace(int);
    void equalPressed();
    void moveLeft();
    void moveRight();

private:
    QGridLayout *m_layout;
    QSignalMapper *m_mapper;
    QHash<Buttons, QPair<DPushButton *, const KeyDescription *>> m_keys;
};

#endif // MEMORYKEYPADTAB_H
