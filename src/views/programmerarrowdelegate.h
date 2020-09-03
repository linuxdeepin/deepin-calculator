#ifndef PROGRAMMERARROWDELEGATE_H
#define PROGRAMMERARROWDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QDebug>
#include <DListWidget>

DWIDGET_USE_NAMESPACE

class ProgrammerArrowDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ProgrammerArrowDelegate(DListWidget *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PROGRAMMERARROWDELEGATE_H
