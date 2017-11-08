#include "textbutton.h"
#include <QDebug>

TextButton::TextButton(const QString &text)
{    
    effect = new DGraphicsGlowEffect(this);
    effect->setColor(QColor(12, 155, 246, 255 * 0.2));
    effect->setOffset(0, 4);
    effect->setBlurRadius(10);
    effect->setEnabled(false);
    effect->setDistance(3);
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
    if (text() == "＝")
        return;

    effect->setEnabled(true);
    raise();
}

void TextButton::leaveEvent(QEvent *)
{
    effect->setEnabled(false);
}
