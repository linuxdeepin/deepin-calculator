#include "textbutton.h"

TextButton::TextButton(const QString &text)
{
    setText(text);
    setFixedHeight(60);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
}

TextButton::~TextButton()
{
}
