#include "textbutton.h"

TextButton::TextButton(const QString &text)
{
    setText(text);
    setFixedHeight(55);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}
