#ifndef MEMORYITEMWIDGET_H
#define MEMORYITEMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "../widgets/memorybutton.h"

class MemoryItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryItemWidget(QWidget *parent = nullptr);
    ~MemoryItemWidget();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
//    void paintEvent(QPaintEvent *event);
signals:
    void plusbtnclicked();
    void minusbtnclicked();
    void cleanbtnclicked();

public:

//    QPainter *getpainter();

private:
    MemoryButton *btnplus;
    MemoryButton *btnminus;
    MemoryButton *btnclean;

};

#endif // MEMORYITEMWIDGET_H
