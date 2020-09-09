#ifndef PROLISTVIEW_H
#define PROLISTVIEW_H

#include <DWidget>
#include <DListView>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QMouseEvent>

#include "prolistmodel.h"

DWIDGET_USE_NAMESPACE

class ProListView : public DListView
{
    Q_OBJECT

public:
    explicit ProListView(DWidget *parent = nullptr);
    ~ProListView();
    void contextMenuEvent(QContextMenuEvent *event);
    void showTextEditMenuByAltM(const QModelIndex &index);

protected:
//    void mouseMoveEvent(QMouseEvent *);
//    void mouseClickEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);
//    void focusInEvent(QFocusEvent *event);
Q_SIGNALS:
    void obtainingHistorical(const QModelIndex &index);
};

#endif // PROLISTVIEW_H
