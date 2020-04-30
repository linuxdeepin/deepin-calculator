#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include <DPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include "memoryitemwidget.h"
#include "memorylistwidget.h"

class MemoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(QWidget *parent = nullptr);
    ~MemoryWidget();

    void generateData(const QString str);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    bool event(QEvent *event);

    void memoryplus(const QString str);
    void memoryminus(const QString str);
    void memoryclean();
    QString getfirstnumber();

    void widgetplusslot(int row, const QString str);
    void widgetminusslot(int row, const QString str);

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void insidewidget();
    void mListUnavailable();
    void mListAvailable();
    void itemclick(const QString str);
private:
    MemoryListWidget *m_listwidget;
    QPoint m_mousepoint;
    DPushButton *m_clearbutton;
    int m_type;
    bool m_isempty;
public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};


#endif // MEMORYWIDGET_H
