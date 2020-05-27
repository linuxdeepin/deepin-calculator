#include "memoryitemdelegate.h"

#include <DStyleHelper>
#include <DApplicationHelper>

MemoryItemDelegate::MemoryItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void MemoryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    MemoryListWidget *dragWidget = qobject_cast<MemoryListWidget *>(option.styleObject);
    QRectF rect = dragWidget->rect();

    painter->setRenderHint(QPainter::Antialiasing, true);
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#F8F8F8"));
        painter->drawRect(rect);
    } else {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor("#252525"));
        painter->drawRect(rect);
    }
    QStyledItemDelegate::paint(painter, option, index);
}

void MemoryItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
