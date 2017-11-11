#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>

class ListView : public QWidget
{
    Q_OBJECT

public:
    ListView(QWidget *parent=0);
    ~ListView();

    void initTheme();
    void addItem(const QString &text);
    void clearItems();
    void scrollToBottom();
    int getItemsCount();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void leaveEvent(QEvent *);
    int getItemsTotalHeight();
    int getScrollbarHeight();
    int getScrollbarX();
    int getScrollbarY();
    int adjustOffset(const int &offset);

private:
    QList<QString> listItems;
    int rowHeight;
    int padding;
    int offset;
    bool isPress;
    bool isShowScrollbar;
    QString backgroundColor;
    QString fontColor;
    QString scrollbarColor;
};

#endif
