#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    addNextLine("0");
    setFixedHeight(175);
    scrollToBottom();
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::addNextLine(const QString &str)
{
    ListItem *item = new ListItem;
    item->insert(str);
    listItems << item;

    addItem(item);
}
