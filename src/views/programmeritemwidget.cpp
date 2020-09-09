#include "programmeritemwidget.h"

ProgrammerItemWidget::ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
    , m_markbtn(new DIconButton(DStyle::SP_MarkElement))
    , m_iconbtn(new DIconButton(nullptr))
    , m_label(new QLabel(label))
{
    setFixedSize(QSize(250, 34));
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_iconbtn->setIcon(icon);
    m_iconbtn->setFlat(true);
    m_iconbtn->setIconSize(QSize(20, 20));
    m_iconbtn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QWidget *markWidget = new QWidget(this);
    markWidget->setFixedSize(12, 10);
    m_markbtn->setParent(markWidget);
    m_markbtn->setObjectName("markBtn");
    m_markbtn->setFlat(true);
    m_markbtn->setIconSize(QSize(12, 10));
    m_markbtn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addSpacing(12);
    hlayout->addWidget(markWidget);
    m_markbtn->setHidden(true);
    hlayout->addSpacing(8);
    hlayout->addWidget(m_label);
    hlayout->addStretch();
    hlayout->addWidget(m_iconbtn);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0, 0, 21, 0);
    setLayout(hlayout);

    //暂时放置，连接themetypechanged后取消
    DPalette pl = this->palette();
    if (m_themetype == 1) {
        pl.setColor(DPalette::Text, QColor("#000000"));
        pl.setColor(DPalette::HighlightedText, QColor("#000000"));
    } else {
        pl.setColor(DPalette::Text, QColor("#B4B4B4"));
        pl.setColor(DPalette::HighlightedText, QColor("#B4B4B4"));
    }
    m_label->setPalette(pl);
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
    update();
    QWidget::mousePressEvent(event);
}

void ProgrammerItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        return;
//    clearFocus();
    m_ispress = false;

    update();
    QWidget::mouseReleaseEvent(event);
}

void ProgrammerItemWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QRectF rect = this->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); //防走样
    painter.setPen(Qt::NoPen);
    QColor press, hover, text, background;
    QColor actcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name();//活动色
    press = actcolor;
    hover = actcolor;
    text = QColor("#FFFFFF");
    if (m_themetype == 1)
        background = QColor("#F8F8F8");
    else
        background = QColor("#252525");
    if (m_ispress) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(press);
        painter.drawRect(rect); //press状态下对item进行颜色填充
        DPalette pl1 = this->palette(); //按下时给label字体设置颜色
        text.setAlphaF(0.5);
        pl1.setColor(DPalette::Text, text);
        pl1.setColor(DPalette::HighlightedText, text);
        m_label->setPalette(pl1);
    } else if (m_ishover) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(press);
        painter.drawRect(rect); //press状态下对item进行颜色填充
        DPalette pl1 = this->palette(); //按下时给label字体设置颜色
        text.setAlphaF(1);
        pl1.setColor(DPalette::Text, text);
        pl1.setColor(DPalette::HighlightedText, text);
        m_label->setPalette(pl1);
    } else {
        painter.setPen(Qt::NoPen);
        painter.setBrush(background);
        painter.drawRect(rect); //press状态下对item进行颜色填充
        DPalette pl1 = this->palette(); //按下时给label字体设置颜色
        if (m_themetype == 1) {
            pl1.setColor(DPalette::Text, QColor("#000000"));
            pl1.setColor(DPalette::HighlightedText, QColor("#000000"));
        } else {
            pl1.setColor(DPalette::Text, QColor("#FFFFFF"));
            pl1.setColor(DPalette::HighlightedText, QColor("#FFFFFF"));
        }
        m_label->setPalette(pl1);
    }
}

void ProgrammerItemWidget::themetypechanged(int type)
{
    m_themetype = type;
    DPalette pl1 = this->palette();
    //itemwidget字体颜色设置
    if (m_themetype == 1) {
        pl1.setColor(DPalette::Text, QColor("#000000"));
    } else {
        pl1.setColor(DPalette::Text, QColor("#FFFFFF"));
    }

    m_label->setPalette(pl1);
}
