#include "memhiskeypadtab.h"

const MemHisKeypadTab::KeyDescription MemHisKeypadTab::keyDescriptions[] = {
    {"F-E", Key_FE, 1, 0, 1, 1},
    {"MC", Key_MC, 1, 1, 1, 1},       {"MR", Key_MR, 1, 2, 1, 1},
    {"M+", Key_Mplus, 1, 3, 1, 1},    {"M-", Key_Mmin, 1, 4, 1, 1},
    {"MS", Key_MS, 1, 5, 1, 1},       {"MH˅", Key_MHlist, 1, 6, 1, 1},
};

MemHisKeypadTab::MemHisKeypadTab(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    this->setFixedHeight(165);
    m_layout->setMargin(0);
    m_layout->setSpacing(8);  //按钮比ui大2pix,此处比ui小2pix
    m_layout->setContentsMargins(0, 0, 0, 0);

    initButtons();
    this->setContentsMargins(19, 0, 19, 0);

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
}

MemHisKeypadTab::~MemHisKeypadTab()
{

}

void MemHisKeypadTab::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

DPushButton *MemHisKeypadTab::button(MemHisKeypadTab::Buttons key)
{
    return m_keys.value(key).first;
}

DPushButton *MemHisKeypadTab::button(int key)
{
    return m_keys.value(Buttons(key)).first;
}

void MemHisKeypadTab::animate(MemHisKeypadTab::Buttons key, bool isspace)
{
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(button(key));
    btn->animate(isspace);
}

bool MemHisKeypadTab::buttonHasFocus()
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
    while (i.hasNext()) {
        i.next();
        if (i.value().first && i.value().first->hasFocus()) {
            return true;
        }
    }
    return false;
}

void MemHisKeypadTab::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;
        if (desc->text == "F-E")
            button = new TextButtonTab(desc->text, false, this);
        else
            button = new MemoryButtonTab(desc->text, false, this);
        QFont font = button->font();
        font.setFamily("Noto Sans");
        button->setFont(font);

        button->setFixedSize(142, 157);
        m_layout->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                            Qt::AlignTop);
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue); //key为枚举值，value.first为DPushButton *, value.second为const KeyDescription *

        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::focus, this, &MemHisKeypadTab::getFocus); //获取上下左右键
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::updateInterface, [ = ] {update();}); //点击及焦点移除时update
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::space, this, [ = ]() {
            Buttons spacekey = m_keys.key(hashValue);
            emit buttonPressedbySpace(spacekey);
        });
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::moveLeft, this, &MemHisKeypadTab::moveLeft);
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::moveRight, this, &MemHisKeypadTab::moveRight);
        m_mapper->setMapping(button, desc->button); //多个按钮绑定到一个mapper上
    }
}

void MemHisKeypadTab::getFocus(int direction)
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
    while (i.hasNext()) {
        i.next();
        if (i.value().first->hasFocus()) {
            break; //获取焦点按钮
        }
    }
    switch (direction) {
    case 2:
        if ((i.key() - 52) % 7 > 0) {
            int enablekey = i.key() - 1;
            while (!button(static_cast<Buttons>(enablekey))->isEnabled() && enablekey > 52) {
                enablekey -= 1;
            }
            button(static_cast<Buttons>(enablekey))->setFocus();
        }
        break;
    case 3:
        if ((i.key() - 52) % 7 < 6) {
            int enablekey = i.key() + 1;
            while (!button(static_cast<Buttons>(enablekey))->isEnabled() && enablekey < 58) {
                enablekey += 1;
            }
            button(static_cast<Buttons>(enablekey))->setFocus();
        }
        break;
    default:
        break;
    }
}

void MemHisKeypadTab::resetWidgetSize(QSize size)
{
    this->setFixedHeight(165 * size.height() / 1880);
    QSize btnsize;
    btnsize.setWidth(142 * size.width() / 1080);
    btnsize.setHeight(157 * size.height() / 1880);
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
    while (i.hasNext()) {
        i.next();
        i.value().first->setFixedSize(btnsize);
    }
}




