#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    for (int i = 0; i < 20; ++i) {
        addItem(new ListItem);
    }
}

DisplayArea::~DisplayArea()
{
}
