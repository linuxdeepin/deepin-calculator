#include "programmeritemwidget.h"

ProgrammerItemWidget::ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
    , m_markbtn(new DIconButton(DStyle::SP_MarkElement))
    , m_iconbtn(new DIconButton(nullptr))
    , m_label(new QLabel(label))
{
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    m_markbtn->setFocusPolicy(Qt::NoFocus);
    m_iconbtn->setFocusPolicy(Qt::NoFocus);
    setFixedSize(QSize(250, 34));
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_iconbtn->setIcon(icon);
    m_iconbtn->setFlat(true);
    m_iconbtn->setIconSize(QSize(30, 30)); //图片有边框，不能与ui图大小一致ui(20, 20)
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
    hlayout->setContentsMargins(0, 0, 16, 0); //图片比ui大10，此处边框比ui小5
    setLayout(hlayout);
    this->installEventFilter(this);
    setMouseTracking(true);
    m_markbtn->installEventFilter(this);
    m_markbtn->setMouseTracking(true);
    m_label->installEventFilter(this);
    m_label->setMouseTracking(true);
    m_iconbtn->installEventFilter(this);
    m_iconbtn->setMouseTracking(true);
    markWidget->installEventFilter(this);
    markWidget->setMouseTracking(true);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &ProgrammerItemWidget::themetypechanged);
}

ProgrammerItemWidget::ProgrammerItemWidget(QString label, QWidget *parent)
    : QWidget(parent)
    , m_markbtn(new DIconButton(DStyle::SP_MarkElement))
    , m_label(new QLabel(label))
{
    setFixedSize(QSize(250, 34));
    setFocusPolicy(Qt::NoFocus);
    m_markbtn->setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_TranslucentBackground, true);
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
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0, 0, 21, 0);
    setLayout(hlayout);
    this->installEventFilter(this);
    setMouseTracking(true);
    m_markbtn->installEventFilter(this);
    m_markbtn->setMouseTracking(true);
    m_label->installEventFilter(this);
    m_label->setMouseTracking(true);
    markWidget->installEventFilter(this);
    markWidget->setMouseTracking(true);

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &ProgrammerItemWidget::themetypechanged);
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

bool ProgrammerItemWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        if (!m_ispress)
            m_ishover = true;
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

void ProgrammerItemWidget::cleanHoverState()
{
//    qDebug() << "ishover" << m_ishover;
    m_ishover = false;
    update();
}

void ProgrammerItemWidget::themetypechanged(int type)
{
    //主题改变时切换悬浮框图片
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    if (m_label->text() == "算数移位")
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_hover.svg"));
    else if (m_label->text() == "逻辑移位") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_hover.svg"));
    } else if (m_label->text() == "旋转循环移位") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_hover.svg"));
    } else if (m_label->text() == "循环移位旋转") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_hover.svg"));
    }

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
