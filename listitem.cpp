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
    font.setPointSize(20);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, expression);
}

void ListItem::insert(const QString &str)
{
    if (expression == "0")
        expression.clear();

    expression.append(str);
}

void ListItem::backspace()
{
    expression = expression.left(expression.count() - 1);
}
