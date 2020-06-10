#include <QTimer>

#include <DPalette>
#include <DImageButton>
#include <QDebug>

#include "dthememanager.h"
#include "tricombobox.h"

const ComboBox::KeyDescription ComboBox::keyDescriptions[] = {
    {"page1", Key_Combo1page, 1, 0, 1, 1}, {"sin", Key_sin, 1, 1, 1, 1},
    {"cos", Key_cos, 1, 2, 1, 1},          {"tan", Key_tan, 1, 3, 1, 1},

    {"hyp", Key_hyp1, 2, 0, 1, 1},  {"sec", Key_sec, 2, 1, 1, 1},
    {"csc", Key_csc, 2, 2, 1, 1},       {"cot", Key_cot, 2, 3, 1, 1}
};

const ComboBox::KeyDescription1 ComboBox::keyDescriptions1[] = {
    {"page2", Key_Combo2page, 1, 0, 1, 1}, {"arcsin", Key_arcsin, 1, 1, 1, 1},
    {"arccos", Key_arccos, 1, 2, 1, 1},    {"arctan", Key_arctan, 1, 3, 1, 1},

    {"hyp", Key_hyp1, 2, 0, 1, 1},     {"arcsec", Key_arcsec, 2, 1, 1, 1},
    {"arccsc", Key_arccsc, 2, 2, 1, 1},    {"arccot", Key_arccot, 2, 3, 1, 1}
};

const ComboBox::KeyDescription2 ComboBox::keyDescriptions2[] = {
    {"page1", Key_Combo1page, 1, 0, 1, 1}, {"sinh", Key_sinh, 1, 1, 1, 1},
    {"cosh", Key_cosh, 1, 2, 1, 1},        {"tanh", Key_tanh, 1, 3, 1, 1},

    {"hyp", Key_hyp2, 2, 0, 1, 1},    {"sech", Key_sech, 2, 1, 1, 1},
    {"csch", Key_csch, 2, 2, 1, 1},       {"coth", Key_coth, 2, 3, 1, 1}
};

const ComboBox::KeyDescription3 ComboBox::keyDescriptions3[] = {
    {"page2", Key_Combo2page, 1, 0, 1, 1}, {"arcsinh", Key_arcsinh, 1, 1, 1, 1},
    {"arccosh", Key_arccosh, 1, 2, 1, 1},  {"arctanh", Key_arctanh, 1, 3, 1, 1},

    {"hyp", Key_hyp2, 2, 0, 1, 1},     {"arcsech", Key_arcsech, 2, 1, 1, 1},
    {"arccsch", Key_arccsch, 2, 2, 1, 1},  {"arccoth", Key_arccoth, 2, 3, 1, 1}
};

//static DPushButton *createSpecialKeyButton(ComboBox::Buttons key)
//{
//    IconButton *button = new IconButton;

//    QString path;
//    if (DGuiApplicationHelper::instance()->themeType() == 2)
//        path = QString(":/images/%1/").arg("dark");
//    else
//        path = QString(":/images/%1/").arg("light");

//    if (key == ComboBox::Key_Div) {
//        button->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
//    } else if (key == ComboBox::Key_Mult) {
//        button->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
//    } else if (key == ComboBox::Key_Min) {
//        button->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
//    } else if (key == ComboBox::Key_Plus) {
//        button->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
//    } else if (key == ComboBox::Key_Backspace) {
//        button->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg");
//    } else if (key == ComboBox::Key_Clear) {
//        button->setIconUrl(path + "AC_normal.svg", path + "AC_hover.svg", path + "AC_press.svg");
//    } else if (key == ComboBox::Key_Percent) {
//        button->setIconUrl(path + "%_normal.svg", path + "%_hover.svg", path + "%_press.svg");
//    }
//    //connect(button, &IconButton::updateInterface, this, &SciBasicKeyPad::updateInterface);
//    return button;
//}

ComboBox::ComboBox(DComboBox *parent)
    : DComboBox(parent)
    , m_stacklayout(new QStackedLayout)
    , m_hlayout(new QHBoxLayout(this))
    , m_gridlayout1(new QGridLayout)
    , m_gridlayout2(new QGridLayout)
    , m_gridlayout3(new QGridLayout)
    , m_gridlayout4(new QGridLayout)
    , m_listwidget(new DListWidget)
    , m_mapper(new QSignalMapper(this))
{
    widget = new QWidget;
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QWidget *page3 = new QWidget;
    QWidget *page4 = new QWidget;
    page1->setLayout(m_gridlayout1);
    page2->setLayout(m_gridlayout2);
    page3->setLayout(m_gridlayout3);
    page4->setLayout(m_gridlayout4);
    widget->setStyleSheet("QWidget{background-color:transparent;border:none;}");
    m_gridlayout1->setSpacing(0);
    m_gridlayout1->setMargin(0);
    m_gridlayout1->setContentsMargins(0, 0, 0, 0);
    m_gridlayout2->setSpacing(0);
    m_gridlayout2->setMargin(0);
    m_gridlayout2->setContentsMargins(0, 0, 0, 0);
    m_gridlayout3->setSpacing(0);
    m_gridlayout3->setMargin(0);
    m_gridlayout3->setContentsMargins(0, 0, 0, 0);
    m_gridlayout4->setSpacing(0);
    m_gridlayout4->setMargin(0);
    m_gridlayout4->setContentsMargins(0, 0, 0, 0);
    m_stacklayout->addWidget(page1);
    m_stacklayout->addWidget(page2);
    m_stacklayout->addWidget(page3);
    m_stacklayout->addWidget(page4);
    m_hlayout->addLayout(m_stacklayout);
    m_hlayout->setMargin(0);
    m_hlayout->setSpacing(0);
    m_hlayout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(m_hlayout);
    this->setModel(m_listwidget->model());
    this->setView(m_listwidget);
    this->setStyleSheet("QComboBox { width: 100px;}"
                        "QComboBox QAbstractItemView { height: 125px; min-width: 290px; \
                        selection-background-color: transparent; background-color:transparent;}"
                        "QComboBox QAbstractItemView::item { height: 125px; min-width: 290px; }");
    initButtons();
    initUI();
    updateData();

    QPushButton *btn = new QPushButton("三角函数");
    btn->setParent(this);
    btn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    btn->setStyleSheet("QPushButton{border:none;}");
    btn->move(15, 6);

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    connect(this, &ComboBox::buttonPressed, this,
            &ComboBox::turnPage);
    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &SciBasicKeyPad::handleThemeChanged);
}

