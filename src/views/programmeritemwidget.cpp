#include "programmeritemwidget.h"

ProgrammerItemWidget::ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
    , m_iconbtn(new DIconButton(nullptr))
    , m_label(new QLabel(label))
{
    setFixedSize(QSize(220, 50));
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_iconbtn->setIcon(icon);
    m_iconbtn->setFlat(true);
    m_iconbtn->setIconSize(QSize(30, 30));
    m_iconbtn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addWidget(m_label);
    hlayout->addStretch();
    hlayout->addWidget(m_iconbtn);
    setLayout(hlayout);
}

ProgrammerItemWidget::~ProgrammerItemWidget()
{

}

void ProgrammerItemWidget::enterEvent(QEvent *event)
{
    m_ishover = true;
    update();
    QWidget::enterEvent(event);
}

void ProgrammerItemWidget::leaveEvent(QEvent *event)
{
    m_ishover = false;
    update();
    QWidget::leaveEvent(event);
}

void ProgrammerItemWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
//    setFocus();
    m_ispress = true;
    DPalette pl1 = this->palette(); //按下时给label字体设置颜色
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, QColor("#303030"));
        pl1.setColor(DPalette::HighlightedText, QColor("#303030"));
    } else {
        pl1.setColor(DPalette::Text, QColor("#FFFFFF"));
        pl1.setColor(DPalette::HighlightedText, QColor("#FFFFFF"));
    }

    m_label->setPalette(pl1);
    update();
    QWidget::mousePressEvent(event);
}

void ProgrammerItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
//    clearFocus();
    m_ispress = false;
    DPalette pl1 = this->palette(); //松开鼠标时给label字体重新设置颜色
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, QColor("#303030"));
        pl1.setColor(DPalette::HighlightedText, QColor("#303030"));
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl1.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }

    update();
    m_label->setPalette(pl1);
    QWidget::mouseReleaseEvent(event);
}

//void ProgrammerItemWidget::setTextLabel(QString s)
//{
//    m_label->setText(s);
//}

//void ProgrammerItemWidget::setIcon(const QIcon &icon)
//{
//    m_iconbtn->setIcon(icon);
//}

void ProgrammerItemWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QRectF rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); //防走样
    painter.setPen(Qt::NoPen);
    QColor press, hover;
    int type = DGuiApplicationHelper::instance()->paletteType();
    if (type == 0)
        type = DGuiApplicationHelper::instance()->themeType(); //获取当前主题类型
    if (type == 1) {
        press = QColor(0, 0, 0);
        press.setAlphaF(0.2);
        hover = QColor(0, 0, 0);
        hover.setAlphaF(0.05);
    } else {
        press = QColor(255, 255, 255);
        press.setAlphaF(0.2);
        hover = QColor(255, 255, 255);
        hover.setAlphaF(0.05);
    }
    if (m_ishover) {
        painter.setPen(Qt::NoPen);
//        painter.setFont(m_font);
        painter.setBrush(hover);
        painter.drawRect(rect); //hover状态下对item进行颜色填充
        if (m_ispress) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(press);
            painter.drawRect(rect); //press状态下对item进行颜色填充

        }
    }
}

void ProgrammerItemWidget::themetypechanged(int type)
{
    m_themetype = type;
    DPalette pl1 = this->palette();
    //itemwidget字体颜色设置
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, QColor("#303030"));
    } else {
        pl1.setColor(DPalette::Text, QColor("#B4B4B4"));
    }

    m_label->setPalette(pl1);
}
