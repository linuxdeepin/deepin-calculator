#include "textbutton.h"

TextButton::TextButton(const QString &text)
{
    setText(text);
    setFixedHeight(64);
    setFocusPolicy(Qt::NoFocus);
}

TextButton::~TextButton()
{
}
