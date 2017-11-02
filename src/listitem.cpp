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
        if (expression.length() < 15)
            font.setPointSize(20);
        else if (expression.length() >= 15 && expression.length() < 20)
            font.setPointSize(17);
        else if (expression.length() >= 20 && expression.length() < 25)
            font.setPointSize(15);
        else if (expression.length() >= 25 && expression.length() < 30)
            font.setPointSize(13);
        else
            font.setPointSize(11);
    } else {
        painter->setPen(QColor("#636363"));
        font.setPointSize(12);
    }

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, expression);
}
