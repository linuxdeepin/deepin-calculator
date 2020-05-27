#ifndef TESTITEMDELEGATE_H
#define TESTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

#include "memorylistwidget.h"
#include "memoryitemwidget.h"

class MemoryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MemoryItemDelegate(QObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QAbstractItemView *m_parentView;
};

#endif // TESTITEMDELEGATE_H
