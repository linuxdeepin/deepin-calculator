#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>
#include <QMouseEvent>
#include "listitem.h"

class ListView : public QWidget
{
    Q_OBJECT

public:
    ListView(QWidget *parent=0);
    ~ListView();

    void addItem(ListItem *item);
    void clearAllItems();
    void clearLastItem();
    void scrollToBottom();

    QList<ListItem *> listItems;

private:
    bool isDragScrollBar;
    int rowHeight;
    int padding;
    int offsetY;
    int scrollBarWidth;
    int scrollBarPadding;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    int getItemsTotalHeight() const;
    int getScrollBarHeight() const;
    int getScrollBarX() const;
    int getScrollBarY() const;
    int adjustOffsetY(const int &offset) const;
};

#endif
