#include "listitem.h"

ListItem::ListItem()
{
    expression = "0";
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
    QFont font;
    font.setPointSize(25);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, expression);
}

void ListItem::insert(const QString &str)
{
    expression.append(str);
}