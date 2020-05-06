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
signals:
    void plusbtnclicked();
    void minusbtnclicked();
    void cleanbtnclicked();
    void itemchanged();

public:

//    QPainter *getpainter();

private:
    MemoryButton *btnplus;
    MemoryButton *btnminus;
    MemoryButton *btnclean;
    int m_type = 0;
};

#endif // MEMORYITEMWIDGET_H
