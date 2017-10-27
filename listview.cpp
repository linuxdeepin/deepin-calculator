#include "listview.h"
#include <QPainter>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 40;
    padding = 10;

    setFixedHeight(175);
}

ListView::~ListView()
{
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#D6D6D6"));
    painter.drawRect(rect());

    painter.setPen("#333333");
    
    for (int i = 0; i < 5; ++i) {
        QPainterPath itemPath;
        itemPath.addRect(QRect(0, i * rowHeight, width(), rowHeight));
        
        painter.fillPath(itemPath, QColor("#FFFFFF"));
        painter.drawText(QRect(padding, i * rowHeight, width() - padding * 2, rowHeight), Qt::AlignVCenter | Qt::AlignRight, "1+1+2+3*(2-3)+1+1+1+1+1+1+1+1+1+1+1+1+1");
    }
}
