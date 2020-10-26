#ifndef BITBUTTON_H
#define BITBUTTON_H

#include <QDebug>
#include <DPushButton>
#include <DPalette>
#include <DGuiApplicationHelper>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 程序员模式0-1二进制按钮
 */
class BitButton : public DPushButton
{
    Q_OBJECT
public:
    BitButton(QWidget *parent = nullptr);
    ~BitButton();

    void init();
    void animate(bool isspace = false, int msec = 100);
    void setButtonState(bool b);

public:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *e);
    void focusOutEvent(QFocusEvent *);
    void keyPressEvent(QKeyEvent *e);

signals:
    void updateInterface();
    void focus(int direction); //direction 0-上　1-下　2-左　3-右
    void space();

private:
    bool m_isHover = false;
    bool m_isPress = false;
    bool m_btnState = false; //true-1, false-0
    QFont m_font;
};

#endif // BITBUTTON_H
