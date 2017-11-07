#include "separator.h"
#include <QPainter>

Separator::Separator(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(5);
}

Separator::~Separator()
{
}

void Separator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#FBFBFB"));
    painter.drawRect(rect());
}
