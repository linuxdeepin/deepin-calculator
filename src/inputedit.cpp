#include "inputedit.h"
#include <QKeyEvent>
#include <QDebug>
#include <QFontMetrics>

InputEdit::InputEdit(QLineEdit *parent) : QLineEdit(parent)
{

}

InputEdit::~InputEdit()
{
}

void InputEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        const QString text = this->text();
        QFontMetrics fm(this->fontMetrics());

        int cursorPos = cursorPositionAt(QPoint(e->x(), e->y()));  // or cursorPosition()
        QStringList list = text.split("[1-9]");

        qDebug() << list;
    }
    // setSelection(0 , 1);

    // QLineEdit::mouseDoubleClickEvent(e);
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3:
    case Qt::Key_4: case Qt::Key_5: case Qt::Key_6: case Qt::Key_7:
    case Qt::Key_8: case Qt::Key_9: case Qt::Key_Plus: case Qt::Key_Minus:
    case Qt::Key_Underscore: case Qt::Key_Asterisk: case Qt::Key_X: case Qt::Key_Slash:
    case Qt::Key_Backspace: case Qt::Key_Period: case Qt::Key_Percent:
        emit inputKeyPressEvent(e);
        return;
        break;

    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        emit inputKeyPressEvent(e);
        break;

    case Qt::Key_C:
        if (e->modifiers() == Qt::ControlModifier) {
            emit inputKeyPressEvent(e);
            return;
        }
        break;
    }

    QLineEdit::keyPressEvent(e);
}
