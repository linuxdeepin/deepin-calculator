#ifndef MEMORYITEMWIDGET_H
#define MEMORYITEMWIDGET_H

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
    MemoryButton *btnplus;
    MemoryButton *btnminus;
    MemoryButton *btnclean;
    QLabel *label;
    int m_type = 0;
    int m_themetype = 1;
    bool isExpressionEmpty = false;
    bool ispress = false;
    bool ishover = false;
    QFont m_font;
};

#endif // MEMORYITEMWIDGET_H
