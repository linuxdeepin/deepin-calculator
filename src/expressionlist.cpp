#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include "expressionlist.h"
#include "utils.h"

ExpressionList::ExpressionList(QWidget *parent) : QWidget(parent)
{
    m_eval = Evaluator::instance();
    m_layout = new QVBoxLayout(this);
    m_listView = new ListView;
    m_inputEdit = new InputEdit;

    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_listView);
    m_layout->addWidget(m_inputEdit);

    m_inputEdit->setTextMargins(10, 0, 10, 8);
    m_inputEdit->setFixedHeight(55);
    m_inputEdit->setAlignment(Qt::AlignRight);

    m_isContinue = true;
    m_isAllClear = false;

    setMinimumHeight(160);
    autoZoomFontSize();

    connect(m_inputEdit, &InputEdit::textChanged, this, &ExpressionList::textChanged);
    connect(m_inputEdit, &InputEdit::inputKeyPressEvent, this, &ExpressionList::inputKeyPressEvent);
}

ExpressionList::~ExpressionList()
{
    delete m_listView;
    delete m_inputEdit;
}

void ExpressionList::setContinue(const bool &mark)
{
    m_isContinue = mark;
}

QString ExpressionList::getInputEditText() const
{
    return m_inputEdit->text();
}

void ExpressionList::enterNumberEvent(const QString &num, bool isKeyPress)
{
    if (!m_isContinue) {
        // the cursor position at the end, it will clear edit text.
        if (cursorPosAtEnd()) {
            m_inputEdit->clear();
        }

        m_isContinue = true;
    }

    if (!isKeyPress) {
        m_inputEdit->insert(num);
    }

    m_isAllClear = false;

    emit clearStateChanged(false);
}

void ExpressionList::enterPointEvent()
{
    m_inputEdit->insert(".");
}

void ExpressionList::enterSymbolEvent(const QString &str)
{
    m_inputEdit->insert(str);
    m_isContinue = true;
}

void ExpressionList::enterBracketsEvent()
{
    const int currentPos = m_inputEdit->cursorPosition();
    m_inputEdit->insert("()");
    m_inputEdit->setCursorPosition(currentPos + 1);

    m_isAllClear = false;
    m_isContinue = true;
}

void ExpressionList::enterBackspaceEvent()
{
    m_inputEdit->backspace();

    m_isContinue = true;
    m_isAllClear = false;
}

void ExpressionList::enterClearEvent()
{
    if (m_isAllClear) {
        m_listView->clearItems();
        m_isAllClear = false;

        emit clearStateChanged(false);
    } else {
        m_inputEdit->setText("");
        m_isAllClear = true;

        emit clearStateChanged(true);
    }
}

void ExpressionList::enterEqualEvent()
{
    if (m_inputEdit->text().isEmpty())
        return;

    QString str = m_eval->autoFix(formatExp(m_inputEdit->text()));
    m_eval->setExpression(str);
    auto quantity = m_eval->evalUpdateAns();

    if (m_eval->error().isEmpty()) {
        if (!quantity.isNan() && !m_eval->isUserFunctionAssign()) {
            const QString result = DMath::format(m_eval->evalUpdateAns(), Quantity::Format::Fixed());
            QString formatResult = Utils::formatThousandsSeparators(result);
            formatResult.replace("-", "－");

            if (formatResult == m_inputEdit->text()) {
                return;
            }

            m_listView->addItem(m_inputEdit->text() + " ＝ " + formatResult);
            m_inputEdit->setText(formatResult);
            m_isContinue = false;
        }
    } else {
        m_listView->addItem(m_inputEdit->text() + " ＝ " + tr("Expression Error"));
    }
}

void ExpressionList::copyResultToClipboard()
{
    const auto str = m_eval->autoFix(formatExp(m_inputEdit->text()));
    m_eval->setExpression(str);

    if (!m_eval->error().isEmpty()) {
        QApplication::clipboard()->setText(m_inputEdit->text());
    } else {
        const QString result = DMath::format(m_eval->evalUpdateAns(), Quantity::Format::Fixed());
        QApplication::clipboard()->setText(result);
    }
}

int ExpressionList::getItemsCount()
{
    return m_listView->getItemsCount();
}

void ExpressionList::textChanged(const QString &text)
{
    const int oldPos = m_inputEdit->cursorPosition();
    const QString exp = QString(text).replace("（", "(").replace("）", ")")
                                     .replace(",", "").replace("××", "^");
    const QString reformatExp = Utils::reformatSeparators(exp);
    m_inputEdit->setText(reformatExp);
    const int newPos = m_inputEdit->cursorPosition();
    m_inputEdit->setCursorPosition(oldPos + (newPos - text.count()));

    // make font size of inputEdit fit text content.
    autoZoomFontSize();

    m_isAllClear = false;
    m_isContinue = true;

    emit clearStateChanged(false);
}

void ExpressionList::autoZoomFontSize()
{
    QFont font;
    for (int i = 28; i > 8; --i) {
        font.setPointSize(i);
        QFontMetrics fm(font);
        int fontWidth = fm.width(m_inputEdit->text());
        int editWidth = m_inputEdit->width() - 30;

        if (fontWidth < editWidth) {
            break;
        }
    }

    m_inputEdit->setFont(font);
}

QString ExpressionList::formatExp(const QString &exp)
{
    return QString(exp).replace("＋", "+")
                       .replace("－", "-")
                       .replace("×", "*")
                       .replace("÷", "/")
                       .replace(",", "");
}

bool ExpressionList::cursorPosAtEnd()
{
    return m_inputEdit->cursorPosition() == m_inputEdit->text().count();
}
