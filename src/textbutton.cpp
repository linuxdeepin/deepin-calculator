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
    setFixedSize(80, 60);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}
void TextButton::enterEvent(QEvent *)
{
    effect->setEnabled(true);
    raise();
}

void TextButton::leaveEvent(QEvent *)
{
    effect->setEnabled(false);
}
