#ifndef MEMORYLISTWIDGET_H
#define MEMORYLISTWIDGET_H

#include <QListWidget>

class MemoryListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit MemoryListWidget(QWidget *parent = nullptr);
    ~MemoryListWidget();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void itempressed(int row);
    void itemselected(int row);

private:
    QPoint mousepoint;
    int clickrow = -1;

public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};

class MemoryListWidgetItem : public QListWidgetItem
{
    friend class MemoryListWidget;
public:
    explicit MemoryListWidgetItem(MemoryListWidget *view = nullptr);
    ~MemoryListWidgetItem();
};

#endif // MEMORYLISTWIDGET_H
