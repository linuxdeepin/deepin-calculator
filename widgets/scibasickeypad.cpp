#include <QTimer>

#include <DPalette>
#include <DImageButton>

#include "dthememanager.h"
#include "scibasickeypad.h"

const SciBasicKeyPad::KeyDescription SciBasicKeyPad::keyDescriptions[] = {
    {"page1", Key_2page, 1, 0, 1, 1}, {"π", Key_PI, 1, 1, 1, 1}, {"e", Key_e, 1, 2, 1, 1},
    {"C", Key_Clear, 1, 3, 1, 1},     {"", Key_Backspace, 1, 4, 1, 1},

    {"x²", Key_x2, 2, 0, 1, 1},       {"1/x", Key_Derivative, 2, 1, 1, 1}, {"|x|", Key_Modulus, 2, 2, 1, 1},
    {"exp", Key_exp, 2, 3, 1, 1},     {"mod", Key_mod, 2, 4, 1, 1},

    {"x³", Key_x3, 3, 0, 1, 1},          {"(", Key_Left, 3, 1, 1, 1}, {")", Key_Right, 3, 2, 1, 1},
    {"n!", Key_Factorials, 3, 3, 1, 1},  {"", Key_Div, 3, 4, 1, 1},

    {"xʸ", Key_xy, 4, 0, 1, 1},       {"7", Key_7, 4, 1, 1, 1}, {"8", Key_8, 4, 2, 1, 1},
    {"9", Key_9, 4, 3, 1, 1},         {"", Key_Mult, 4, 4, 1, 1},

    {"10ˣ", Key_10x, 5, 0, 1, 1},       {"4", Key_4, 5, 1, 1, 1}, {"5", Key_5, 5, 2, 1, 1},
    {"6", Key_6, 5, 3, 1, 1},         {"", Key_Min, 5, 4, 1, 1},

    {"log", Key_log, 6, 0, 1, 1},     {"1", Key_1, 6, 1, 1, 1},         {"2", Key_2, 6, 2, 1, 1},
    {"3", Key_3, 6, 3, 1, 1},         {"", Key_Plus, 6, 4, 1, 1},

    {"ln", Key_ln, 7, 0, 1, 1},     {"%", Key_Percent, 7, 1, 1, 1},   {"0", Key_0, 7, 2, 1, 1},
    {".", Key_Point, 7, 3, 1, 1},     {"=", Key_Equal, 7, 4, 1, 1},
};

const SciBasicKeyPad::KeyDescription1 SciBasicKeyPad::keyDescriptions1[] = {
    {"page2", Key_2page, 1, 0, 1, 1}, {"π", Key_PI, 1, 1, 1, 1}, {"e", Key_e, 1, 2, 1, 1},
    {"C", Key_Clear, 1, 3, 1, 1},     {"", Key_Backspace, 1, 4, 1, 1},

    {"√x", Key_sqrt2, 2, 0, 1, 1},       {"1/x", Key_Derivative, 2, 1, 1, 1}, {"|x|", Key_Modulus, 2, 2, 1, 1},
    {"exp", Key_exp, 2, 3, 1, 1},     {"mod", Key_mod, 2, 4, 1, 1},

    {"³√x", Key_sqrt3, 3, 0, 1, 1},          {"(", Key_Left, 3, 1, 1, 1}, {")", Key_Right, 3, 2, 1, 1},
    {"n!", Key_Factorials, 3, 3, 1, 1},  {"", Key_Div, 3, 4, 1, 1},

    {"ʸ√x", Key_yx, 4, 0, 1, 1},       {"7", Key_7, 4, 1, 1, 1}, {"8", Key_8, 4, 2, 1, 1},
    {"9", Key_9, 4, 3, 1, 1},         {"", Key_Mult, 4, 4, 1, 1},

    {"2ˣ", Key_2x, 5, 0, 1, 1},       {"4", Key_4, 5, 1, 1, 1}, {"5", Key_5, 5, 2, 1, 1},
    {"6", Key_6, 5, 3, 1, 1},         {"", Key_Min, 5, 4, 1, 1},

    {"logᵧx", Key_logyx, 6, 0, 1, 1},     {"1", Key_1, 6, 1, 1, 1},         {"2", Key_2, 6, 2, 1, 1},
    {"3", Key_3, 6, 3, 1, 1},         {"", Key_Plus, 6, 4, 1, 1},

    {"eˣ", Key_ex, 7, 0, 1, 1},     {"%", Key_Percent, 7, 1, 1, 1},   {"0", Key_0, 7, 2, 1, 1},
    {".", Key_Point, 7, 3, 1, 1},     {"=", Key_Equal, 7, 4, 1, 1},
};

