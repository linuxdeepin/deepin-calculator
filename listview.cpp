#include "listview.h"
#include <QPainter>
#include <QDebug>

ListView::ListView(QWidget *parent) : QWidget(parent)
{
    rowHeight = 41;
    padding = 10;
}

ListView::~ListView()
{
}

void ListView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#E7E7E7"));
    painter.drawRect(rect());

    painter.setPen("#333333");

    QFont font;

    for (int i = 0; i < 10; ++i) {
        QPainterPath itemPath;
        itemPath.addRect(QRect(0, i * rowHeight, width(), rowHeight));

        QString express("1234+12123-22+21312*211-221/222");
        
        if (i == 10) {
            font.setPointSize(15);
            painter.setFont(font);
            express = "23432+2343-213";
        }

        painter.fillPath(itemPath, QColor("#FFFFFF"));
        painter.drawText(QRect(padding, i * rowHeight, width() - padding * 2, rowHeight), Qt::AlignVCenter | Qt::AlignRight, express);
    }
}
