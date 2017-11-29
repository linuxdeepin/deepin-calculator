#include "inputedit.h"
#include <QKeyEvent>
#include <QDebug>

InputEdit::InputEdit(QLineEdit *parent) : QLineEdit(parent)
{
    setAttribute(Qt::WA_InputMethodEnabled, false);
}

InputEdit::~InputEdit()
{
}

void InputEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    // QString exp = this->text();
    // QString seg;
    // QStringList expList;

    // for (int i = 0; i < exp.count(); ++i) {
    //     const QChar ch = exp.at(i);

    //     if (ch.isDigit() || ch == '.' || ch == ',') {
    //         seg.append(ch);
    //     } else {
    //         expList << seg;
    //         seg.clear();
    //         seg.append(ch);
    //         expList << seg;
    //         seg.clear();
    //     }

    //     if (i == exp.count() - 1) {
    //         if (!seg.isEmpty()) {
    //             expList << seg;
    //         }
    //     }
    // }

    // qDebug() << expList;

    QLineEdit::mouseDoubleClickEvent(e);
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;

    switch (e->key())
    {
    case Qt::Key_0: case Qt::Key_1: case Qt::Key_2: case Qt::Key_3:
    case Qt::Key_4: case Qt::Key_5: case Qt::Key_6: case Qt::Key_7:
    case Qt::Key_8: case Qt::Key_9:
        emit inputKeyPressEvent(e);
        QLineEdit::keyPressEvent(e);
        break;

    case Qt::Key_Plus: case Qt::Key_Minus:
    case Qt::Key_Underscore: case Qt::Key_Asterisk: case Qt::Key_Slash:
    case Qt::Key_Period: case Qt::Key_Percent: case Qt::Key_Equal:
    case Qt::Key_Escape: case Qt::Key_Enter: case Qt::Key_Return:
        emit inputKeyPressEvent(e);
        break;

    case Qt::Key_ParenLeft: case Qt::Key_ParenRight:
        emit inputKeyPressEvent(e);
        QLineEdit::keyPressEvent(e);
        break;

    case Qt::Key_Backspace: case Qt::Key_Delete:
        emit inputKeyPressEvent(e);
        backspace();
        break;

    case Qt::Key_A:
        if (isPressCtrl) {
            QLineEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_C:
        if (isPressCtrl) {
            if (!hasSelectedText()) {
                emit inputKeyPressEvent(e);
                return;
            } else {
                QLineEdit::keyPressEvent(e);
            }
        }
        break;

    case Qt::Key_V:
        if (isPressCtrl) {
            QLineEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_X:
        if (isPressCtrl) {
            QLineEdit::keyPressEvent(e);
        } else {
            emit inputKeyPressEvent(e);
        }
        break;

    case Qt::Key_Z:
        if (isPressCtrl) {
            QLineEdit::keyPressEvent(e);
        }
        break;

    case Qt::Key_Left: case Qt::Key_Right: case Qt::Key_Home: case Qt::Key_End:
        QLineEdit::keyPressEvent(e);
        break;
    }

}
