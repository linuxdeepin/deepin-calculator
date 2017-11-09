#include "listitem.h"
#include <QFontMetrics>

ListItem::ListItem()
{
    expression = "0";
    result = "0";
    fontSize = 20;
}

ListItem::~ListItem()
{
}

void ListItem::drawBackground(const QRect &rect, QPainter *painter, QString color)
{
    QPainterPath path;
    path.addRect(rect);

    painter->fillPath(path, QColor(color));
}

void ListItem::drawContent(const QRect &rect, QPainter *painter, QString fontColor, QString lastFontColor, bool isLast)
{
    QFont font;
    font.setWeight(300);

    if (isLast) {
        painter->setPen(QColor(lastFontColor));
        font.setPointSize(fontSize);

        QFontMetrics fontMetrics(font);
        int fontWidth = fontMetrics.width(expression);

        if (fontWidth > rect.width()) {
            fontSize -= 2;
            font.setPointSize(fontSize);
        }
    } else {
        painter->setPen(QColor(fontColor));
    }

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignRight, expression);
}

void ListItem::initFontSize()
{
    fontSize = 20;
}
