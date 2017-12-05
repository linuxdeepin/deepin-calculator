#include "textbutton.h"
#include "dthememanager.h"
#include <QTimer>

DWIDGET_USE_NAMESPACE

TextButton::TextButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    m_effect = new QGraphicsDropShadowEffect(this);

    setMinimumSize(80, 60);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
    setGraphicsEffect(m_effect);

    initShadow();
    hideShadowEffect();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, [=] {
        initShadow();
    });
}

TextButton::~TextButton()
{
    delete m_effect;
}

void TextButton::showShadowEffect()
{
    m_effect->setEnabled(true);

    raise();
}

void TextButton::hideShadowEffect()
{
    m_effect->setEnabled(false);
}

void TextButton::animate(int msec)
{
    setDown(true);

    QTimer::singleShot(msec, this, [=] { setDown(false); });
}

void TextButton::initShadow()
{
    if (DThemeManager::instance()->theme() == "light") {
        m_effect->setColor(QColor(12, 155, 246, 255 * 0.1));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);

        if (text() == "＝") {
            m_effect->setColor(QColor(12, 155, 246, 255 * 0.8));
            m_effect->setBlurRadius(20);
        }

    } else {
        m_effect->setColor(QColor(0, 0, 0, 255 * 0.1));
        m_effect->setXOffset(0);
        m_effect->setYOffset(4);
        m_effect->setBlurRadius(12);

        if (text() == "＝") {
            m_effect->setColor(QColor(12, 155, 246, 255 * 0.6));
            m_effect->setBlurRadius(30);
            m_effect->setXOffset(-2);
            m_effect->setYOffset(-4);
        }
    }
}

void TextButton::mousePressEvent(QMouseEvent *e)
{
    hideShadowEffect();

    QPushButton::mousePressEvent(e);
}

void TextButton::mouseReleaseEvent(QMouseEvent *e)
{
    showShadowEffect();

    QPushButton::mouseReleaseEvent(e);
}

void TextButton::enterEvent(QEvent *e)
{
    showShadowEffect();

    QPushButton::enterEvent(e);
}

void TextButton::leaveEvent(QEvent *e)
{
    hideShadowEffect();

    QPushButton::leaveEvent(e);
}
