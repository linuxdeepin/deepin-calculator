#include "textbutton.h"

TextButton::TextButton(const QString &text)
{    
    effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor("#75C8FF"));
    effect->setOffset(0);
    effect->setBlurRadius(5);
    setGraphicsEffect(effect);
    effect->setEnabled(false);

    setText(text);
    setFixedHeight(55);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}

void TextButton::enterEvent(QEvent *)
{
    effect->setEnabled(true);
}

void TextButton::leaveEvent(QEvent *)
{
    effect->setEnabled(false);
}