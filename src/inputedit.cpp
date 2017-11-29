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
    QLineEdit::mouseDoubleClickEvent(e);

    if (e->button() == Qt::LeftButton) {
        int position = cursorPositionAt(e->pos());
        int posBegin = findWordBeginPosition(position);
        int posEnd = findWordEndPosition(position);
        setSelection(posBegin, posEnd - posBegin + 1);
    }
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    const bool isPressCtrl = e->modifiers() == Qt::ControlModifier;

    switch (e->key()) {
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

bool InputEdit::isSymbolCategoryChanged(int pos1, int pos2)
{
    QString str = text();
    QChar::Category category1 = str.at(pos1).category();
    QChar::Category category2 = str.at(pos2).category();

    if (category1 == QChar::Number_DecimalDigit || category1 == QChar::Punctuation_Other) {
        if (category2 == QChar::Number_DecimalDigit || category2 == QChar::Punctuation_Other) {
            return false ;
        }
    }

    return true;
}

int InputEdit::findWordBeginPosition(int pos)
{
    QString str = text();

    if (0 >= pos) {
        return 0;
    }

    while (pos > 0) {
        pos--;
        if (isSymbolCategoryChanged(pos, pos + 1)) {
            return pos + 1;
        }
    }
    return 0;

}

int InputEdit::findWordEndPosition(int pos)
{
    QString str = text();
    if (pos >= str.length()) {
        return str.length() - 1;
    }
    while (pos < str.length() - 1) {
        pos++;
        if (isSymbolCategoryChanged(pos, pos - 1)) {
            return pos - 1;
        }
    }
    return str.length() - 1;
}
