#include "equalbutton.h"

#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include <DGuiApplicationHelper>

EqualButton::EqualButton(const QString &text, QWidget *parent)
    : DPushButton(text, parent),
      m_effect(new QGraphicsDropShadowEffect(this))
{
    m_settings = DSettings::instance(this);
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(80, 58) : setFixedSize(50, 33);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("TextButton");

    init();
    m_isHover = m_isPress = false;
    m_effect->setOffset(0, 4);
    m_effect->setBlurRadius(4);
}

EqualButton::~EqualButton()
{

}

void EqualButton::init()
{
    m_font.setPixelSize(30);
    m_font.setFamily("HelveticaNeue");
    m_font.setStyleName("Light");
}

void EqualButton::animate(int msec)
{
    if (m_isPress == false) { //edit for bug-20492 20200416
        m_isHover = false;  //edit for bug-20508 20200414
        setDown(true);
        m_isPress = true;

        QTimer::singleShot(msec, this, [ = ] { setDown(false); m_isPress = false;});
    }
}

void EqualButton::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Left) {
        emit moveLeft();
        return;
    } else if (e->key() == Qt::Key_Right) {
        emit moveRight();
        return;
    } else if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
        return;
    else
        DPushButton::keyPressEvent(e);
}

void EqualButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
    setFocus();
    m_isPress = true;
    DPushButton::mousePressEvent(e);
}

void EqualButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
    this->setPalette(m_palette);
    m_isPress = false;
    DPushButton::mouseReleaseEvent(e);
}

void EqualButton::enterEvent(QEvent *e)
{
    //m_font.setPixelSize(20);
    //m_font.setStyleName("");
    m_isHover = true;
    DPushButton::enterEvent(e);
}

void EqualButton::leaveEvent(QEvent *e)
{
    //m_font.setPixelSize(18);
    //m_font.setStyleName("Light");
    m_isHover = false;
    DPushButton::leaveEvent(e);
}

void EqualButton::paintEvent(QPaintEvent *e)
{
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(80, 58) : setFixedSize(50, 33);
    QRectF rect = this->rect();
    QRectF normal(rect.left() + 2, rect.top() + 2, rect.width() - 4, rect.height() - 4);
    QRectF focusBase(rect.left() + 3, rect.top() + 3, rect.width() - 6, rect.height() - 6);
    QLinearGradient linearGradient(rect.width() / 2, 0, rect.width() / 2, rect.height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setFont(m_font);
    QRectF textRect = painter.fontMetrics().boundingRect("=");
    textRect.moveCenter(rect.center());
    QColor base, text, pressText, hover0, hover1, press0, press1, frame;
    QColor normalShadow, hoverShadow, pressShadow, focusShadow;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        base = QColor("#0081FF");
        text = Qt::white;
        pressText = QColor(255, 255, 255, 0.6 * 255);
        hover0 = QColor("#209BFF");
        hover1 = QColor("#0188FF");
        press0 = QColor("#0077CE");
        press1 = QColor("#0070FF");
        frame = Qt::white;
        normalShadow = QColor(44, 167, 248, 0.4 * 255);
        hoverShadow = QColor(0, 145, 255, 0.3 * 255);
        pressShadow = QColor(0, 145, 255, 0.3 * 255);
        focusShadow = QColor(0, 0, 0, 0.05 * 255);
    } else {
        base = QColor("#0059D2");
        pressText = QColor(255, 255, 255, 0.3 * 255);
        text = Qt::white;
        hover0 = QColor("#0966E4");
        hover1 = QColor("#0063EA");
        press0 = QColor("#003279");
        press1 = QColor("#0C2E7A");
        frame = QColor(0, 0, 0, 0.4 * 255);
        normalShadow = QColor(0, 42, 175, 0.4 * 255);
        hoverShadow = QColor(0, 37, 255, 0.3 * 255);
        pressShadow = QColor(6, 26, 121, 0.3 * 255);
        focusShadow = QColor(0, 0, 0, 0.05 * 255);
    }
    if (hasFocus()) {
        painter.setPen(Qt::NoPen);
        if (m_isPress) {
            linearGradient.setColorAt(0, press0);
            linearGradient.setColorAt(1, press1);
            painter.setBrush(QBrush(linearGradient));
            painter.drawRoundRect(normal, 18, 18);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, "=");
            m_effect->setColor(pressShadow);
            this->setGraphicsEffect(m_effect);
        } else {
            QPen pen;
            pen.setColor(base);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(rect, 25, 25);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(frame));
            painter.drawRoundRect(normal, 25, 25);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(focusBase, 25, 25);

            //painter.drawRoundRect(rect,10,10);
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, "=");
            m_effect->setColor(focusShadow);
            this->setGraphicsEffect(m_effect);
        }
    } else {
        painter.setPen(Qt::NoPen);
        if (m_isHover) {
            linearGradient.setColorAt(0, hover0);
            linearGradient.setColorAt(1, hover1);
            painter.setBrush(linearGradient);
            painter.drawRoundRect(normal, 18, 18);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, "=");
            m_effect->setColor(hoverShadow);
            this->setGraphicsEffect(m_effect);
        } else if (m_isPress) {
            linearGradient.setColorAt(0, press0);
            linearGradient.setColorAt(1, press1);
            painter.setBrush(QBrush(linearGradient));
            painter.drawRoundRect(normal, 18, 18);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, "=");
            m_effect->setColor(pressShadow);
            this->setGraphicsEffect(m_effect);
        } else {
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal, 18, 18);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, "=");
            m_effect->setColor(normalShadow);
            this->setGraphicsEffect(m_effect);
        }
    }
}

