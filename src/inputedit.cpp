#include "inputedit.h"
#include "utils.h"
#include "math/floatconfig.h"
#include <QKeyEvent>
#include <QCoreApplication>

#define NOT_USED(x) ( (void)(x) )

InputEdit::InputEdit(QLineEdit *parent) : QLineEdit(parent)
{
    m_ans = 0;
    m_ans_start = 0;
    m_ans_length = 0;
    m_ans_vaild = false;
    m_cur_in_ans = false;
    m_cur_on_ans_left = false;
    m_old_text = "";

    setAttribute(Qt::WA_InputMethodEnabled, false);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &QLineEdit::textChanged, [=](const QString & text){
        if (m_cur_in_ans) {
            m_ans_length = 0;
        } else if (m_cur_on_ans_left && m_old_text.length() != 0) {
            int nl = text.length();
            int ol = m_old_text.length();
            int l = std::min(nl, ol);
            int i = 1;
            for (; i <= l && text[nl - i] == m_old_text[ol - i]; i++) {}
            int diff = (nl - i) - (ol - i);
            m_ans_start += diff;
        }
        int ans_end = m_ans_start + m_ans_length;
        m_ans_vaild = m_ans_length != 0 &&
                       (m_ans_start == 0 || !text[m_ans_start - 1].isDigit()) &&
                       (ans_end == text.length() || !text[ans_end].isDigit());
        this->heilightAns(m_ans_start, m_ans_vaild ? m_ans_length : 0);
        m_old_text = text;
    });
    connect(this, &QLineEdit::selectionChanged, [=](){
        int pos = this->cursorPosition();
        this->cursorPositionChanged(pos, pos);
    });
    connect(this, &QLineEdit::cursorPositionChanged, [=](int old_pos, int new_pos){
        NOT_USED(old_pos);
        int ans_end = m_ans_start + m_ans_length;
        int sel_start = this->selectionStart();
        int sel_end = sel_start + this->selectedText().length();
        if (new_pos > m_ans_start && new_pos < ans_end) {
            m_cur_in_ans = true;
        } else if (this->hasSelectedText() &&
                   ((sel_start >= m_ans_start && sel_start < ans_end) ||
                    (sel_end > m_ans_start && sel_end <= ans_end) ||
                    (sel_start < m_ans_start && sel_end > ans_end))) {
            m_cur_in_ans = true;
        } else if (new_pos <= m_ans_start) {
            m_cur_in_ans = false;
            m_cur_on_ans_left = true;
        } else {
            m_cur_in_ans = false;
            m_cur_on_ans_left = false;
        }
    });
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

    case Qt::Key_Backspace:
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

    case Qt::Key_Left: case Qt::Key_Right: case Qt::Key_Home: case Qt::Key_End: case Qt::Key_Delete:
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

void InputEdit::clear()
{
    m_ans_length = 0;
    setText("");
}

void InputEdit::setAnswer(QString & str, const Quantity & ans)
{
    m_ans = ans;
    m_ans_start = 0;
    m_ans_length = str.length();
    m_old_text = "";
    setText(str);
}

void InputEdit::heilightAns(int start, int length)
{
    QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
    start = start - cursorPosition();
    QTextCharFormat f;
    f.setFontWeight(QFont::Bold);
    QList<QInputMethodEvent::Attribute> attributes;
    attributes.append(QInputMethodEvent::Attribute(type, start, length, f));

    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(this, &event);
}

QString InputEdit::expressionText()
{
    QString r = text();
    if (m_ans_vaild) {
        QString ans = DMath::format(m_ans, Quantity::Format::Precision(DECPRECISION));
        r.remove(m_ans_start, m_ans_length);
        r.insert(m_ans_start, ans);
    }
    return r;
}
