#include "textbutton.h"

TextButton::TextButton(const QString &text)
{    
    effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor("#2CA7F8"));
    effect->setOffset(0);
    effect->setBlurRadius(5);
    effect->setEnabled(false);
    setGraphicsEffect(effect);

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