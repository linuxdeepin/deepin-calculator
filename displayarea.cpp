#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    addItem(new ListItem);

    setFixedHeight(165);
}

DisplayArea::~DisplayArea()
{
}
