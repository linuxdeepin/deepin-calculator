#ifndef PROCHECKBTNKEYPAD_H
#define PROCHECKBTNKEYPAD_H

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

/**
 * @brief 程序员第一排按钮
 */
class ProCheckBtnKeypad : public DWidget
{
    Q_OBJECT

public:
    /**
     * @brief 按键栏
     */
    enum Buttons {Key_GeneralKeypad = 36, Key_BinaryKeypad, Key_System, Key_Option, Key_MS, Key_Mlist}; //普通键盘, 二进制键盘, 位数, 计算方法选项, MS, M^

    ProCheckBtnKeypad(QWidget *parent = nullptr);
    ~ProCheckBtnKeypad();

    void mouseMoveEvent(QMouseEvent *e);

    DPushButton *button(Buttons key);
    void animate(Buttons key, bool isspace = false);
    bool buttonHasFocus();

    static const struct KeyDescription {
        QString text; //按钮text
        Buttons button; //按钮枚举值名
        int row; //行
        int column; //列
    } keyDescriptions[];
public slots:
    void initButtons();
    void buttonThemeChanged(int type);
    void getFocus(int);
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

#endif // PROCHECKBTNKEYPAD_H
