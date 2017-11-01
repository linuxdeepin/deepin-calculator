#include "displayarea.h"
#include "listitem.h"
#include "algorithm.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    isLeftBracket = true;

    ListItem *item = new ListItem;
    addItem(item);

    scrollToBottom();
    setFixedHeight(175);
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::enterNumberEvent(const QString &num)
{
    if (listItems.last()->expression == "0") {
        listItems.last()->expression = nullptr;
    }

    listItems.last()->expression.append(num);

    scrollToBottom();
}

void DisplayArea::enterPointEvent()
{
    listItems.last()->expression.append(".");

    scrollToBottom();
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    listItems.last()->expression.append(str);

    scrollToBottom();
}

void DisplayArea::enterBracketsEvent()
{
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

    if (exp.length() == 1) {
        listItems.last()->expression = "0";
    }else {
        listItems.last()->expression = exp.left(exp.length() - 1);
    }

    scrollToBottom();
}

void DisplayArea::enterClearEvent()
{
    listItems.last()->expression = "0";

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
    }

    scrollToBottom();
}
