#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    isLeftBracket = true;

    ListItem *item = new ListItem;
    addItem(item);

    setScrollToBottom();
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

    setScrollToBottom();
}

void DisplayArea::enterPointEvent()
{
    listItems.last()->expression.append(".");

    setScrollToBottom();
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    listItems.last()->expression.append(str);

    setScrollToBottom();
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

    setScrollToBottom();
}

void DisplayArea::enterBackspaceEvent()
{
    const QString exp = listItems.last()->expression;

    if (exp.length() == 1) {
        listItems.last()->expression = "0";
    }else {
        listItems.last()->expression = exp.left(exp.length() - 1);
    }

    setScrollToBottom();
}

void DisplayArea::enterClearEvent()
{
    listItems.last()->expression = "0";

    setScrollToBottom();
}

void DisplayArea::enterEqualEvent()
{
    ListItem *item = new ListItem;
    listItems << item;

    setScrollToBottom();
}
