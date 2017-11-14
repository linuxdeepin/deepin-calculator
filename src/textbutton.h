#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>

class TextButton : public QPushButton
{
    Q_OBJECT

public:
    TextButton(const QString &text, QWidget *parent = 0);
    ~TextButton();

protected:
    void initShadow();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    void showShadowEffect();
    void hideShadowEffect();

private:
    QGraphicsDropShadowEffect *effect;
};

#endif











