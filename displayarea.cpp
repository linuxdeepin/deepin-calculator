#include "displayarea.h"
#include "listitem.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    for (int i = 0; i < 10; ++i) {
        addItem(new ListItem);
    }

    setFixedHeight(165);
}

DisplayArea::~DisplayArea()
{
}
