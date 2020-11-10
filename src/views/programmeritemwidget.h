#ifndef PROGRAMMERITEMWIDGET_H
#define PROGRAMMERITEMWIDGET_H

#include <QDebug>
#include <QWidget>
#include <DIconButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <DGuiApplicationHelper>
#include <QSvgRenderer>

DWIDGET_USE_NAMESPACE

class ProgrammerItemWidget : public QWidget
{
    Q_OBJECT
public:
    ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent = nullptr);
    ProgrammerItemWidget(QString label, QWidget *parent = nullptr);
    ~ProgrammerItemWidget();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void cleanHoverState();
    void isMarkHide(bool ishide);

public slots:
    void themetypechanged(int type);
    void setHover();

private:
    void drawMark(QPainter *painter) const;
    void initMark();
    void iconChanged(QPainter *painter) const;
    void setIconDark(QPainter *painter, QString path) const;

private:
    DIconButton *m_iconbtn;
    QLabel *m_label;
    bool m_ispress = false;
    bool m_ishover = false;
    int m_themetype = 1;
    QSvgRenderer *m_iconRenderer;

    //画一个勾
    QPoint m_markstart;
    QPoint m_markmid;
    QPoint m_markend;
    QVector<QPoint> m_pointVec;
    QPen m_markPen;
    bool m_isMarkHide = true;
};

#endif // PROGRAMMERITEMWIDGET_H
