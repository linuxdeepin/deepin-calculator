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
    ListItem* lastItem();

private:
    QList<ListItem *> listItems;
    bool isDragScrollbar;
    bool isShowScrollbar;
    int rowHeight;
    int padding;
    int offsetY;
    int scrollbarWidth;
    int scrollbarPadding;

protected:
    void paintEvent(QPaintEvent *);
    void drawScrollbar(QPainter *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void leaveEvent(QEvent *);

    int getItemsTotalHeight() const;
    int getScrollbarHeight() const;
    int getScrollbarX() const;
    int getScrollbarY() const;
    int adjustOffsetY(const int &offset) const;
};

#endif
