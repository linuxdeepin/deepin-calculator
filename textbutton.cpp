#include "textbutton.h"

TextButton::TextButton(const QString &text)
{
    setText(text);
    setFixedHeight(65);
    setFocusPolicy(Qt::NoFocus);
}

TextButton::~TextButton()
{
}
