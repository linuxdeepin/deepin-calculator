#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>
#include <dgraphicsgloweffect.h>

DWIDGET_USE_NAMESPACE

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
    DGraphicsGlowEffect *effect;
};

#endif











