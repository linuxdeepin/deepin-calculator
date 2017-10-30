#include "listitem.h"

ListItem::ListItem()
{
}

ListItem::~ListItem()
{
}

void ListItem::drawBackground(const QRect &rect, QPainter *painter)
{
    QPainterPath path;
    path.addRect(rect);

    painter->fillPath(path, QColor("#FFFFFF"));
}

void ListItem::drawContent(const QRect &rect, QPainter *painter)
{
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, "111+222+87331283*12+2323=3488984897");
}
