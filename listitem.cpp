#include "listitem.h"

ListItem::ListItem()
{
    expression = "0";
    result = "0";
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

void ListItem::drawContent(const QRect &rect, QPainter *painter, bool isLast)
{
    QFont font;

    if (isLast) {
        painter->setPen(QColor("#3A3A3A"));
        font.setPointSize(20);
    } else {
        painter->setPen(QColor("#636363"));
        font.setPointSize(14);
    }

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, expression);
}
