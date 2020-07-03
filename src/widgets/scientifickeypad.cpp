/*
 * 1. @类名:    ScientificKeyPad
 * 2. @作者:    夏菁 ut000489
 * 3. @日期:    2020-07-01
 * 4. @说明:    科学计算器键盘区域界面
 */
#include <QTimer>

#include <DPalette>
#include <DImageButton>
#include <QFloat16>
#include <DGuiApplicationHelper>

#include "dthememanager.h"
#include "scientifickeypad.h"
#include <com_deepin_daemon_appearance.h>
using ActionColor = com::deepin::daemon::Appearance;

const ScientificKeyPad::KeyDescription ScientificKeyPad::keyDescriptions[] = {
    {"F-E", Key_FE, 1, 0, 1, 1},   {"MC", Key_MC, 1, 1, 1, 1},   {"MR", Key_MR, 1, 2, 1, 1},
    {"M+", Key_Mplus, 1, 3, 1, 1}, {"M-", Key_Mmin, 1, 4, 1, 1}, {"MS", Key_MS, 1, 5, 1, 1},

    {"", Key_deg, 2, 0, 1, 1}, {"2ⁿᵈ", Key_page, 2, 1, 1, 1}, {"π", Key_PI, 2, 2, 1, 1},
    {"e", Key_e, 2, 3, 1, 1},     {"mod", Key_Mod, 2, 4, 1, 1},   {"", Key_Backspace, 2, 5, 1, 1},

    {"sin", Key_sin, 3, 0, 1, 1},       {"x²", Key_x2, 3, 1, 1, 1},   {"1/x", Key_Derivative, 3, 2, 1, 1},
    {"x!", Key_Factorials, 3, 3, 1, 1}, {"exp", Key_exp, 3, 4, 1, 1}, {"C", Key_Clear, 3, 5, 1, 1},

    {"cos", Key_cos, 4, 0, 1, 1}, {"x³", Key_x3, 4, 1, 1, 1},     {"(", Key_left, 4, 2, 1, 1},
    {")", Key_right, 4, 3, 1, 1}, {"%", Key_Percent, 4, 4, 1, 1}, {"", Key_Div, 4, 5, 1, 1},

    {"tan", Key_tan, 5, 0, 1, 1},       {"xʸ", Key_xy, 5, 1, 1, 1}, {"7", Key_7, 5, 2, 1, 1},
    {"8", Key_8, 5, 3, 1, 1},         {"9", Key_9, 5, 4, 1, 1}, {"", Key_Mult, 5, 5, 1, 1},

    {"cot", Key_cot, 6, 0, 1, 1}, {"10ˣ", Key_10x, 6, 1, 1, 1}, {"4", Key_4, 6, 2, 1, 1},
    {"5", Key_5, 6, 3, 1, 1},     {"6", Key_6, 6, 4, 1, 1},     {"", Key_Min, 6, 5, 1, 1},

    {"|x|", Key_Modulus, 7, 0, 1, 1}, {"log", Key_log, 7, 1, 1, 1}, {"1", Key_1, 7, 2, 1, 1},
    {"2", Key_2, 7, 3, 1, 1},         {"3", Key_3, 7, 4, 1, 1},     {"", Key_Plus, 7, 5, 1, 1},

    {"Rand", Key_Rand, 8, 0, 1, 1}, {"ln", Key_ln, 8, 1, 1, 1},   {"+/-", Key_opposite, 8, 2, 1, 1},
    {"0", Key_0, 8, 3, 1, 1},       {".", Key_Point, 8, 4, 1, 1}, {"=", Key_Equals, 8, 5, 1, 1},
};

const ScientificKeyPad::KeyDescription1 ScientificKeyPad::keyDescriptions1[] = {

    {"", Key_arcsin, 3, 0, 1, 1},       {"", Key_sqrt2, 3, 1, 1, 1},
    {"", Key_arccos, 4, 0, 1, 1}, {"", Key_sqrt3, 4, 1, 1, 1},

    {"", Key_arctan, 5, 0, 1, 1},       {"", Key_ysqrtx, 5, 1, 1, 1},

    {"", Key_arccot, 6, 0, 1, 1}, {"2ˣ", Key_2x, 6, 1, 1, 1},

    {"logᵧx", Key_logyx, 7, 1, 1, 1},

    {"eˣ", Key_ex, 8, 1, 1, 1},
};

