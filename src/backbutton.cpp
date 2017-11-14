#include "backbutton.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

BackButton::BackButton(QWidget *parent)
    : TextButton(nullptr, parent)
{
    init();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, [=] {
        init();
    });
}

BackButton::~BackButton()
{
}

void BackButton::mousePressEvent(QMouseEvent *e)
{
    init();
    TextButton::mousePressEvent(e);
}

void BackButton::mouseReleaseEvent(QMouseEvent *e)
{
    setIconSize(QSize(33, 26));
    TextButton::mouseReleaseEvent(e);
}

void BackButton::enterEvent(QEvent *e)
{
    setIconSize(QSize(33, 26));
    TextButton::enterEvent(e);
}

void BackButton::leaveEvent(QEvent *e)
{
    init();
    TextButton::leaveEvent(e);
}

void BackButton::init()
{
    setIconSize(QSize(30, 23));
    setIcon(QIcon(QString(":/images/delete_%1_normal.svg").arg(DThemeManager::instance()->theme())));
}