const SciBasicKeyPad::KeyDescriptionfun SciBasicKeyPad::keyDescriptionsfun[] = {
    {"a", Key_abs, 1, 0, 1, 1}, {"b", Key_down, 1, 1, 1, 1}, {"c", Key_up, 1, 2, 1, 1},

    {"d", Key_rand, 2, 0, 1, 1},  {"e", Key_dms, 2, 1, 1, 1}, {"f", Key_deg, 2, 2, 1, 1},
};

const SciBasicKeyPad::KeyDescriptiontri SciBasicKeyPad::KeyDescriptionstri[] = {
    {"page1", Key_Combo1page, 1, 0, 1, 1}, {"sin", Key_sin, 1, 1, 1, 1},
    {"cos", Key_cos, 1, 2, 1, 1},          {"tan", Key_tan, 1, 3, 1, 1},

    {"hyp", Key_hyp1, 2, 0, 1, 1},  {"sec", Key_sec, 2, 1, 1, 1},
    {"csc", Key_csc, 2, 2, 1, 1},       {"cot", Key_cot, 2, 3, 1, 1}
};

const SciBasicKeyPad::KeyDescriptiontri1 SciBasicKeyPad::KeyDescriptionstri1[] = {
    {"page2", Key_Combo2page, 1, 0, 1, 1}, {"arcsin", Key_arcsin, 1, 1, 1, 1},
    {"arccos", Key_arccos, 1, 2, 1, 1},    {"arctan", Key_arctan, 1, 3, 1, 1},

    {"hyp", Key_hyp1, 2, 0, 1, 1},     {"arcsec", Key_arcsec, 2, 1, 1, 1},
    {"arccsc", Key_arccsc, 2, 2, 1, 1},    {"arccot", Key_arccot, 2, 3, 1, 1}
};

const SciBasicKeyPad::KeyDescriptiontri2 SciBasicKeyPad::KeyDescriptionstri2[] = {
    {"page1", Key_Combo1page, 1, 0, 1, 1}, {"sinh", Key_sinh, 1, 1, 1, 1},
    {"cosh", Key_cosh, 1, 2, 1, 1},        {"tanh", Key_tanh, 1, 3, 1, 1},

    {"hyp", Key_hyp2, 2, 0, 1, 1},    {"sech", Key_sech, 2, 1, 1, 1},
    {"csch", Key_csch, 2, 2, 1, 1},       {"coth", Key_coth, 2, 3, 1, 1}
};

const SciBasicKeyPad::KeyDescriptiontri3 SciBasicKeyPad::KeyDescriptionstri3[] = {
    {"page2", Key_Combo2page, 1, 0, 1, 1}, {"arcsinh", Key_arcsinh, 1, 1, 1, 1},
    {"arccosh", Key_arccosh, 1, 2, 1, 1},  {"arctanh", Key_arctanh, 1, 3, 1, 1},

    {"hyp", Key_hyp2, 2, 0, 1, 1},     {"arcsech", Key_arcsech, 2, 1, 1, 1},
    {"arccsch", Key_arccsch, 2, 2, 1, 1},  {"arccoth", Key_arccoth, 2, 3, 1, 1}
};

