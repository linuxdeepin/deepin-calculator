#ifndef ARROWRECTANGLE_H
#define ARROWRECTANGLE_H

#include <DArrowRectangle>
#include <QWidget>
DWIDGET_USE_NAMESPACE

class ArrowRectangle : public DArrowRectangle
{
    Q_OBJECT
public:
    explicit ArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent = nullptr);
    ~ArrowRectangle();
    bool eventFilter(QObject *obj, QEvent *event);
    void focusInEvent(QFocusEvent *event);

signals:
    void hidearrowrectangle();
};

#endif // ARROWRECTANGLE_H
