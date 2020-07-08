/*
 * 1. @类名:    MemoryButton
 * 2. @作者:    夏菁 ut000489
 * 3. @日期:    2020-07-01
 * 4. @说明:    简易计算器内存按键
 */
#include "memorybutton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <DGuiApplicationHelper>

#include "dthememanager.h"

MemoryButton::MemoryButton(const QString &text, bool listwidgetbtn, QWidget *parent)
    : TextButton(text, parent)
    , m_effect(new QGraphicsDropShadowEffect(this))
    , m_isallgray(false)
{
    m_settings = DSettings::instance(this);
    int mode = m_settings->getOption("mode").toInt();
    if (mode == 0)
        setFixedSize(50, 33);
    setObjectName("MemoryButton");
    m_widgetbtn = listwidgetbtn;

    init();
    m_isHover = m_isPress = false;
    showtips();
    m_effect->setOffset(0, 4);
    m_effect->setBlurRadius(12);
}

MemoryButton::~MemoryButton()
{
    // delete m_effect;
}

void MemoryButton::init()
{
    m_font.setPixelSize(16);
    m_font.setFamily("Noto Sans CJK SC");
    m_font.setStyleName("Light");

//    if (text() == "MC" || text() == "MR" || text() == "M^" || text() == "M+" || text() == "M-" || text() == "MS") {
//        if (!widgetbtn) {
//            m_isgray = true;
//            this->setEnabled(false);
//        }
//    } else
//        m_isgray = false;
    if (!m_widgetbtn) {
        m_isgray = true;
        this->setEnabled(false);
    }
}

/*void MemoryButton::showShadow()
{
    m_effect->setEnabled(true);
    raise();
}

void MemoryButton::hideShadow()
{
    m_effect->setEnabled(false);
}*/

void MemoryButton::animate(int msec)
{
    setDown(true);
    m_isPress = true;

    QTimer::singleShot(msec, this, [ = ] {
        setDown(false);
        m_isPress = false;
        update();
    });
}

void MemoryButton::setbtnlight(bool light)
{
    m_islight = light;
    if (light == true)
        setText("M˄");
    else
        setText("M˅");
}

void MemoryButton::showtips()
{
    QString tooltext;
    if (this->text() == "MC") {
        if (m_widgetbtn)
            tooltext = tr("Clear memory item");
        else
            tooltext = tr("Clear all memory");
    } else if (this->text() == "MR")
        tooltext = tr("Memory recall");
    else if (this->text() == "M+") {
        if (m_widgetbtn)
            tooltext = tr("Add to memory item");
        else
            tooltext = tr("Memory add");
    } else if (this->text() == "M-") {
        if (m_widgetbtn)
            tooltext = tr("Subtract from memory item");
        else
            tooltext = tr("Memory subtract");
    } else if (this->text() == "MS")
        tooltext = tr("Memory store");
    else if (this->text() == "M˄" || this->text() == "M˅")
        tooltext = tr("Memory");
    this->setToolTip(tooltext);
}

/**
 * @brief MemoryButton::setbuttongray
 * @param memorywidgetshow 历史区域是否显示
 * 历史区域显示，除mlist外按钮置灰
 */
void MemoryButton:: setbuttongray(bool memorywidgetshow)
{
    //20200701 精简代码
    m_isallgray = memorywidgetshow;
}

void MemoryButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
    setFocus();
    m_palette = this->palette();
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        DPalette pa = this->palette();
        QColor light, dark, buttontext;
        light = QColor(0, 0, 0);
        light.setAlphaF(0.1);
        dark = QColor(0, 0, 0);
        dark.setAlphaF(0.1);
        buttontext = QColor(0, 129, 255);
        buttontext.setAlphaF(1);
        pa.setColor(DPalette::Light, light);
        pa.setColor(DPalette::Dark, dark);
        pa.setColor(DPalette::ButtonText, buttontext);
//        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.1 * 255));
//        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.1 * 255));
//        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
        this->setPalette(pa);
    } else {
        DPalette pa = this->palette();
        QColor light, dark, buttontext;
        light = QColor(0, 0, 0);
        light.setAlphaF(0.5);
        dark = QColor(0, 0, 0);
        dark.setAlphaF(0.5);
        buttontext = QColor(0, 129, 255);
        buttontext.setAlphaF(1);
        pa.setColor(DPalette::Light, light);
        pa.setColor(DPalette::Dark, dark);
        pa.setColor(DPalette::ButtonText, buttontext);
//        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.5 * 255));
//        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.5 * 255));
//        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
        this->setPalette(pa);
    }
    m_isPress = true;
    emit updateInterface();
    DPushButton::mousePressEvent(e);
}

void MemoryButton::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        return;
    this->setPalette(m_palette);
    m_isPress = false;
    if (this->rect().contains(e->pos())) {
        m_isacting = true;
        m_isHover = true;
    } else
        m_isacting = false;
    DPushButton::mouseReleaseEvent(e);
}

