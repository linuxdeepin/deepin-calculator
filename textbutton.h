#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <QPushButton>

class TextButton : public QPushButton
{
    Q_OBJECT

public:
    TextButton(const QString &text);
    ~TextButton(); 
};

#endif











