#include "listview.h"
#include <QPainter>

ListView::ListView(QWidget *parent) : QWidget(parent)
{

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
}
