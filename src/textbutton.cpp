#include "textbutton.h"
#include <QDebug>

TextButton::TextButton(const QString &text)
{    
    effect = new QGraphicsDropShadowEffect(this);
    effect->setColor(QColor(12, 155, 246, 255 * 0.2));
    effect->setXOffset(0);
    effect->setYOffset(4);
    effect->setBlurRadius(12);

    if (text == "＝") {
        effect->setColor(QColor(12, 155, 246, 255 * 0.8));
        effect->setBlurRadius(20);
    }

    effect->setEnabled(false);
    setGraphicsEffect(effect);

    setText(text);
    setFixedSize(80, 60);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    hideShadowEffect();

    QPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    showShadowEffect();

    QPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    showShadowEffect();

    QPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    hideShadowEffect();

    QPushButton::leaveEvent(e);
}

void TextButton::showShadowEffect()
{
    effect->setEnabled(true);
    raise();
}

void TextButton::hideShadowEffect()
{
    effect->setEnabled(false);
}
