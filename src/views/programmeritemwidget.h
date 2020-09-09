#ifndef PROGRAMMERITEMWIDGET_H
#define PROGRAMMERITEMWIDGET_H

#include <QWidget>
#include <DIconButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <DGuiApplicationHelper>

DWIDGET_USE_NAMESPACE

class ProgrammerItemWidget : public QWidget
{
    Q_OBJECT
public:
    ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent = nullptr);
    ~ProgrammerItemWidget();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
public slots:
    void themetypechanged(int type);

private:
    DIconButton *m_markbtn;
    DIconButton *m_iconbtn;
    QLabel *m_label;
    bool m_ispress = false;
    bool m_ishover = false;
    int m_themetype = 1;
};

#endif // PROGRAMMERITEMWIDGET_H