static DPushButton *createSpecialKeyButton(SciBasicKeyPad::Buttons key)
{
    IconButton *button = new IconButton;

    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/images/%1/").arg("dark");
    else
        path = QString(":/images/%1/").arg("light");

    if (key == SciBasicKeyPad::Key_Div) {
        button->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
    } else if (key == SciBasicKeyPad::Key_Mult) {
        button->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
    } else if (key == SciBasicKeyPad::Key_Min) {
        button->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
    } else if (key == SciBasicKeyPad::Key_Plus) {
        button->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
    } else if (key == SciBasicKeyPad::Key_Backspace) {
        button->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg");
    } else if (key == SciBasicKeyPad::Key_Clear) {
        button->setIconUrl(path + "AC_normal.svg", path + "AC_hover.svg", path + "AC_press.svg");
    } else if (key == SciBasicKeyPad::Key_Percent) {
        button->setIconUrl(path + "%_normal.svg", path + "%_hover.svg", path + "%_press.svg");
    }
    //connect(button, &IconButton::updateInterface, this, &SciBasicKeyPad::updateInterface);
    return button;
}

SciBasicKeyPad::SciBasicKeyPad(QWidget *parent)
    : DWidget(parent),
      tri(new DPushButton("tri")),
      fun(new DPushButton("fun")),
      m_hlayout(new QHBoxLayout),
      m_vlayout(new QVBoxLayout(this)),
      m_mapper(new QSignalMapper(this)),
      m_stacklayout(new QStackedLayout),
      m_tristacklayout(new QStackedLayout),
      m_gridlayout1(new QGridLayout),
      m_gridlayout2(new QGridLayout),
      m_fungridlayout(new QGridLayout),
      m_trigridlayout1(new QGridLayout),
      m_trigridlayout2(new QGridLayout),
      m_trigridlayout3(new QGridLayout),
      m_trigridlayout4(new QGridLayout),
      m_funwidget(new QWidget),
      m_triwidget(new QWidget),
      m_triCombobox(new ComboBox),
      m_funCombobox(new FunCombobox),
      hwidget(new QWidget(this)),
      stackwidget(new QWidget(this))
{
    QWidget *page1 = new QWidget(this);
    QWidget *page2 = new QWidget(this);

    page1->setLayout(m_gridlayout1);
    page2->setLayout(m_gridlayout2);
    m_stacklayout->addWidget(page1);
    m_stacklayout->addWidget(page2);
    stackwidget->setLayout(m_stacklayout);
//    m_hlayout->addWidget(m_triCombobox);
//    m_hlayout->addWidget(m_funCombobox);
    m_hlayout->addWidget(tri);
    m_hlayout->addWidget(fun);
    m_hlayout->setMargin(0);
    m_hlayout->setSpacing(0);
    m_hlayout->setContentsMargins(0, 0, 0, 0);
    hwidget->setLayout(m_hlayout);
    m_funwidget->setParent(this);
    m_funwidget->setFixedSize(210, 95);
    m_funwidget->setLayout(m_fungridlayout);

    m_funwidget->setStyleSheet("QWidget { border: 0px solid #000000; background-color:rgb(180, 180, 180)}");
    m_funwidget->move(12, hwidget->height() + 5);
    m_funwidget->hide();

    QWidget *tripage1 = new QWidget;
    QWidget *tripage2 = new QWidget;
    QWidget *tripage3 = new QWidget;
    QWidget *tripage4 = new QWidget;
    tripage1->setLayout(m_trigridlayout1);
    tripage2->setLayout(m_trigridlayout2);
    tripage3->setLayout(m_trigridlayout3);
    tripage4->setLayout(m_trigridlayout4);
//    widget->setStyleSheet("QWidget{background-color:transparent;border:none;}");
    m_trigridlayout1->setSpacing(0);
    m_trigridlayout1->setMargin(0);
    m_trigridlayout1->setContentsMargins(0, 0, 0, 0);
    m_trigridlayout2->setSpacing(0);
    m_trigridlayout2->setMargin(0);
    m_trigridlayout2->setContentsMargins(0, 0, 0, 0);
    m_trigridlayout3->setSpacing(0);
    m_trigridlayout3->setMargin(0);
    m_trigridlayout3->setContentsMargins(0, 0, 0, 0);
    m_trigridlayout4->setSpacing(0);
    m_trigridlayout4->setMargin(0);
    m_trigridlayout4->setContentsMargins(0, 0, 0, 0);
    m_tristacklayout->addWidget(tripage1);
    m_tristacklayout->addWidget(tripage2);
    m_tristacklayout->addWidget(tripage3);
    m_tristacklayout->addWidget(tripage4);
//    m_hlayout->addLayout(m_stacklayout);
//    m_hlayout->setMargin(0);
//    m_hlayout->setSpacing(0);
//    m_hlayout->setContentsMargins(0, 0, 0, 0);
    m_triwidget->setParent(this);
    m_triwidget->setFixedSize(280, 95);
    m_triwidget->setLayout(m_tristacklayout);
    m_triwidget->setStyleSheet("QWidget { border: 0px solid #000000; background-color:rgb(180, 180, 180)}");
    m_triwidget->move(12, hwidget->height() + 5);
    m_triwidget->hide();

    m_hlayout->addStretch();
    m_vlayout->addWidget(hwidget);
    m_vlayout->addWidget(stackwidget);
    m_vlayout->setMargin(0);
    m_vlayout->setSpacing(0);
    m_vlayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_vlayout);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(m_triCombobox, SIGNAL(buttonPressed(int)), SIGNAL(buttonPressed(int)));
    connect(this, &SciBasicKeyPad::buttonPressed, this,
            &SciBasicKeyPad::turnPage);
    connect(fun, &DPushButton::clicked, m_funwidget, [ = ]() {
        m_funwidget->show();
        hwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        emit funshow();
    });
    connect(tri, &DPushButton::clicked, m_triwidget, [ = ]() {
        m_triwidget->show();
        hwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents);
        emit funshow();
    });

    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &SciBasicKeyPad::handleThemeChanged);
}

