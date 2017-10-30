#include "listitem.h"

ListItem::ListItem()
{
    expression = nullptr;
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
    font.setPointSize(13);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, "1211+22+871283*12+2323=388982247");
}