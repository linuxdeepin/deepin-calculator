#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>
#include <QGraphicsDropShadowEffect>

class TextButton : public QPushButton
{
    Q_OBJECT

public:
    TextButton(const QString &text);
    ~TextButton();

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    QGraphicsDropShadowEffect *effect;
};

#endif