SciBasicKeyPad::~SciBasicKeyPad()
{
}

DPushButton *SciBasicKeyPad::button(Buttons key)
{
    if (m_stacklayout->currentIndex() == 0) {
        return m_keys.value(key).first;
    } else {
        return m_keys1.value(key).first;
    }
}

DPushButton *SciBasicKeyPad::funbutton(Buttons key)
{
    return m_funkeys.value(key).first;
}

DPushButton *SciBasicKeyPad::tributton(Buttons key)
{
    if (m_tristacklayout->currentIndex() == 0) {
        return m_trikeys.value(key).first;
    } else if (m_tristacklayout->currentIndex() == 1) {
        return m_trikeys1.value(key).first;
    } else if (m_tristacklayout->currentIndex() == 2) {
        return m_trikeys2.value(key).first;
    } else if (m_tristacklayout->currentIndex() == 3) {
        return m_trikeys3.value(key).first;
    }
}

DSuggestButton *SciBasicKeyPad::button()
{
    //return m_equal;
}

void SciBasicKeyPad::animate(Buttons key)
{
    if (button(key)->text().isEmpty()) {
        IconButton *btn = static_cast<IconButton *>(button(key));
        btn->animate();
    } else {
        if (button(key)->text() == "=") {
            EqualButton *btn = dynamic_cast<EqualButton *>(button(key));
            btn->animate();
        } else {
            TextButton *btn = static_cast<TextButton *>(button(key));
            btn->animate();
        }
    }

    if (!m_funwidget->isHidden()) {
        TextButton *btn = static_cast<TextButton *>(funbutton(key));
        btn->animate();
    }
    if (!m_triwidget->isHidden()) {
        TextButton *btn = static_cast<TextButton *>(tributton(key));
        btn->animate();
    }
}

void SciBasicKeyPad::animate()
{
    //m_equal->setChecked(true);

    //QTimer::singleShot(100, this, [=] { m_equal->setChecked(false); });
}

void SciBasicKeyPad::mousePressEvent(QMouseEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *pEvent = static_cast<QMouseEvent *>(event);
        m_mousepoint = pEvent->pos();

        QRect rect(m_triwidget->frameGeometry());
        QRect qrect(m_funwidget->frameGeometry());
        if ((qrect.contains(m_mousepoint) == true && !m_funwidget->isHidden())
                || (rect.contains(m_mousepoint) == true && !m_triwidget->isHidden())) {
            qDebug() << "in";
            emit funinside();
        }
    }
    QWidget::mousePressEvent(event);
}

