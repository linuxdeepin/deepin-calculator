#ifndef MEMORYLISTWIDGET_H
#define MEMORYLISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include "memoryitemwidget.h"

class MemoryListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryListWidget(QWidget *parent = nullptr);
    ~MemoryListWidget();

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
private:
    QListWidget *m_listwidget;
    QScopedPointer<QPushButton> m_pDeleteBtn;
    QPoint m_mousepoint;
    int m_type;
    bool m_isempty;
public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
};


#endif // MEMORYLISTWIDGET_H
