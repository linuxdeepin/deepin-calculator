#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include "dthememanager.h"
#include "expressionlist.h"
#include "utils.h"
#include "abacus/Expression.h"
#include <QDebug>
#include <QKeyEvent>

DWIDGET_USE_NAMESPACE

ExpressionList::ExpressionList(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    listView = new ListView;
    inputEdit = new InputEdit;

    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(listView);
    layout->addWidget(inputEdit);

    inputEdit->setTextMargins(10, 0, 10, 8);
    inputEdit->setFixedHeight(55);
    inputEdit->setAlignment(Qt::AlignRight);

    defaultFontSize = 25;
    minFontSize = 10;
    fontSize = defaultFontSize;
    isLeftBracket = true;
    isContinue = true;
    isAllClear = false;

    setFixedHeight(160);
    initFontSize();

    connect(inputEdit, &InputEdit::textChanged, this, &ExpressionList::inputEditChanged);
    connect(inputEdit, &InputEdit::inputKeyPressEvent, this, &ExpressionList::inputKeyPressEvent);
}

ExpressionList::~ExpressionList()
{
}

void ExpressionList::setContinue(const bool &mark)
{
    isContinue = mark;
}

void ExpressionList::enterNumberEvent(const QString &num)
{
    if (!isContinue) {
        inputEdit->setText("");
        isContinue = true;
    }

    inputEdit->insert(num);
    isAllClear = false;

    emit clearStateChanged(false);
}

void ExpressionList::enterPointEvent()
{
    inputEdit->insert(".");
}

void ExpressionList::enterSymbolEvent(const QString &str)
{
    inputEdit->insert(str);
    isContinue = true;
}

void ExpressionList::enterBracketsEvent()
{
    if (isLeftBracket) {
        inputEdit->insert("(");
        isLeftBracket = false;
    } else {
        inputEdit->insert(")");
        isLeftBracket = true;
    }

    isContinue = true;
    isAllClear = false;
}

void ExpressionList::enterBackspaceEvent()
{
    inputEdit->backspace();

    isContinue = true;
    isAllClear = false;
}

void ExpressionList::enterClearEvent()
{
    if (isAllClear) {
        listView->clearItems();
        isAllClear = false;

        emit clearStateChanged(false);
    } else {
        inputEdit->setText("");
        isAllClear = true;

        initFontSize();
        emit clearStateChanged(true);
    }

    isLeftBracket = true;
}

void ExpressionList::enterEqualEvent()
{
    if (inputEdit->text() == "0" || inputEdit->text().isEmpty() || !isContinue || lastCharIsLeftBracket() || lastCharIsPoint()) {
        return;
    }

    Expression e(formatExp(inputEdit->text()).toStdString(), 10);

    try {
        const QString result = QString::number(e.getResult()).replace("-", "－");

        if (inputEdit->text() == result) {
            return;
        }

        listView->addItem(inputEdit->text() + " ＝ " + result);
        inputEdit->setText(result);

        isContinue = false;
    } catch (runtime_error err) {
        qDebug() << err.what();
    }
}

void ExpressionList::copyResultToClipboard()
{
    Expression e(formatExp(inputEdit->text()).toStdString(), 10);

    try {
        QApplication::clipboard()->setText(QString::number(e.getResult()));
    } catch (runtime_error err) {

    }
}

int ExpressionList::getItemsCount()
{
    return listView->getItemsCount();
}

void ExpressionList::inputEditChanged(const QString &text)
{
    // using setText() will move the cursor pos to end.
    const int currentPos = inputEdit->cursorPosition();

    // replace string.
    inputEdit->setText(QString(text).replace("+", "＋").replace("-", "－")
                                    .replace(QRegExp("[x|X|*]"), "×").replace("/", "÷")
                                    .replace("（", "(").replace("）", ")")
                                    .replace("。", ".").replace("——", "－"));
    inputEdit->setCursorPosition(currentPos);

    // // make font size of inputEdit fit text content.
    // QFontMetrics fm = inputEdit->fontMetrics();
    // int w = fm.boundingRect(inputEdit->text()).width();

    // if (w >= inputEdit->width() - 30) {
    //     fontSize -= 2;
    //     QFont font;
    //     font.setPointSize(qMax(fontSize, minFontSize));
    //     inputEdit->setFont(font);
    // }

    // when text is empty, the clear button status will changed.
    // if (text.isEmpty()) {
    //     isAllClear = true;
    //     initFontSize();
    //     emit clearStateChanged(true);
    // } else {
    //     isAllClear = false;
    //     emit clearStateChanged(false);
    // }

    isAllClear = false;
    emit clearStateChanged(false);
}

void ExpressionList::initFontSize()
{
    fontSize = defaultFontSize;
    QFont font;
    font.setPointSize(fontSize);
    inputEdit->setFont(font);
}

QString ExpressionList::formatExp(const QString &exp)
{
    return QString(exp).replace("＋", "+").replace("－", "-").replace("×", "*").replace("÷", "/");
}

QChar ExpressionList::getLastChar()
{
    QString exp = formatExp(inputEdit->text());
    QString::const_iterator laster = exp.end();
    laster--;

    return *laster;
}

bool ExpressionList::lastCharIsNumber()
{
    const QChar lastChar = getLastChar();

    if (lastChar == '0' || lastChar == '1' || lastChar == '2' || lastChar == '3' ||
        lastChar == '4' || lastChar == '5' || lastChar == '6' || lastChar == '7' ||
        lastChar == '8' || lastChar == '9') {
        return true;
    } else {
        return false;
    }
}

bool ExpressionList::lastCharIsSymbol()
{
    const QChar lastChar = getLastChar();

    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/') {
        return true;
    } else {
        return false;
    }
}

bool ExpressionList::lastCharIsPoint()
{
    return getLastChar() == '.' ? true : false;
}

bool ExpressionList::lastCharIsLeftBracket()
{
    return getLastChar() == '(' ? true : false;
}

bool ExpressionList::lastCharIsRightBracket()
{
    return getLastChar() == ')' ? true : false;
}