static DPushButton *createSpecialKeyButton(ScientificKeyPad::Buttons key, bool page)
{
    IconButton *button;
    if (page)
        button = new IconButton(nullptr, 0, true);
    else
        button = new IconButton();

    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");

    if (key == ScientificKeyPad::Key_Div) {
        button->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
    } else if (key == ScientificKeyPad::Key_Mult) {
        button->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
    } else if (key == ScientificKeyPad::Key_Min) {
        button->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
    } else if (key == ScientificKeyPad::Key_Plus) {
        button->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
    } else if (key == ScientificKeyPad::Key_Backspace) {
        button->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg", 1);
    } else if (key == ScientificKeyPad::Key_Clear) {
        button->setIconUrl(path + "AC_normal.svg", path + "AC_hover.svg", path + "AC_press.svg");
    } else if (key == ScientificKeyPad::Key_Percent) {
        button->setIconUrl(path + "%_normal.svg", path + "%_hover.svg", path + "%_press.svg");
    } else if (key == ScientificKeyPad::Key_sqrt2) {
        button->setIconUrl(path + "squareroot_normal.svg", path + "squareroot_hover.svg", path + "squareroot_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_sqrt3) {
        button->setIconUrl(path + "cuberoot_normal.svg", path + "cuberoot_hover.svg", path + "cuberoot_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_ysqrtx) {
        button->setIconUrl(path + "yroot_normal.svg", path + "yroot_hover.svg", path + "yroot_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_arcsin) {
        button->setIconUrl(path + "sin-1_normal.svg", path + "sin-1_hover.svg", path + "sin-1_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_arccos) {
        button->setIconUrl(path + "cos-1_normal.svg", path + "cos-1_hover.svg", path + "cos-1_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_arctan) {
        button->setIconUrl(path + "tan-1_normal.svg", path + "tan-1_hover.svg", path + "tan-1_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_arccot) {
        button->setIconUrl(path + "cot-1_normal.svg", path + "cot-1_hover.svg", path + "cot-1_press.svg", 3);
    } else if (key == ScientificKeyPad::Key_deg) {
        button->setIconUrl(path + "deg_normal.svg", path + "deg_hover.svg", path + "deg_press.svg", 5);
    }
    //connect(button, &IconButton::updateInterface, this, &ScientificKeyPad::updateInterface);
    return button;
}

ScientificKeyPad::ScientificKeyPad(QWidget *parent)
    : DWidget(parent)
//    , m_page2(new QWidget)
    , m_vlayout(new QVBoxLayout(this))
    , m_gridlayout1(new QGridLayout)
//    , m_gridlayout2(new QGridLayout)
    , m_mapper(new QSignalMapper(this))
    , m_leftBracket(new DLabel)
    , m_rightBracket(new DLabel)
    , m_arcsinwidget(new QStackedWidget)
    , m_arccoswidget(new QStackedWidget)
    , m_arctanwidget(new QStackedWidget)
    , m_arccotwidget(new QStackedWidget)
    , m_sqrtwidget(new QStackedWidget)
    , m_cbrtwidget(new QStackedWidget)
    , m_yrootwidget(new QStackedWidget)
    , m_2xwidget(new QStackedWidget)
    , m_logyxwidget(new QStackedWidget)
    , m_exwidget(new QStackedWidget)
{
    m_leftBracket->setFixedSize(24, 14);
    m_rightBracket->setFixedSize(24, 14);
    QWidget *page1 = new QWidget(this);
    DPalette pl = m_leftBracket->palette();
    pl.setColor(DPalette::Text, QColor(Qt::red));
    m_leftBracket->setPalette(pl);
    page1->setLayout(m_gridlayout1);
    m_vlayout->addWidget(page1);
    m_vlayout->setMargin(0);
    m_vlayout->setSpacing(0);
    m_vlayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_vlayout);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(this, &ScientificKeyPad::buttonPressed, this,
            &ScientificKeyPad::turnPage);

    m_bracketcolor = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color().name();
    m_leftBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:black;font-size:14px;"));
    m_rightBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:black;font-size:14px;"));
//connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &ScientificKeyPad::handleThemeChanged);
}

ScientificKeyPad::~ScientificKeyPad()
{
}

DPushButton *ScientificKeyPad::button(Buttons key)
{
    if (key == Key_arcsin || key == Key_arccos || key == Key_arccot ||
            key == Key_arctan || key == Key_sqrt2 || key == Key_sqrt3 ||
            key == Key_ysqrtx || key == Key_2x || key == Key_logyx || key == Key_ex) {
        return m_keys1.value(key).first;
    } else {
        return m_keys.value(key).first;
    }
}

void ScientificKeyPad::animate(Buttons key)
{
    if (button(key)->text().isEmpty()) {
        IconButton *btn = static_cast<IconButton *>(button(key));
        btn->animate();
    } else {
        if (button(key)->text() == "=") {
            EqualButton *btn = dynamic_cast<EqualButton *>(button(key));
            btn->animate();
        } else if (key == Key_MC || key == Key_MR || key == Key_Mplus || key == Key_Mmin || key == Key_MS) {
            MemoryButton *btn = static_cast<MemoryButton *>(button(key));
            btn->animate();
        } else {
            TextButton *btn = static_cast<TextButton *>(button(key));
            btn->animate();
        }
    }
}

void ScientificKeyPad::animate()
{
    //m_equal->setChecked(true);

    //QTimer::singleShot(100, this, [=] { m_equal->setChecked(false); });
}

void ScientificKeyPad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);

    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;
        DPushButton *pagebutton;

        if (desc->text.isEmpty()) {
            if (i > 5 && (i % 6 == 0 || i % 6 == 1))
                button = createSpecialKeyButton(desc->button, true);
            else
                button = createSpecialKeyButton(desc->button, false);
        } else {
            if (desc->text == "=")
                button = new EqualButton(desc->text);
            else if (desc->text == "MC" || desc->text == "MR" || desc->text == "M+" || desc->text == "M-" || desc->text == "MS") {
                button = new MemoryButton(desc->text);
            } else {
                if (i > 5 && (i % 6 == 0 || i % 6 == 1))
                    button = new TextButton(desc->text, true);
                else {
                    button = new TextButton(desc->text);
                }
            }
        }
        if (desc->button == Key_sin) {
            const KeyDescription1 *desc1 = keyDescriptions1;
            pagebutton = createSpecialKeyButton(Key_arcsin, true);
            initStackWidget(m_arcsinwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_x2) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 1;
            pagebutton = createSpecialKeyButton(Key_sqrt2, true);
            initStackWidget(m_arccoswidget, button, pagebutton, desc1);
        } else if (desc->button == Key_cos) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 2;
            pagebutton = createSpecialKeyButton(Key_arccos, true);
            initStackWidget(m_arctanwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_x3) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 3;
            pagebutton = createSpecialKeyButton(Key_sqrt3, true);
            initStackWidget(m_arccotwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_tan) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 4;
            pagebutton = createSpecialKeyButton(Key_arctan, true);
            initStackWidget(m_sqrtwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_xy) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 5;
            pagebutton = createSpecialKeyButton(Key_ysqrtx, true);
            initStackWidget(m_cbrtwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_cot) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 6;
            pagebutton = createSpecialKeyButton(Key_arccot, true);
            initStackWidget(m_yrootwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_10x) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 7;
            pagebutton = new TextButton("2ˣ", true);
            initStackWidget(m_2xwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_log) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 8;
            pagebutton = new TextButton("logᵧx", true);
            initStackWidget(m_logyxwidget, button, pagebutton, desc1);
        } else if (desc->button == Key_ln) {
            const KeyDescription1 *desc1 = keyDescriptions1 + 9;
            pagebutton = new TextButton("eˣ", true);
            initStackWidget(m_exwidget, button, pagebutton, desc1);
        } else {
            pagebutton = new DPushButton;
            m_gridlayout1->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                                     Qt::AlignHCenter | Qt::AlignBottom);
        }
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ScientificKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ScientificKeyPad::moveRight);
        m_mapper->setMapping(button, desc->button);

    }
    m_gridlayout1->setMargin(0);
    m_gridlayout1->setSpacing(0);
    m_gridlayout1->setContentsMargins(0, 0, 0, 0);
}

