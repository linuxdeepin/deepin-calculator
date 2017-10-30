#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    addItem(new ListItem);

    setFixedHeight(175);
}

DisplayArea::~DisplayArea()
{
}