void SciBasicKeyPad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;

        if (desc->text.isEmpty()) {
            button = createSpecialKeyButton(desc->button);
        } else {
            if (desc->text == "=")
                button = new EqualButton(desc->text);
            else {
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
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
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
            button = createSpecialKeyButton(desc1->button);
        } else {
            if (desc1->text == "=")
                button = new EqualButton(desc1->text);
            else {
                button = new TextButton(desc1->text);
                QFont font = button->font();
                font.setFamily("HelveticaNeue");
                button->setFont(font);
            }
        }

        m_gridlayout2->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription1 *> hashValue(button, desc1);
        m_keys1.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        m_mapper->setMapping(button, desc1->button);
    }
    m_gridlayout2->setMargin(0);
    m_gridlayout2->setSpacing(0);
    m_gridlayout2->setContentsMargins(0, 0, 0, 0);

    const int countfun = sizeof(keyDescriptionsfun) / sizeof(keyDescriptionsfun[0]);
    for (int i = 0; i < countfun; ++i) {
        const KeyDescriptionfun *desc1 = keyDescriptionsfun + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_fungridlayout->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                   Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescriptionfun *> hashValue(button, desc1);
        m_funkeys.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        connect(button, &DPushButton::clicked, m_funwidget, [ = ]() {
            m_funwidget->hide();
            hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        });
        m_mapper->setMapping(button, desc1->button);
    }
    m_fungridlayout->setMargin(0);
    m_fungridlayout->setSpacing(0);
    m_fungridlayout->setContentsMargins(0, 0, 0, 0);

    const int counttri1 = sizeof(KeyDescriptionstri) / sizeof(KeyDescriptionstri[0]);
    for (int i = 0; i < counttri1; ++i) {
        const KeyDescriptiontri *desc1 = KeyDescriptionstri + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_trigridlayout1->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                    Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescriptiontri *> hashValue(button, desc1);
        m_trikeys.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        connect(button, &DPushButton::clicked, m_triwidget, [ = ]() {
            if (!(button->text() == "page1") && !(button->text() == "hyp")) {
                m_triwidget->hide();
                hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
        });
        m_mapper->setMapping(button, desc1->button);
    }
    m_trigridlayout1->setMargin(0);
    m_trigridlayout1->setSpacing(0);
    m_trigridlayout1->setContentsMargins(0, 0, 0, 0);

    const int counttri2 = sizeof(KeyDescriptionstri1) / sizeof(KeyDescriptionstri1[0]);
    for (int i = 0; i < counttri2; ++i) {
        const KeyDescriptiontri1 *desc1 = KeyDescriptionstri1 + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_trigridlayout2->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                    Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescriptiontri1 *> hashValue(button, desc1);
        m_trikeys1.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        connect(button, &DPushButton::clicked, m_funwidget, [ = ]() {
            if (!(button->text() == "page2") && !(button->text() == "hyp")) {
                m_triwidget->hide();
                hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
        });
        m_mapper->setMapping(button, desc1->button);
    }
    m_trigridlayout2->setMargin(0);
    m_trigridlayout2->setSpacing(0);
    m_trigridlayout2->setContentsMargins(0, 0, 0, 0);

    const int counttri3 = sizeof(KeyDescriptionstri2) / sizeof(KeyDescriptionstri2[0]);
    for (int i = 0; i < counttri3; ++i) {
        const KeyDescriptiontri2 *desc1 = KeyDescriptionstri2 + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_trigridlayout3->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                    Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescriptiontri2 *> hashValue(button, desc1);
        m_trikeys2.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        connect(button, &DPushButton::clicked, m_funwidget, [ = ]() {
            if (!(button->text() == "page1") && !(button->text() == "hyp")) {
                m_triwidget->hide();
                hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
        });
        m_mapper->setMapping(button, desc1->button);
    }
    m_trigridlayout3->setMargin(0);
    m_trigridlayout3->setSpacing(0);
    m_trigridlayout3->setContentsMargins(0, 0, 0, 0);

    const int counttri4 = sizeof(KeyDescriptionstri3) / sizeof(KeyDescriptionstri3[0]);
    for (int i = 0; i < counttri4; ++i) {
        const KeyDescriptiontri3 *desc1 = KeyDescriptionstri3 + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_trigridlayout4->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                    Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescriptiontri3 *> hashValue(button, desc1);
        m_trikeys3.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &SciBasicKeyPad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &SciBasicKeyPad::moveRight);
        connect(button, &DPushButton::clicked, m_funwidget, [ = ]() {
            if (!(button->text() == "page2") && !(button->text() == "hyp")) {
                m_triwidget->hide();
                hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
                stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
            }
        });
        m_mapper->setMapping(button, desc1->button);
    }
    m_trigridlayout4->setMargin(0);
    m_trigridlayout4->setSpacing(0);
    m_trigridlayout4->setContentsMargins(0, 0, 0, 0);
}

void SciBasicKeyPad::initUI()
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

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescriptionfun *>> ifun(m_funkeys);

    while (ifun.hasNext()) {
        ifun.next();
        ifun.value().first->setFocusPolicy(Qt::NoFocus);
    }

    button(Key_Div)->setObjectName("SymbolButton");
    button(Key_Mult)->setObjectName("SymbolButton");
    button(Key_Min)->setObjectName("SymbolButton");
    button(Key_Plus)->setObjectName("SymbolButton");

    this->setContentsMargins(12, 0, 13, 0);
}

