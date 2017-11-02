#include "displayarea.h"
#include "listitem.h"
#include "algorithm.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    isLeftBracket = true;
    isContinue = true;
    isAllClear = false;

    setFixedHeight(175);
    addNewRow();
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::addNewRow()
{
    ListItem *item = new ListItem;
    addItem(item);

    scrollToBottom();
}

void DisplayArea::enterNumberEvent(const QString &num)
{
    if (isEnding())
        return;

    if (listItems.last()->expression == "0") {
        listItems.last()->expression = nullptr;
    }

    if (!isContinue) {
        listItems.last()->expression = nullptr;

        isContinue = true;
    }

    listItems.last()->expression.append(num);
    scrollToBottom();
}

void DisplayArea::enterPointEvent()
{
    if (isEnding())
        return;

    const QChar lastChar = getLastChar();
    if (lastChar != '.') {
        listItems.last()->expression.append(".");
        isContinue = true;
    }

    scrollToBottom();
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    if (isEnding())
        return;

    isContinue = true;

    const QChar lastChar = getLastChar();
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/' || lastChar == '%') {
        enterBackspaceEvent();
    }

    listItems.last()->expression.append(str);

    scrollToBottom();
}

void DisplayArea::enterBracketsEvent()
{
    if (isEnding())
        return;

    if (!isContinue) {
        listItems.last()->expression = nullptr;
        isContinue = true;
    }

    if (isLeftBracket) {
        listItems.last()->expression.append("(");
        isLeftBracket = false;
    }else {
        listItems.last()->expression.append(")");
        isLeftBracket = true;
    }

    scrollToBottom();
}

void DisplayArea::enterBackspaceEvent()
{
    const QString exp = listItems.last()->expression;
    const QChar lastChar = getLastChar();

    if (exp.length() == 1) {
        listItems.last()->expression = "0";
    }else {
        if (lastChar == '(')
            isLeftBracket = true;

        listItems.last()->expression = exp.left(exp.length() - 1);
    }

    isContinue = true;

    scrollToBottom();
}

void DisplayArea::enterClearEvent()
{   
    if (isAllClear) {
        clearAllItems();
        addNewRow();
    } else {
        listItems.last()->expression = "0";
    }

    isLeftBracket = true;
    scrollToBottom();
}

void DisplayArea::enterEqualEvent()
{
    if (listItems.last()->expression != "0") {
        QString exp = listItems.last()->expression;
        const double result = Algorithm::getResult(exp.replace("×", "*").replace("÷", "/").toStdString());
        listItems.last()->expression.append(" = " + QString::number(result));

        ListItem *item = new ListItem;
        listItems << item;

        listItems.last()->expression = QString::number(result);

        isContinue = false;
        isLeftBracket = true;
    }

    scrollToBottom();
}

void DisplayArea::copyResultToClipboard()
{
    QApplication::clipboard()->setText(listItems.last()->expression);
}

QChar DisplayArea::getLastChar()
{
    QString exp = listItems.last()->expression;
    QString::const_iterator laster = exp.replace("×", "*").replace("÷", "/").end();
    laster--;

    return *laster;
}

bool DisplayArea::isEnding()
{
    return listItems.last()->expression.count() > 35 ? true : false;
}