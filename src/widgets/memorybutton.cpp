#include "memorybutton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include "dthememanager.h"

#include <DGuiApplicationHelper>

MemoryButton::MemoryButton(const QString &text, bool listwidgetbtn, QWidget *parent)
    : DPushButton(text, parent)
    , m_isallgray(false)//,
      // m_effect(new QGraphicsDropShadowEffect(this))
{
    m_settings = DSettings::instance(this);
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(50, 30) : setFixedSize(67, 44);
    setFocusPolicy(Qt::NoFocus);
    setObjectName("MemoryButton");
    widgetbtn = listwidgetbtn;

    init();
//    if (this->text() == "MC" || this->text() == "MR" || this->text() == "M^") {
//        if (m_islistwidgetbtn == true) {
//            this->setEnabled(true);
//        }
//    }

    // connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &MemoryButton::init);
    m_isHover = m_isPress = false;
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
    if (!widgetbtn) {
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

void MemoryButton::setbuttongray(bool b)
{
    if (text() != "M˅" || text() != "M˄")
        m_isallgray = true;
    if (b == false)
        m_isallgray = false;
    update();
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
        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.1 * 255));
        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.1 * 255));
        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
        this->setPalette(pa);
    } else {
        DPalette pa = this->palette();
        pa.setColor(DPalette::Light, QColor(0, 0, 0, 0.5 * 255));
        pa.setColor(DPalette::Dark, QColor(0, 0, 0, 0.5 * 255));
        pa.setColor(DPalette::ButtonText, QColor(0, 129, 255, 255));
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

bool MemoryButton::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(e);
        if (this->m_isHover == true) {
            QString tooltext;
            if (this->text() == "MC") {
                if (widgetbtn)
                    tooltext = tr("Clear memory item");
                else
                    tooltext = tr("Clear all memory");
            } else if (this->text() == "MR")
                tooltext = tr("Memory recall");
            else if (this->text() == "M+") {
                if (widgetbtn)
                    tooltext = tr("Add to memory item");
                else
                    tooltext = tr("Memory add");
            } else if (this->text() == "M-") {
                if (widgetbtn)
                    tooltext = tr("Subtract from memory item");
                else
                    tooltext = tr("Memory subtract");
            } else if (this->text() == "MS")
                tooltext = tr("Memory store");
            else if (this->text() == "M˄" || this->text() == "M˅")
                tooltext = tr("Memory");

            QToolTip::showText(helpEvent->globalPos(), tooltext);
        } else {
            QToolTip::hideText();
            e->ignore();
        }

        return true;
    }
//    if(e->type() == QEvent::MouseButtonPress)
//    {
//        if (m_isgray == true) {
//            setEnabled(false);
//            qDebug() << "unable";
//        }
//        else {
//            setEnabled(true);
//            qDebug() << "enable";
//        }

//        return true;
//    }
    return DPushButton::event(e);
}

void MemoryButton::leaveEvent(QEvent *e)
{
    m_font.setPixelSize(16);
    m_font.setStyleName("Light");
    m_isHover = false;
    DPushButton::leaveEvent(e);
}

void MemoryButton::keyPressEvent(QKeyEvent *e)
{
    clearFocus();
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

void MemoryButton::paintEvent(QPaintEvent *e)
{
    int mode = m_settings->getOption("mode").toInt();
    mode == 0 ? setFixedSize(50, 33) : setFixedSize(67, 44);
    if (widgetbtn)
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
    QRectF textRect = painter.fontMetrics().boundingRect(0, 0, rect.width(), rect.height(),
                                                         Qt::AlignCenter, this->text());
    // QRectF
    // textRect(QPointF((rect.width()/2)-(textR.width()/2),(rect.height()/2)-(textR.height()/2)),textR.width(),textR.height());
    QColor actcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color();//活动色
    QColor pressBrush, focus, hoverFrame, base, text, hoverbrush;
    QColor pressText = actcolor;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType();
    if (type == 1) {
        pressBrush = QColor(0, 0, 0, 0.1 * 255);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        base = Qt::white;
        hoverbrush = Qt::white;
        text = Qt::black;
        if (m_isgray == true || m_isallgray == true) {
            base = QColor("#FFFFFF");
            hoverbrush = QColor("#FFFFFF");
            text = QColor(0, 26, 46, 0.4 * 255);
            pressText = Qt::black;
            pressBrush = QColor("#FFFFFF");
        }
        if (this->text() == "M˄") {
            text = actcolor;
        }
    } else {
        pressBrush = QColor(0, 0, 0, 0.5 * 255);
        focus = actcolor;
        hoverFrame = actcolor;
        hoverFrame.setAlphaF(0.2);
        if (widgetbtn == false)
            base = QColor(48, 48, 48);
        else
            base = QColor(65, 65, 65);
        if (m_isHover)
            text = Qt::white;
        else
            text = QColor(224, 224, 224);
        hoverbrush = QColor(255, 255, 255, 0.1 * 255);
        if (m_isgray == true || m_isallgray == true) {
            base = QColor(48, 48, 48, 0.4 * 255);
            text = QColor(224, 224, 224, 0.4 * 255);
            pressText = Qt::black;
            pressBrush = QColor("#FFFFFF");
            hoverbrush = QColor(48, 48, 48, 0.4 * 255);
            if (widgetbtn) {
                base = QColor(65, 65, 65);
                text = QColor(224, 224, 224, 0.3 * 255);
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
//            QPen pen;
//            pen.setColor(focus);
//            pen.setWidth(2);
//            painter.setPen(pen);
//            painter.setBrush(Qt::NoBrush);
////            if (widgetbtn == false) {
//            QPainterPath path;
//            path.addRoundedRect(rect, 8, 8);
//            painter.fillPath(path, QBrush(focus));
//            painter.drawPath(path);
////            }
//            painter.setPen(Qt::NoPen);
//            painter.setBrush(QBrush(base));
//            painter.drawRoundRect(normal, 25, 30);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(base));
            painter.drawRoundRect(normal, 25, 30);
            QPen pen;
            pen.setColor(focus);
            pen.setWidth(2);
            painter.setPen(pen);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundRect(normal, 25, 30);

            // painter.drawRoundRect(rect,10,10);
            pen.setColor(text);
            painter.setPen(pen);
            painter.setFont(m_font);
            painter.drawText(textRect, this->text());
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
        }

    }
}

void MemoryButton::focusOutEvent(QFocusEvent *e)
{
    emit updateInterface();
    QPushButton::focusOutEvent(e);
}
