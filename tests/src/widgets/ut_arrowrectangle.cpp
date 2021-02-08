#include "ut_arrowrectangle.h"
#include "../../src/widgets/arrowrectangle.h"


Ut_ArrowRectangle::Ut_ArrowRectangle()
{

}

TEST_F(Ut_ArrowRectangle, eventFilter)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    arrowrectangle->eventFilter(arrowrectangle, static_cast <QEvent *>(new QKeyEvent(QKeyEvent::Type::FocusOut, Qt::NoButton, Qt::KeyboardModifier::NoModifier)));
    ASSERT_TRUE(arrowrectangle->isHidden());
}

TEST_F(Ut_ArrowRectangle, focusInEvent)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    arrowrectangle->focusInEvent(new QFocusEvent(QFocusEvent::Type::FocusIn));
//    ASSERT_FALSE(arrowrectangle->getContent()->hasFocus());
}

TEST_F(Ut_ArrowRectangle, keyPressEvent)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    arrowrectangle->keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
}
