#ifndef MEMORYITEMWIDGET_H
#define MEMORYITEMWIDGET_H
/*******************************************************************************
 1. @类名:    MemoryItemWidget
 2. @作者:    京洲
 3. @日期:    2020-07-01
 4. @说明:    内存列表Item界面
*******************************************************************************/
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include "../widgets/memorybutton.h"

class MemoryItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemoryItemWidget(QWidget *parent = nullptr);
    ~MemoryItemWidget();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void setTextLabel(QString s);
    QString textLabel();
    void paintEvent(QPaintEvent *e);
    void setLineHight(int line);
signals:
    void plusbtnclicked();
    void minusbtnclicked();
    void cleanbtnclicked();
    void itemchanged(int type);
    void menuclean();
    void menucopy();
    void menuplus();
    void menuminus();
public slots:
    void themetypechanged(int type);
    void isexpressionempty(bool b);

public:

//    QPainter *getpainter();

private:
    MemoryButton *m_btnplus;
    MemoryButton *m_btnminus;
    MemoryButton *m_btnclean;
    QLabel *m_label;
    int m_type = 0;
    int m_themetype = 1;
    bool m_isExpressionEmpty = false;
    bool m_ispress = false;
    bool m_ishover = false;
    QFont m_font;
};

#endif // MEMORYITEMWIDGET_H