void SciBasicKeyPad::buttonThemeChanged(int type)
{
    QString path;
    if (type == 2)
        path = QString(":/images/%1/").arg("dark");
    else
        path = QString(":/images/%1/").arg("light");

    IconButton *btn = static_cast<IconButton *>(button(Key_Div));
    btn->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
    btn = static_cast<IconButton *>(button(Key_Mult));
    btn->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
    btn = static_cast<IconButton *>(button(Key_Min));
    btn->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
    btn = static_cast<IconButton *>(button(Key_Plus));
    btn->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
    btn = static_cast<IconButton *>(button(Key_Backspace));
    btn->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg");
}

void SciBasicKeyPad::turnPage(int key)
{
    if (key == Key_2page) {
        TextButton *degbtn = static_cast<TextButton *>(button(Key_exp));
        QString degtext = degbtn->text();
        TextButton *acbtn = static_cast<TextButton *>(button(Key_Clear));
        QString actext = acbtn->text();
        if (m_stacklayout->currentIndex() == 0) {
            m_stacklayout->setCurrentIndex(1);
            TextButton *degbtn1 = static_cast<TextButton *>(button(Key_exp));
            degbtn1->setText(degtext);
            TextButton *acbtn1 = static_cast<TextButton *>(button(Key_Clear));
            acbtn1->setText(actext);
        } else {
            m_stacklayout->setCurrentIndex(0);
            TextButton *degbtn2 = static_cast<TextButton *>(button(Key_exp));
            degbtn2->setText(degtext);
            TextButton *acbtn2 = static_cast<TextButton *>(button(Key_Clear));
            acbtn2->setText(actext);
        }
    }
    if (m_tristacklayout->currentIndex() == 0) {
        if (key == Key_Combo1page) {
            m_tristacklayout->setCurrentIndex(1);
        }
        if (key == Key_hyp1) {
            m_tristacklayout->setCurrentIndex(2);
        }
    }
    if (m_tristacklayout->currentIndex() == 1) {
        if (key == Key_Combo2page) {
            m_tristacklayout->setCurrentIndex(0);
        }
        if (key == Key_hyp1) {
            m_tristacklayout->setCurrentIndex(3);
        }
    }
    if (m_tristacklayout->currentIndex() == 2) {
        if (key == Key_Combo1page) {
            m_tristacklayout->setCurrentIndex(3);
        }
        if (key == Key_hyp2) {
            m_tristacklayout->setCurrentIndex(0);
        }
    }
    if (m_tristacklayout->currentIndex() == 3) {
        if (key == Key_Combo2page) {
            m_tristacklayout->setCurrentIndex(2);
        }
        if (key == Key_hyp2) {
            m_tristacklayout->setCurrentIndex(1);
        }
    }
}

void SciBasicKeyPad::funhide()
{
    if (m_funwidget->isHidden()) {
        m_triwidget->hide();
    } else {
        m_funwidget->hide();
    }
    hwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    stackwidget->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}
