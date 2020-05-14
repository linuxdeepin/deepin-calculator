#ifndef MEMORYWIDGET_H
#define MEMORYWIDGET_H

#include <QWidget>
#include <DPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QList>
#include <QObject>
#include "memoryitemwidget.h"
#include "memorylistwidget.h"
#include "../core/evaluator.h"

class MemoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(QWidget *parent = nullptr);
    ~MemoryWidget();

    void generateData(Quantity answer);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
//    bool event(QEvent *event);

    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();
    QString getfirstnumber();

    void widgetplusslot(int row, Quantity answer);
    void widgetminusslot(int row, Quantity answer);

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void insidewidget();
    void mListUnavailable();
    void mListAvailable();
    void itemclick(const QString str);
    void themechange(int type);
private:
    MemoryListWidget *m_listwidget;
    QPoint m_mousepoint;
    DPushButton *m_clearbutton;
    int m_type;
    bool m_isempty;
    Evaluator *m_evaluator;
    Quantity m_ans;
    bool m_ansVaild;
    QList<Quantity> list;
    int m_themetype = 0;

private:
    QString formatExpression(const QString &text);
    QString setitemwordwrap(const QString &text);
public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void setThemeType(int type);
};


#endif // MEMORYWIDGET_H
