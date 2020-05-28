#include "memorykeypad.h"
#include <QTimer>
#include <DPalette>
#include <DImageButton>

#include "dthememanager.h"

const MemoryKeypad::KeyDescription MemoryKeypad::keyDescriptions[] = {
    {"MC", Key_MC, 1, 0, 1, 1},       {"MR", Key_MR, 1, 1, 1, 1},
    {"M+", Key_Mplus, 1, 2, 1, 1},    {"M-", Key_Mminus, 1, 3, 1, 1},
    {"MS", Key_MS, 1, 4, 1, 1},       {"M˅", Key_Mlist, 1, 5, 1, 1},
};

static DPushButton *createSpecialKeyButton(MemoryKeypad::Buttons key)
{
    IconButton *button = new IconButton;

    QString path;
    if (DGuiApplicationHelper::instance()->themeType() == 2)
        path = QString(":/src/assets/images/%1/").arg("dark");
    else
        path = QString(":/src/assets/images/%1/").arg("light");

//    if (key == BasicKeypad::Key_Div) {
//        button->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
//    } else if (key == BasicKeypad::Key_Mult) {
//        button->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
//    } else if (key == BasicKeypad::Key_Min) {
//        button->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
//    } else if (key == BasicKeypad::Key_Plus) {
//        button->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
//    } else if (key == BasicKeypad::Key_Backspace) {
//        button->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg");
//    } else if (key == BasicKeypad::Key_Clear) {
//        button->setIconUrl(path + "AC_normal.svg", path + "AC_hover.svg", path + "AC_press.svg");
//    } else if (key == BasicKeypad::Key_Percent) {
//        button->setIconUrl(path + "%_normal.svg", path + "%_hover.svg", path + "%_press.svg");
//    } else if (key == BasicKeypad::Key_Brackets) {
//        button->setIconUrl(path + "( )_normal.svg", path + "( )_hover.svg", path + "( )_press.svg");
//    }
    //connect(button, &IconButton::updateInterface, this, &BasicKeypad::updateInterface);
    return button;
}

MemoryKeypad::MemoryKeypad(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    this->setFixedHeight(33);
    m_layout->setMargin(0);
    m_layout->setSpacing(1);
    m_layout->setContentsMargins(0, 0, 0, 0);

    initButtons();
    initUI();

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &BasicKeypad::handleThemeChanged);
}

MemoryKeypad::~MemoryKeypad()
{
}

DPushButton *MemoryKeypad::button(Buttons key)
{
    return m_keys.value(key).first;
}

void MemoryKeypad::animate(Buttons key)
{
    MemoryButton *btn = static_cast<MemoryButton *>(button(key));
    btn->animate();
}

void MemoryKeypad::animate()
{
    //m_equal->setChecked(true);

    //QTimer::singleShot(100, this, [=] { m_equal->setChecked(false); });
}

void MemoryKeypad::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;
        if (desc->text.isEmpty()) {
            button = createSpecialKeyButton(desc->button);
        } else {
            button = new MemoryButton(desc->text);
            QFont font = button->font();
            font.setFamily("Noto Sans CJK SC");
            button->setFont(font);
        }

        m_layout->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                            Qt::AlignHCenter | Qt::AlignBottom);
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue);

        connect(static_cast<TextButton *>(button), &TextButton::updateInterface, [ = ] {update();});
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &MemoryKeypad::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &MemoryKeypad::moveRight);
        m_mapper->setMapping(button, desc->button);
    }
}

void MemoryKeypad::initUI()
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy(Qt::NoFocus);
    }

    this->setContentsMargins(12, 0, 13, 0);
}

void MemoryKeypad::buttonThemeChanged(int type)
{
    QString path;
    if (type == 2)
        path = QString(":/src/assets/images/%1/").arg("dark");
    else
        path = QString(":/src/assets/images/%1/").arg("light");

//    IconButton *btn = static_cast<IconButton *>(button(Key_Div));
//    btn->setIconUrl(path + "divide_normal.svg", path + "divide_hover.svg", path + "divide_press.svg");
//    btn = static_cast<IconButton *>(button(Key_Mult));
//    btn->setIconUrl(path + "x_normal.svg", path + "x_hover.svg", path + "x_press.svg");
//    btn = static_cast<IconButton *>(button(Key_Min));
//    btn->setIconUrl(path + "-_normal.svg", path + "-_hover.svg", path + "-_press.svg");
//    btn = static_cast<IconButton *>(button(Key_Plus));
//    btn->setIconUrl(path + "+_normal.svg", path + "+_hover.svg", path + "+_press.svg");
//    btn = static_cast<IconButton *>(button(Key_Backspace));
//    btn->setIconUrl(path + "clear_normal.svg", path + "clear_hover.svg", path + "clear_press.svg");
}
