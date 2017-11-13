#include "textbutton.h"
#include "dthememanager.h"

DWIDGET_USE_NAMESPACE

TextButton::TextButton(const QString &text)
{
    effect = new QGraphicsDropShadowEffect(this);

    setText(text);
    setFixedSize(80, 60);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");
    setGraphicsEffect(effect);

    initShadow();
    hideShadowEffect();

    connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, [=] {
        initShadow();
    });
}

TextButton::~TextButton()
{
}

void TextButton::initShadow()
{
    if (DThemeManager::instance()->theme() == "light") {
        effect->setColor(QColor(12, 155, 246, 255 * 0.1));
        effect->setXOffset(0);
        effect->setYOffset(4);
        effect->setBlurRadius(12);

        if (text() == "＝") {
            effect->setColor(QColor(12, 155, 246, 255 * 0.8));
            effect->setBlurRadius(20);
        }

    } else {
        effect->setColor(QColor(0, 0, 0, 255 * 0.1));
        effect->setXOffset(0);
        effect->setYOffset(4);
        effect->setBlurRadius(12);

        if (text() == "＝") {
            effect->setColor(QColor(12, 155, 246, 255 * 0.6));
            effect->setBlurRadius(30);
            effect->setXOffset(-2);
            effect->setYOffset(-4);
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

void TextButton::showShadowEffect()
{
    effect->setEnabled(true);
    raise();
}

void TextButton::hideShadowEffect()
{
    effect->setEnabled(false);
}
