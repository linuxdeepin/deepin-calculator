#include "programmeritemwidget.h"

#include <QPen>
#include <QDomElement>

ProgrammerItemWidget::ProgrammerItemWidget(QString label, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
    , m_iconbtn(new DIconButton(this))
    , m_label(new QLabel(label))
    , m_iconRenderer(new QSvgRenderer(this))
{
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);
    m_iconbtn->setFocusPolicy(Qt::NoFocus);
    setFixedSize(QSize(250, 34));
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_iconbtn->setIcon(icon);
    m_iconbtn->setFlat(true);
    m_iconbtn->setIconSize(QSize(34, 34)); //图片有边框，不能与ui图大小一致ui(20, 20)
    m_iconbtn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    QWidget *markWidget = new QWidget(this);
    markWidget->setFixedSize(12, 10);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addSpacing(12);
    hlayout->addWidget(markWidget);
    hlayout->addSpacing(8);
    hlayout->addWidget(m_label);
    hlayout->addStretch();
    hlayout->addWidget(m_iconbtn);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0, 0, 16, 0); //图片比ui大10，此处边框比ui小5
    setLayout(hlayout);
    this->installEventFilter(this);
    setMouseTracking(true);
    m_label->installEventFilter(this);
    m_label->setMouseTracking(true);
    m_iconbtn->installEventFilter(this);
    m_iconbtn->setMouseTracking(true);
    markWidget->installEventFilter(this);
    markWidget->setMouseTracking(true);
    initMark();

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
            this, &ProgrammerItemWidget::themetypechanged);
}

ProgrammerItemWidget::ProgrammerItemWidget(QString label, QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel(label))
{
    setFixedSize(QSize(250, 34));
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QWidget *markWidget = new QWidget(this);
    markWidget->setFixedSize(12, 10);
    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addSpacing(12);
    hlayout->addWidget(markWidget);
    hlayout->addSpacing(8);
    hlayout->addWidget(m_label);
    hlayout->setMargin(0);
    hlayout->setContentsMargins(0, 0, 21, 0);
    setLayout(hlayout);
    this->installEventFilter(this);
    setMouseTracking(true);
    m_label->installEventFilter(this);
    m_label->setMouseTracking(true);
    markWidget->installEventFilter(this);
    markWidget->setMouseTracking(true);
    initMark();

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
    drawMark(&painter);
    iconChanged(&painter);
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
    m_ishover = false;
    update();
}

/**
 * @brief ProgrammerItemWidget::isMarkHide
 * @param ishide:是否隐藏勾
 * 当前已选中和点击的行显示勾，其他行隐藏
 */
void ProgrammerItemWidget::isMarkHide(bool ishide)
{
    m_isMarkHide = ishide;
}

void ProgrammerItemWidget::drawMark(QPainter *painter) const
{
    if (!m_isMarkHide) {
        QPen pen;
        QColor color;
        if (m_ispress) {
            color = QColor("#FFFFFF");
            color.setAlphaF(0.5);
        } else if (m_ishover) {
            color = QColor("#FFFFFF");
        } else {
            if (m_themetype == 1) {
                color = QColor("#000000");
            } else {
                color = QColor("#FFFFFF");
            }
        }
        pen.setColor(color);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawPolyline(m_pointVec);
    }
}

/**
 * @brief ProgrammerItemWidget::initMark
 * 初始化勾所需要的点和容器
 */
void ProgrammerItemWidget::initMark()
{
    m_markstart = QPoint(this->rect().x() + 12, this->rect().y() + 18);
    m_markmid = QPoint(this->rect().x() + 16, this->rect().y() + 22);
    m_markend = QPoint(this->rect().x() + 24, this->rect().y() + 12);
    m_pointVec << m_markstart;
    m_pointVec << m_markmid;
    m_pointVec << m_markend;
}

/**
 * @brief ProgrammerItemWidget::iconChanged
 * @param status:0-normal 1-hover 2-press
 * 根据不同鼠标事件修改icon图片
 */
void ProgrammerItemWidget::iconChanged(QPainter *painter) const
{
    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2) {
        path = QString(":/assets/images/%1/").arg("dark");
        if (m_ispress) {
            if (m_label->text() == "算数移位") {
                setIconDark(painter, path + "icon_as_hover.svg");
            } else if (m_label->text() == "逻辑移位") {
                setIconDark(painter, path + "icon_ls_hover.svg");
            } else if (m_label->text() == "旋转循环移位") {
                setIconDark(painter, path + "icon_ro_hover.svg");
            } else if (m_label->text() == "循环移位旋转") {
                setIconDark(painter, path + "icon_rc_hover.svg");
            }
        } else if (m_ishover) {
            if (m_label->text() == "算数移位")
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_hover.svg"));
            else if (m_label->text() == "逻辑移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_hover.svg"));
            } else if (m_label->text() == "旋转循环移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_hover.svg"));
            } else if (m_label->text() == "循环移位旋转") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_hover.svg"));
            }
        } else {
            if (m_label->text() == "算数移位")
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_normal.svg"));
            else if (m_label->text() == "逻辑移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_normal.svg"));
            } else if (m_label->text() == "旋转循环移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_normal.svg"));
            } else if (m_label->text() == "循环移位旋转") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_normal.svg"));
            }
        }
    } else {
        path = QString(":/assets/images/%1/").arg("light");
        if (m_ispress) {
            if (m_label->text() == "算数移位") {
                setIconDark(painter, path + "icon_as_menuhover.svg");
            } else if (m_label->text() == "逻辑移位") {
                setIconDark(painter, path + "icon_ls_menuhover.svg");
            } else if (m_label->text() == "旋转循环移位") {
                setIconDark(painter, path + "icon_ro_menuhover.svg");
            } else if (m_label->text() == "循环移位旋转") {
                setIconDark(painter, path + "icon_rc_menuhover.svg");
            }
        } else if (m_ishover) {
            if (m_label->text() == "算数移位")
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_menuhover.svg"));
            else if (m_label->text() == "逻辑移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_menuhover.svg"));
            } else if (m_label->text() == "旋转循环移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_menuhover.svg"));
            } else if (m_label->text() == "循环移位旋转") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_menuhover.svg"));
            }
        } else {
            if (m_label->text() == "算数移位")
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_normal.svg"));
            else if (m_label->text() == "逻辑移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_normal.svg"));
            } else if (m_label->text() == "旋转循环移位") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_normal.svg"));
            } else if (m_label->text() == "循环移位旋转") {
                m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_normal.svg"));
            }
        }
    }

}

void ProgrammerItemWidget::setIconDark(QPainter *painter, QString path) const
{
    painter->save();

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray baData = file.readAll();

    QDomDocument doc;
    doc.setContent(baData);

    file.close();

    //将svg文件中的fill颜色设置改为当前活动色
    doc.documentElement().setAttribute("fill-opacity", 0.5);
    QRectF frameRect = m_iconbtn->rect();
    frameRect.moveTo(200, 0);
    m_iconRenderer->load(doc.toByteArray());
    m_iconRenderer->render(painter, frameRect);
    m_iconbtn->setIcon(QIcon());
    painter->restore();
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
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_as_normal.svg"));
    else if (m_label->text() == "逻辑移位") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ls_normal.svg"));
    } else if (m_label->text() == "旋转循环移位") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_ro_normal.svg"));
    } else if (m_label->text() == "循环移位旋转") {
        m_iconbtn->setIcon(QIcon::fromTheme(path + "icon_rc_normal.svg"));
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

void ProgrammerItemWidget::setHover()
{
    m_ishover = true;
    update();
}