void ScientificKeyPad::initUI()
{
    ActionColor *m_pActionColor;
    //dbus接口获取系统活动色
    m_pActionColor = new ActionColor("com.deepin.daemon.Appearance",
                                     "/com/deepin/daemon/Appearance",
                                     QDBusConnection::sessionBus(), this);

    if (m_pActionColor->isValid()) {
        connect(m_pActionColor, &ActionColor::QtActiveColorChanged, this, &ScientificKeyPad::themeColorChanged);
    }

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        //以下信号槽按窗口比例缩放按键等
        connect(this, &ScientificKeyPad::windowSize, [ = ](int width, int height, bool hishide) {
            int padding;
            hishide == false ? padding = 370 : padding = 0;
            i.value().first->setFixedSize((width - 25 - padding) / 6, (height - 200) / 8);
        });
        if (i.key() == Key_left) {
            connect(this, &ScientificKeyPad::windowSize, [ = ]() {
                m_leftBracket->setParent(i.value().first);

                m_leftBracket->move(i.value().first->rect().x() + 37 * i.value().first->width() / 67, i.value().first->rect().y() + 22 * i.value().first->height() / 47);
            });
            connect(i.value().first, &DPushButton::pressed, [ = ]() {
                m_leftBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:%1;font-size:14px;")
                                             .arg(m_bracketcolor));
            });
            connect(i.value().first, &DPushButton::released, [ = ]() {
                m_leftBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:black;font-size:14px;"));
            });
        }
        if (i.key() == Key_right) {
            connect(this, &ScientificKeyPad::windowSize, [ = ]() {
                m_rightBracket->setParent(i.value().first);
                m_rightBracket->move(i.value().first->rect().x() + 37 * i.value().first->width() / 67, i.value().first->rect().y() + 22 * i.value().first->height() / 47);
            });
            connect(i.value().first, &DPushButton::pressed, [ = ]() {
                m_rightBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:%1;font-size:14px;")
                                              .arg(m_bracketcolor));
            });
            connect(i.value().first, &DPushButton::released, [ = ]() {
                m_rightBracket->setStyleSheet(tr("font-family:Noto Sans CJK SC;color:black;font-size:14px;"));
            });
        }
    }

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription1 *>> i1(m_keys1);

    while (i1.hasNext()) {
        i1.next();
        //以下信号槽按窗口比例缩放按键等
        connect(this, &ScientificKeyPad::windowSize, [ = ](int width, int height, bool hishide) {
            int padding;
            hishide == false ? padding = 370 : padding = 0;
            i1.value().first->setFixedSize((width - 25 - padding) / 6, (height - 200) / 8);
        });
    }

    button(Key_Div)->setObjectName("SymbolButton");
    button(Key_Mult)->setObjectName("SymbolButton");
    button(Key_Min)->setObjectName("SymbolButton");
    button(Key_Plus)->setObjectName("SymbolButton");

    this->setContentsMargins(12, 0, 13, 0);
}

