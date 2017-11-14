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

    void showShadowEffect();
    void hideShadowEffect();

protected:
    void initShadow();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QGraphicsDropShadowEffect *effect;
};

#endif











