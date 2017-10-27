#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    ListItem *item = new ListItem;
    ListItem *item2 = new ListItem;
    ListItem *item3 = new ListItem;
    ListItem *item4 = new ListItem;
    ListItem *item5 = new ListItem;
    
    addItem(item);
    addItem(item2);
    addItem(item3);
    addItem(item4);
    addItem(item5);
}

DisplayArea::~DisplayArea()
{
}
