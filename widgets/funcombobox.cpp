#include <QTimer>

#include <DPalette>
#include <DImageButton>
#include <QDebug>

#include "dthememanager.h"
#include "funcombobox.h"

const FunCombobox::KeyDescription FunCombobox::keyDescriptions[] = {
    {"a", Key_abs, 1, 0, 1, 1}, {"b", Key_down, 1, 1, 1, 1}, {"c", Key_up, 1, 2, 1, 1},

    {"d", Key_rand, 2, 0, 1, 1},  {"e", Key_dms, 2, 1, 1, 1}, {"f", Key_deg, 2, 2, 1, 1},
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

FunCombobox::FunCombobox(DComboBox *parent)
    : DComboBox(parent),
      m_hlayout(new QHBoxLayout(this)),
      m_mapper(new QSignalMapper(this)),
      m_stacklayout(new QStackedLayout),
      m_gridlayout1(new QGridLayout),
      m_listwidget(new DListWidget)
{
    widget = new QWidget;
    widget->setLayout(m_gridlayout1);
    widget->setStyleSheet("QWidget{background-color:transparent;border:none;}");
    m_gridlayout1->setSpacing(0);
    m_gridlayout1->setMargin(0);
    m_gridlayout1->setContentsMargins(0, 0, 0, 0);
    this->setModel(m_listwidget->model());
    this->setView(m_listwidget);
    m_listwidget->setFrameShape(QFrame::NoFrame);
    this->setStyleSheet("QComboBox { width: 80px;}"
                        "QComboBox QAbstractItemView { height: 60px; min-width: 210px; \
                        selection-background-color: transparent; background-color:transparent;}"
                        "QComboBox QAbstractItemView::item { height: 60px; min-width: 210px; }");
    initButtons();
    initUI();
    updateData();

    DPushButton *btn = new DPushButton("函数");
    btn->raise();
    btn->setParent(this);
    btn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    btn->setStyleSheet("QPushButton{border:none;}");
    btn->setFixedSize(50, 20);
    btn->move(20, 7);

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
    //connect(DThemeManager::instance(), &DThemeManager::themeChanged, this, &SciBasicKeyPad::handleThemeChanged);
}

FunCombobox::~FunCombobox()
{
}

DPushButton *FunCombobox::button(Buttons key)
{
    return m_keys.value(key).first;
}

DSuggestButton *FunCombobox::button()
{
    //return m_equal;
}

void FunCombobox::animate(Buttons key)
{
    TextButton *btn = static_cast<TextButton *>(button(key));
    btn->animate();
}

void FunCombobox::animate()
{
    //m_equal->setChecked(true);

    //QTimer::singleShot(100, this, [=] { m_equal->setChecked(false); });
}

void FunCombobox::updateData()
{
    m_listwidget->clear();
    QListWidgetItem *item = new QListWidgetItem(m_listwidget);
    m_listwidget->setItemWidget(item, widget);
    item->setSizeHint(QSize(90, 90));
    item->setFlags(Qt::NoItemFlags);
}

void FunCombobox::initButtons()
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
        connect(static_cast<TextButton *>(button), &TextButton::moveLeft, this, &FunCombobox::moveLeft);
        connect(static_cast<TextButton *>(button), &TextButton::moveRight, this, &FunCombobox::moveRight);
        m_mapper->setMapping(button, desc->button);
    }
}

void FunCombobox::initUI()
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);

    while (i.hasNext()) {
        i.next();
        i.value().first->setFocusPolicy(Qt::NoFocus);
    }

    this->setContentsMargins(12, 0, 13, 0);
}

void FunCombobox::buttonThemeChanged(int type)
{
}
