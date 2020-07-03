/*
 * 1. @类名:    MemoryListWidget
 * 2. @作者:    京洲 ut000490
 * 3. @日期:    2020-07-01
 * 4. @说明:    内存ListWidget
 */
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
    QPoint m_mousepoint;
    int m_clickrow = -1;

public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // MEMORYLISTWIDGET_H