void MemoryButton::enterEvent(QEvent *e)
{
    m_font.setPixelSize(17);
    m_font.setStyleName("");
    m_isHover = true;

    //    if (e->type() == QEvent::ToolTip) {
    //        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
    //        QToolTip::showText(helpEvent->globalPos(), this->text());
    //    }
    DPushButton::enterEvent(e);
}

void MemoryButton::leaveEvent(QEvent *e)
{
    m_font.setPixelSize(16);
    m_font.setStyleName("Light");
    m_isHover = false;
    m_isacting = false;
    DPushButton::leaveEvent(e);
}

void MemoryButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    int mode = m_settings->getOption("mode").toInt();
    if (mode == 0)
        setFixedSize(50, 33);
    if (m_widgetbtn)
        setFixedSize(50, 33);
    if (isEnabled() == false) {
        m_font.setPixelSize(16);
        m_font.setStyleName("Light");
        m_isHover = false;
        m_isgray = true;
    } else {
        m_isgray = false;
    }
    QRectF rect = this->rect();
    QRectF normal(rect.left() + 2, rect.top() + 2, rect.width() - 4, rect.height() - 4);
    QRectF hover(rect.left() + 3, rect.top() + 3, rect.width() - 6, rect.height() - 6);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setFont(m_font);
//    QRectF textRect = painter.fontMetrics().boundingRect(this->text());
//    textRect.moveCenter(rect.center());
    QRectF textRect = painter.fontMetrics().boundingRect(0, 0, int(rect.width()), int(rect.height()),
                                                         Qt::AlignCenter, this->text());
    // QRectF
    // textRect(QPointF((rect.width()/2)-(textR.width()/2),(rect.height()/2)-(textR.height()/2)),textR.width(),textR.height());
    QColor actcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name();//活动色
    QColor pressBrush, focus, hoverFrame, base, text, hoverbrush;
    QColor pressText = actcolor;
    QColor hoverShadow, focusShadow;
    hoverShadow = actcolor;
    hoverShadow.setAlphaF(0.1);
    focusShadow = QColor(0, 0, 0);
    focusShadow.setAlphaF(0.05);
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        pressBrush = QColor(0, 0, 0);
        pressBrush.setAlphaF(0.1);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        base = Qt::white;
        hoverbrush = Qt::white;
        text = Qt::black;
        if (m_isgray == true || m_isallgray == true) {
            base = QColor("#FFFFFF");
            hoverbrush = QColor("#FFFFFF");
            text = QColor(0, 26, 46);
            text.setAlphaF(0.4);
            pressText = Qt::black;
            pressBrush = QColor("#FFFFFF");
        }
        if (this->text() == "M˄") {
            text = actcolor;
        }
    } else {
        pressBrush = QColor(0, 0, 0);
        pressBrush.setAlphaF(0.5);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        if (m_widgetbtn == false)
            base = QColor(48, 48, 48);
        else
            base = QColor(65, 65, 65);
        if (m_isHover)
            text = Qt::white;
        else
            text = QColor(224, 224, 224);
        hoverbrush = QColor(255, 255, 255);
        hoverbrush.setAlphaF(0.1);
        if (m_isgray == true || m_isallgray == true) {
            base = QColor(48, 48, 48);
            base.setAlphaF(0.4);
            text = QColor(224, 224, 224);
            text.setAlphaF(0.4);
            pressText = Qt::black;
            pressBrush = QColor("#FFFFFF");
            hoverbrush = QColor(48, 48, 48);
            hoverbrush.setAlphaF(0.4);
            if (m_widgetbtn) {
                base = QColor(65, 65, 65);
                text = QColor(224, 224, 224);
                text.setAlphaF(0.3);
            }
        }
        if (this->text() == "M˄") {
            text = actcolor;
        }
    }
    if (hasFocus()) {
        painter.setPen(Qt::NoPen);
        if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            if (m_isacting) {
                painter.setPen(Qt::NoPen);
            } else {
                pen.setColor(focus);
                pen.setWidth(2);
                painter.setPen(pen);
            }
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundRect(normal, 25, 30);

            // painter.drawRoundRect(rect,10,10);
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(focusShadow);
            this->setGraphicsEffect(m_effect);
        }
    } else {
        painter.setPen(Qt::NoPen);
        if (m_isHover) {
            painter.setPen(QPen(hoverFrame));
            painter.setBrush(QBrush(hoverbrush));
            painter.drawRoundRect(normal, 25, 30);

//            painter.setPen(Qt::NoPen);
//            painter.setBrush(Qt::NoBrush);
//            painter.setBrush(QBrush(base));
//            painter.drawRoundRect(hover, 25, 30);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(hoverShadow);
            this->setGraphicsEffect(m_effect);
        } else if (m_isPress) {
            painter.setBrush(QBrush(pressBrush));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(pressText);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
        } else {
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
            m_effect->setColor(QColor(0, 0, 0, 0));
            this->setGraphicsEffect(m_effect);
        }

    }
}

void MemoryButton::focusOutEvent(QFocusEvent *e)
{
    emit updateInterface();
    QPushButton::focusOutEvent(e);
}
