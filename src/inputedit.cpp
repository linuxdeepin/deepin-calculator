#include "inputedit.h"
#include <QKeyEvent>
#include <QDebug>

InputEdit::InputEdit(QLineEdit *parent) : QLineEdit(parent)
{
    
}

InputEdit::~InputEdit()
{
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Backspace) {
        qDebug() << "hello";
    }

    QLineEdit::keyPressEvent(e);
}
