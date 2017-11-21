#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>

class TextButton : public QPushButton
{
    Q_OBJECT

public:
    TextButton(const QString &text = QString(), QWidget *parent = 0);
    ~TextButton();

    void showShadowEffect();
    void hideShadowEffect();
    void animate(int msec = 100);

protected:
    void initShadow();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QGraphicsDropShadowEffect *m_effect;
};

#endif











