#include "backbutton.h"
#include "dthememanager.h"
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

BackButton::BackButton(QWidget *parent)
    : TextButton(nullptr, parent),
      m_iconWidget(new QSvgWidget)
{
    setLayout(new QHBoxLayout(this));
    layout()->addWidget(m_iconWidget);
    init();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &BackButton::init);
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
    m_iconWidget->setFixedSize(26, 26);

    TextButton::mouseReleaseEvent(e);
}

void BackButton::enterEvent(QEvent *e)
{
    m_iconWidget->setFixedSize(26, 26);

    TextButton::enterEvent(e);
}

void BackButton::leaveEvent(QEvent *e)
{
    init();

    TextButton::leaveEvent(e);
}

void BackButton::init()
{
    m_iconWidget->load(QString(":/images/delete_%1_normal.svg").arg(DThemeManager::instance()->theme()));
    m_iconWidget->setFixedSize(23, 23);
}
