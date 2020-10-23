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
#include "prolistdelegate.h"

DWIDGET_USE_NAMESPACE

class ProListView : public DListView
{
    Q_OBJECT

public:
    explicit ProListView(DWidget *parent = nullptr);
    ~ProListView();
    void contextMenuEvent(QContextMenuEvent *event);
    void showTextEditMenuByAltM(const QModelIndex &index);
    QModelIndex focusIndex() const;
    QModelIndex indexBeforeFocusOut() const;

public slots:
    void itemclicked(const QModelIndex &index);

protected:
//    void mouseMoveEvent(QMouseEvent *);
//    void mouseClickEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *event);
Q_SIGNALS:
    void obtainingHistorical(const QModelIndex &index);

private:
    int m_focusrow = 1;
    int m_currentrow = 1;
    bool m_isMenuAltM = false; //是否是altM的菜单
};

#endif // PROLISTVIEW_H
