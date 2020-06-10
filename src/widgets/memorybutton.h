#ifndef MEMORYBUTTON_H
#define MEMORYBUTTON_H

#include <DPalette>
#include <DPushButton>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QHelpEvent>
#include <QToolTip>

#include "../dsettings.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class MemoryButton : public DPushButton
{
    Q_OBJECT

public:
    MemoryButton(const QString &text = QString(), bool listwidgetbtn = false, QWidget *parent = nullptr);
    ~MemoryButton();

    void init();
    // void showShadow();
    // void hideShadow();
    void animate(int msec = 100);
    void setbtnlight(bool light);
    void showtips();
public slots:
    void setbuttongray(bool b);

signals:
    void moveLeft();
    void moveRight();
    void updateInterface();

public:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void keyPressEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *e);
    void focusOutEvent(QFocusEvent *);

private:
    // QGraphicsDropShadowEffect *m_effect;

    DPalette m_palette;
    DSettings *m_settings;
    bool m_isHover;
    bool m_isPress;
    bool m_isgray;
    QFont m_font;
    bool widgetbtn;
    bool m_isallgray;
    bool m_islight = false;
};

#endif  // MEMORYBUTTON_H
