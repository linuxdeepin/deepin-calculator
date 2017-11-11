#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include "dthememanager.h"
#include "expressionlist.h"
#include "utils.h"
#include "abacus/Expression.h"
#include <QDebug>

DWIDGET_USE_NAMESPACE

ExpressionList::ExpressionList(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    listView = new ListView;
    inputEdit = new QLineEdit;

    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(listView);
    layout->addWidget(inputEdit);

    inputEdit->setFixedHeight(55);
    inputEdit->setAlignment(Qt::AlignRight);
    inputEdit->setReadOnly(true);
    inputEdit->setFocusPolicy(Qt::NoFocus);
    inputEdit->setEnabled(false);
    inputEdit->setText("0");

    isLeftBracket = true;
    isContinue = true;
    isAllClear = false;

    setFixedHeight(160);
}

ExpressionList::~ExpressionList()
{
}

void ExpressionList::enterNumberEvent(const QString &num)
{
    if (!isContinue || inputEdit->text() == "0") {
        inputEdit->setText("");
        isContinue = true;
    }

    inputEdit->insert(num);
    isAllClear = false;
}

void ExpressionList::enterPointEvent()
{
    if (lastCharIsSymbol()) {
        inputEdit->insert("0");
    }

    inputEdit->insert(".");
    isContinue = true;
}

void ExpressionList::enterSymbolEvent(const QString &str)
{
    if (lastCharIsSymbol()) {
        enterBackspaceEvent();
    } else if (lastCharIsPoint() || lastCharIsLeftBracket()) {
        inputEdit->insert("0");
    } else if (str == "－" && inputEdit->text() == "0") {
        inputEdit->clear();
    }

    inputEdit->insert(str);
    isContinue = true;
    isAllClear = false;
}

void ExpressionList::enterBracketsEvent()
{
}

void ExpressionList::enterBackspaceEvent()
{
    if (inputEdit->text() == "0") {
        return;
    }

    inputEdit->backspace();
}

void ExpressionList::enterClearEvent()
{
    if (isAllClear) {
        listView->clearItems();
        isAllClear = false;
    } else {
        inputEdit->setText("0");
        isAllClear = true;
    }
}

void ExpressionList::enterEqualEvent()
{
    if (inputEdit->text() == "0" || !isContinue || lastCharIsLeftBracket() || lastCharIsPoint()) {
        return;
    }

    Expression e(formatExp(inputEdit->text()).toStdString(), 10);

    try {
        const QString result = QString::number(e.getResult());

        if (inputEdit->text() == result) {
            return;
        }

        listView->addItem(inputEdit->text() + " = " + result);
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

QString ExpressionList::getResult()
{   

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
