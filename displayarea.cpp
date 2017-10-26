#include "displayarea.h"
#include <QPainter>

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(150);
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QColor("#000000"));
    painter.drawRect(rect());
}
