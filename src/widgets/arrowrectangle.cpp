#include "arrowrectangle.h"
#include <QDebug>

ArrowRectangle::ArrowRectangle(ArrowDirection direction, FloatMode floatMode, QWidget *parent)
    : DArrowRectangle(direction, floatMode, parent)
{
    this->installEventFilter(this);
    if (getContent())
        getContent()->installEventFilter(this);
}

ArrowRectangle::~ArrowRectangle()
{

}

bool ArrowRectangle::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        //点击标题栏及外部桌面失去焦点时切换至scientifickeypad
        QFocusEvent *focus_Event = static_cast<QFocusEvent *>(event);
        if (focus_Event->reason() == Qt::MouseFocusReason || focus_Event->reason() == Qt::ActiveWindowFocusReason) {
            emit hidearrowrectangle();
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key_event = static_cast < QKeyEvent *>(event); //将事件转化为键盘事件
        if (key_event->key() == Qt::Key_Escape) {
            emit hidearrowrectangle();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ArrowRectangle::focusInEvent(QFocusEvent *event)
{
    if (event->reason() == Qt::TabFocusReason && getContent()) {
        getContent()->setFocus();
    }
    DArrowRectangle::focusInEvent(event);
}
