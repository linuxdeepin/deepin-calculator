#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    addNextLine("0");
    setScrollToBottom();
    setFixedHeight(175);
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::addNextLine(const QString &str)
{
    ListItem *item = new ListItem;
    item->insert(str);

    addItem(item);
}
