#include "ut_arrowrectangle.h"
#include "../../src/widgets/arrowrectangle.h"


Ut_ArrowRectangle::Ut_ArrowRectangle()
{

}

TEST_F(Ut_ArrowRectangle, eventFilter)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    QKeyEvent *k = new QKeyEvent(QKeyEvent::Type::FocusOut, Qt::NoButton, Qt::KeyboardModifier::NoModifier);
    arrowrectangle->eventFilter(arrowrectangle, static_cast <QEvent *>(k));
    ASSERT_TRUE(arrowrectangle->isHidden());
    delete k;
}

TEST_F(Ut_ArrowRectangle, focusInEvent)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    QFocusEvent *f = new QFocusEvent(QFocusEvent::Type::FocusIn);
    arrowrectangle->focusInEvent(f);
    delete f;
//    ASSERT_FALSE(arrowrectangle->getContent()->hasFocus());
}

TEST_F(Ut_ArrowRectangle, keyPressEvent)
{
    ArrowRectangle *arrowrectangle = new ArrowRectangle(DArrowRectangle::ArrowTop, DArrowRectangle::FloatWidget);
    QKeyEvent *k = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    arrowrectangle->keyPressEvent(k);
    delete k;
}