ComboBox::~ComboBox()
{
}

DPushButton *ComboBox::button(Buttons key)
{
    if (m_stacklayout->currentIndex() == 0) {
        return m_keys.value(key).first;
    } else if (m_stacklayout->currentIndex() == 1) {
        return m_keys1.value(key).first;
    } else if (m_stacklayout->currentIndex() == 2) {
        return m_keys2.value(key).first;
    } else {
        return m_keys3.value(key).first;
    }
}

void ComboBox::animate(Buttons key)
{
    TextButton *btn = static_cast<TextButton *>(button(key));
    btn->animate();
}

void ComboBox::animate()
{
    //m_equal->setChecked(true);

    //QTimer::singleShot(100, this, [=] { m_equal->setChecked(false); });
}

void ComboBox::updateData()
{
    m_listwidget->clear();
    QListWidgetItem *item = new QListWidgetItem(m_listwidget);
    m_listwidget->setItemWidget(item, widget);
//    this->setLineEdit();
    item->setSizeHint(QSize(100, 110));
    item->setFlags(Qt::NoItemFlags);
}

void ComboBox::initButtons()
{

    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;

        button = new TextButton(desc->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_gridlayout1->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ComboBox::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ComboBox::moveRight);
        m_mapper->setMapping(button, desc->button);
    }

    const int count1 = sizeof(keyDescriptions1) / sizeof(keyDescriptions1[0]);
    for (int i = 0; i < count1; ++i) {
        const KeyDescription1 *desc1 = keyDescriptions1 + i;
        DPushButton *button;

        button = new TextButton(desc1->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_gridlayout2->addWidget(button, desc1->row, desc1->column, desc1->rowcount, desc1->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription1 *> hashValue(button, desc1);
        m_keys1.insert(desc1->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ComboBox::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ComboBox::moveRight);
        m_mapper->setMapping(button, desc1->button);
    }

    const int count2 = sizeof(keyDescriptions2) / sizeof(keyDescriptions2[0]);
    for (int i = 0; i < count2; ++i) {
        const KeyDescription2 *desc2 = keyDescriptions2 + i;
        DPushButton *button;

        button = new TextButton(desc2->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_gridlayout3->addWidget(button, desc2->row, desc2->column, desc2->rowcount, desc2->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription2 *> hashValue(button, desc2);
        m_keys2.insert(desc2->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ComboBox::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ComboBox::moveRight);
        m_mapper->setMapping(button, desc2->button);
    }

    const int count3 = sizeof(keyDescriptions3) / sizeof(keyDescriptions3[0]);
    for (int i = 0; i < count3; ++i) {
        const KeyDescription3 *desc3 = keyDescriptions3 + i;
        DPushButton *button;

        button = new TextButton(desc3->text);
        QFont font = button->font();
        font.setFamily("HelveticaNeue");
        button->setFont(font);

        m_gridlayout4->addWidget(button, desc3->row, desc3->column, desc3->rowcount, desc3->columncount,
                                 Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription3 *> hashValue(button, desc3);
        m_keys3.insert(desc3->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &ComboBox::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &ComboBox::moveRight);
        m_mapper->setMapping(button, desc3->button);
    }
}

void ComboBox::initUI()
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

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription2 *>> i2(m_keys2);

    while (i2.hasNext()) {
        i2.next();
        i2.value().first->setFocusPolicy(Qt::NoFocus);
    }

    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription3 *>> i3(m_keys3);

    while (i3.hasNext()) {
        i3.next();
        i3.value().first->setFocusPolicy(Qt::NoFocus);
    }

    this->setContentsMargins(12, 0, 13, 0);
}

void ComboBox::turnPage(int key)
{
    if (m_stacklayout->currentIndex() == 0) {
        if (key == Key_Combo1page) {
            m_stacklayout->setCurrentIndex(1);
        }
        if (key == Key_hyp1) {
            m_stacklayout->setCurrentIndex(2);
        }
    }
    if (m_stacklayout->currentIndex() == 1) {
        if (key == Key_Combo2page) {
            m_stacklayout->setCurrentIndex(0);
        }
        if (key == Key_hyp1) {
            m_stacklayout->setCurrentIndex(3);
        }
    }
    if (m_stacklayout->currentIndex() == 2) {
        if (key == Key_Combo1page) {
            m_stacklayout->setCurrentIndex(3);
        }
        if (key == Key_hyp2) {
            m_stacklayout->setCurrentIndex(0);
        }
    }
    if (m_stacklayout->currentIndex() == 3) {
        if (key == Key_Combo2page) {
            m_stacklayout->setCurrentIndex(2);
        }
        if (key == Key_hyp2) {
            m_stacklayout->setCurrentIndex(1);
        }
    }
}