/**
 * @brief ScientificKeyPad::initStackWidget
 * @param widget
 * @param button index(0)的按键
 * @param pagebutton index(1)的按键
 * @param desc1 index(1)按键信息
 * page2按键初始化设置
 */
void ScientificKeyPad::initStackWidget(QStackedWidget *widget, DPushButton *button, DPushButton *pagebutton, const KeyDescription1 *desc1)
{
    widget->addWidget(button);
    widget->addWidget(pagebutton);
    widget->setCurrentIndex(0);
    m_gridlayout1->addWidget(widget, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                             Qt::AlignHCenter | Qt::AlignBottom);

    const QPair<DPushButton *, const KeyDescription1 *> hashValue1(pagebutton, desc1);
    m_keys1.insert(desc1->button, hashValue1);
    connect(static_cast<TextButton *>(pagebutton), &TextButton::updateInterface, [ = ] {update();});
    connect(pagebutton, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    connect(static_cast<TextButton *>(pagebutton), &TextButton::moveLeft, this, &ScientificKeyPad::moveLeft);
    connect(static_cast<TextButton *>(pagebutton), &TextButton::moveRight, this, &ScientificKeyPad::moveRight);
    m_mapper->setMapping(pagebutton, desc1->button);
    connect(this, &ScientificKeyPad::windowSize, [ = ](int width, int height, bool hishide) {
        int padding;
        hishide == false ? padding = 370 : padding = 0;
        widget->setFixedSize((width - 25 - padding) / 6, (height - 200) / 8);
    });
}

void ScientificKeyPad::buttonThemeChanged(int type)
{
    m_themetype = type;
    QString path;
    if (type == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");

    IconButton *btn = static_cast<IconButton *>(button(Key_Div));
    btn->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
    btn = static_cast<IconButton *>(button(Key_Mult));
    btn->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
    btn = static_cast<IconButton *>(button(Key_Min));
    btn->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
    btn = static_cast<IconButton *>(button(Key_Plus));
    btn->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
    btn = static_cast<IconButton *>(button(Key_Backspace));
    btn->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg", 1);
    btn = static_cast<IconButton *>(button(Key_sqrt2));
    btn->setIconUrl(path + "squareroot_normal.svg", path + "squareroot_hover.svg", path + "squareroot_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_sqrt3));
    btn->setIconUrl(path + "cuberoot_normal.svg", path + "cuberoot_hover.svg", path + "cuberoot_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_ysqrtx));
    btn->setIconUrl(path + "yroot_normal.svg", path + "yroot_hover.svg", path + "yroot_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_arcsin));
    btn->setIconUrl(path + "sin-1_normal.svg", path + "sin-1_hover.svg", path + "sin-1_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_arccos));
    btn->setIconUrl(path + "cos-1_normal.svg", path + "cos-1_hover.svg", path + "cos-1_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_arctan));
    btn->setIconUrl(path + "tan-1_normal.svg", path + "tan-1_hover.svg", path + "tan-1_press.svg", 3);
    btn = static_cast<IconButton *>(button(Key_arccot));
    btn->setIconUrl(path + "cot-1_normal.svg", path + "cot-1_hover.svg", path + "cot-1_press.svg", 3);
    if (m_deg == 1) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "rad_normal.svg", path + "rad_hover.svg", path + "rad_press.svg", 5);
    } else if (m_deg == 2) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "grad_normal.svg", path + "grad_hover.svg", path + "grad_press.svg", 5);
    } else if (m_deg == 3) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "deg_normal.svg", path + "deg_hover.svg", path + "deg_press.svg", 5);
    }
}

void ScientificKeyPad::turnPage(int key)
{
    if (key == Key_page) {
        if (m_arcsinwidget->currentIndex() == 0) {
            m_arcsinwidget->setCurrentIndex(1);
            m_arccoswidget->setCurrentIndex(1);
            m_arctanwidget->setCurrentIndex(1);
            m_arccotwidget->setCurrentIndex(1);
            m_sqrtwidget->setCurrentIndex(1);
            m_cbrtwidget->setCurrentIndex(1);
            m_yrootwidget->setCurrentIndex(1);
            m_2xwidget->setCurrentIndex(1);
            m_logyxwidget->setCurrentIndex(1);
            m_exwidget->setCurrentIndex(1);
        } else {
            m_arcsinwidget->setCurrentIndex(0);
            m_arccoswidget->setCurrentIndex(0);
            m_arctanwidget->setCurrentIndex(0);
            m_arccotwidget->setCurrentIndex(0);
            m_sqrtwidget->setCurrentIndex(0);
            m_cbrtwidget->setCurrentIndex(0);
            m_yrootwidget->setCurrentIndex(0);
            m_2xwidget->setCurrentIndex(0);
            m_logyxwidget->setCurrentIndex(0);
            m_exwidget->setCurrentIndex(0);
        }
    }
}

/**
 * @brief ScientificKeyPad::getdeg
 * @param deg 当前为deg/rad/grad
 * 切换deg按键图标
 */
void ScientificKeyPad::getdeg(int deg)
{
    m_deg = deg;
    QString path;
    if (m_themetype == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    IconButton *btn;
    //m_deg == 1，点击后为rad
    if (m_deg == 1) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "rad_normal.svg", path + "rad_hover.svg", path + "rad_press.svg", 5);
    } else if (m_deg == 2) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "grad_normal.svg", path + "grad_hover.svg", path + "grad_press.svg", 5);
    } else if (m_deg == 3) {
        btn = static_cast<IconButton *>(button(Key_deg));
        btn->setIconUrl(path + "deg_normal.svg", path + "deg_hover.svg", path + "deg_press.svg", 5);
    }
}

/**
 * @brief ScientificKeyPad::bracketsNum
 * @param direction 0-左括号， 1-右括号
 * @param num 不匹配括号数
 * 显示左右括号不匹配数
 */
void ScientificKeyPad::bracketsNum(int direction, QString num)
{
    if (num == "0")
        num = "";

    if (direction == 0)
        m_leftBracket->setText(num);
    else if (direction == 1)
        m_rightBracket->setText(num);
}

void ScientificKeyPad::themeColorChanged(const QString &strColor)
{
    m_bracketcolor = strColor;
}
