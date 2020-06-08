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
#include "../widgets/iconbutton.h"
#include "memoryitemdelegate.h"

class MemoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryWidget(int mode = 0, QWidget *parent = nullptr);
    ~MemoryWidget();

    void generateData(Quantity answer);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    bool event(QEvent *event);

    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();//内存列表为空时插入一个item
    QPair<QString, Quantity> getfirstnumber();

    void widgetplusslot(int row, Quantity answer);
    void widgetminusslot(int row, Quantity answer);

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void insidewidget();
    void mListUnavailable();
    void mListAvailable();
    void itemclick(const QPair<QString, Quantity>);
    void themechange(int type);
    void widgetclean(int row, int mode);
    void memorycleansig();
private:
    MemoryListWidget *m_listwidget;
    QPoint m_mousepoint;
    IconButton *m_clearbutton;
    int m_type;
    bool m_isempty;
    Evaluator *m_evaluator;
    Quantity m_ans;
    bool m_ansVaild;
    QList<Quantity> list;
    int m_themetype = 0;
    int calculatormode;//0-标准下拉 1-科学右侧
    int line = 1; //item数字行数
    MemoryItemDelegate *memoryDelegate;
    int itemwidth = 0;
    int precision = -1;//计算精度

private:
    QString formatExpression(const QString &text);
    QString setitemwordwrap(const QString &text, int row = 0);
//    void nothinginmemory(); //内存列表为空时插入一个item
    void emptymemoryfontcolor();
public slots:
//    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void setThemeType(int type);
    void expressionempty(bool b);
    void widgetcleanslot(int row, int mode);
};


#endif // MEMORYWIDGET_H
