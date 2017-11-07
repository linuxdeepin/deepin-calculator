#include "textbutton.h"
#include <QDebug>

TextButton::TextButton(const QString &text)
{    
    effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor("#2CA7F8"));
    effect->setOffset(0);
    effect->setBlurRadius(15);
    effect->setEnabled(false);
    setGraphicsEffect(effect);

    setText(text);
    setFixedSize(82, 55);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}

void TextButton::enterEvent(QEvent *)
{
    setFixedSize(81, 54);
    setFixedHeight(55);
    effect->setEnabled(true);
}

void TextButton::leaveEvent(QEvent *)
{
    setFixedSize(82, 55);
    effect->setEnabled(false);
}
