#include <QTimer>

#include <DPalette>
#include <DImageButton>

#include "dthememanager.h"
#include "scientifickeypad.h"

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

    {"", Key_arcsin, 1, 0, 1, 1},       {"", Key_sqrt2, 1, 1, 1, 1},
    {"", Key_arccos, 2, 0, 1, 1}, {"", Key_sqrt3, 2, 1, 1, 1},

    {"", Key_arctan, 3, 0, 1, 1},       {"", Key_ysqrtx, 3, 1, 1, 1},

    {"", Key_arccot, 4, 0, 1, 1}, {"2ˣ", Key_10x, 4, 1, 1, 1},

    {"|x|", Key_Modulus, 5, 0, 1, 1}, {"logᵧx", Key_logyx, 5, 1, 1, 1},

    {"Rand", Key_Rand, 6, 0, 1, 1}, {"eˣ", Key_ex, 6, 1, 1, 1},
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
    : DWidget(parent),
      m_vlayout(new QVBoxLayout(this)),
      m_mapper(new QSignalMapper(this)),
      m_gridlayout1(new QGridLayout),
      m_gridlayout2(new QGridLayout),
      page2(new QWidget)
{
    QWidget *page1 = new QWidget(this);

    page1->setLayout(m_gridlayout1);
    page2->setLayout(m_gridlayout2);
    page2->setParent(this);
    page2->setFixedSize(137, 314);
    page2->setAutoFillBackground(true);
    page2->move(11, page1->height() + 75);
    page2->hide();
    m_vlayout->addWidget(page1);
    m_vlayout->setMargin(0);
    m_vlayout->setSpacing(0);
    m_vlayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_vlayout);
    this->setFixedWidth(430);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(this, &ScientificKeyPad::buttonPressed, this,
            &ScientificKeyPad::turnPage);

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

DSuggestButton *ScientificKeyPad::button()
{
    //return m_equal;
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

void ScientificKeyPad::mousePressEvent(QMouseEvent *event)
{
}

void ScientificKeyPad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;

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
                QFont font = button->font();
                font.setFamily("HelveticaNeue");
                button->setFont(font);
            } else {
                if (i > 5 && (i % 6 == 0 || i % 6 == 1))
                    button = new TextButton(desc->text, true);
                else
                    button = new TextButton(desc->text);
                QFont font = button->font();
                font.setFamily("HelveticaNeue");
                button->setFont(font);
            }
        }

        m_gridlayout1->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
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

    const int count1 = sizeof(keyDescriptions1) / sizeof(keyDescriptions1[0]);
    for (int i = 0; i < count1; ++i) {
        const KeyDescription1 *desc1 = keyDescriptions1 + i;
        DPushButton *button;

        if (desc1->text.isEmpty()) {
            button = createSpecialKeyButton(desc1->button, true);
        } else {
            button = new TextButton(desc1->text, true);

            QFont font = button->font();
            font.setFamily("HelveticaNeue");
            button->setFont(font);

        }

        m_gridlayout2->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription1 *> hashValue(button, desc1);
        if (!(desc1->text == "|x|" || desc1->text == "Rand"))
            m_keys1.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ScientificKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ScientificKeyPad::moveRight);
        m_mapper->setMapping(button, desc1->button);
    }
    m_gridlayout2->setMargin(0);
    m_gridlayout2->setSpacing(0);
    m_gridlayout2->setContentsMargins(0, 0, 0, 0);
}

void ScientificKeyPad::initUI()
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy(Qt::NoFocus);
    }

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription1 *>> i1(m_keys1);

    while (i1.hasNext()) {
        i1.next();
        i1.value().first->setFocusPolicy(Qt::NoFocus);
    }

    button(Key_Div)->setObjectName("SymbolButton");
    button(Key_Mult)->setObjectName("SymbolButton");
    button(Key_Min)->setObjectName("SymbolButton");
    button(Key_Plus)->setObjectName("SymbolButton");

    this->setContentsMargins(12, 0, 13, 0);
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
        if (page2->isHidden()) {
            page2->setHidden(false);
//            buttonThemeChanged(m_themetype);
        } else {
            page2->setHidden(true);
//            buttonThemeChanged(m_themetype);
        }
    }
}

void ScientificKeyPad::getdeg(int deg)
{
    m_deg = deg;
    QString path;
    if (m_themetype == 2)
        path = QString(":/assets/images/%1/").arg("dark");
    else
        path = QString(":/assets/images/%1/").arg("light");
    IconButton *btn;
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
