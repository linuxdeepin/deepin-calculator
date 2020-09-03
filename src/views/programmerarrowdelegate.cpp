#include "programmerarrowdelegate.h"

ProgrammerArrowDelegate::ProgrammerArrowDelegate(DListWidget *parent)
    : QStyledItemDelegate(parent)
{

}

void ProgrammerArrowDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

void ProgrammerArrowDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
