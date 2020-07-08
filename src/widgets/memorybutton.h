/*
 * 1. @类名:    MemoryButton
 * 2. @作者:    夏菁 ut000489
 * 3. @日期:    2020-07-01
 * 4. @说明:    简易计算器内存按键
 */
#ifndef MEMORYBUTTON_H
#define MEMORYBUTTON_H

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QHelpEvent>
#include <QToolTip>
#include <DPalette>
#include <DPushButton>

#include "src/dsettings.h"
#include "textbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class MemoryButton : public TextButton
{
    Q_OBJECT

public:
    MemoryButton(const QString &text = QString(), bool listwidgetbtn = false, QWidget *parent = nullptr); //listwidgetbtn-是否是内存界面中的按钮
    ~MemoryButton();

    void init();
    // void showShadow();
    // void hideShadow();
    void animate(int msec = 100);
    void setbtnlight(bool light);
    void showtips();
public slots:
    void setbuttongray(bool memorywidgetshow);

signals:
    void moveLeft();
    void moveRight();
    void updateInterface();

public:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *e);
    void focusOutEvent(QFocusEvent *);

private:
    QGraphicsDropShadowEffect *m_effect;

    DPalette m_palette;
    DSettings *m_settings;
    bool m_isHover;
    bool m_isPress;
    bool m_isgray; //按键不可用置灰
    QFont m_font;
    bool m_widgetbtn; //内存列表按键
    bool m_isallgray; //内存区显示置灰
    bool m_islight = false;
    bool m_isacting = false; //规避鼠标在button内release时出现foucus状态的框
};

#endif  // MEMORYBUTTON_H
