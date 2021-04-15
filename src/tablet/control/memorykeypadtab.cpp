#include "memorykeypadtab.h"

const QSize MEMORYBUTTON_SIZE = QSize(279, 87); //标准模式大小，为画边框比ui大2pix
const QSize MEMORYBUTTONVER_SIZE = QSize(167, 182); //竖屏大小

const MemoryKeypadTab::KeyDescription MemoryKeypadTab::keyDescriptions[] = {
    {"MC", Key_MC, 1, 0, 1, 1},       {"MR", Key_MR, 1, 1, 1, 1},
    {"M+", Key_Mplus, 1, 2, 1, 1},    {"M-", Key_Mminus, 1, 3, 1, 1},
    {"MS", Key_MS, 1, 4, 1, 1},       {"M˅", Key_Mlist, 1, 5, 1, 1},
};

MemoryKeypadTab::MemoryKeypadTab(QWidget *parent)
    : DWidget(parent),
      m_layout(new QGridLayout(this)),
      m_mapper(new QSignalMapper(this))
{
    this->setFixedHeight(95);
    m_layout->setMargin(0);
    m_layout->setSpacing(8);  //按钮比ui大2pix,此处比ui小2pix
    m_layout->setContentsMargins(0, 0, 0, 0);

    initButtons();
    this->setContentsMargins(19, 0, 19, 0);

    connect(m_mapper, SIGNAL(mapped(int)), SIGNAL(buttonPressed(int)));
}

MemoryKeypadTab::~MemoryKeypadTab()
{
}

void MemoryKeypadTab::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

/**
 * @brief 根据枚举值返回相应button
 * @param key 按钮名
 */
DPushButton *MemoryKeypadTab::button(Buttons key)
{
    return m_keys.value(key).first;
}

DPushButton *MemoryKeypadTab::button(int key)
{
    return m_keys.value(Buttons(key)).first;
}

/**
 * @brief 按钮点击动画效果
 */
void MemoryKeypadTab::animate(Buttons key, bool isspace)
{
    MemoryButtonTab *btn = static_cast<MemoryButtonTab *>(button(key));
    btn->animate(isspace);
}

bool MemoryKeypadTab::buttonHasFocus()
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

/**
 * @brief 初始化button
 */
void MemoryKeypadTab::initButtons()
{
    const int count = sizeof(keyDescriptions) / sizeof(keyDescriptions[0]);
    for (int i = 0; i < count; ++i) {
        const KeyDescription *desc = keyDescriptions + i;
        DPushButton *button;
        button = new MemoryButtonTab(desc->text, false, this);
        QFont font = button->font();
        font.setFamily("Noto Sans");
        button->setFont(font);

        button->setFixedSize(MEMORYBUTTON_SIZE);
        m_layout->addWidget(button, desc->row, desc->column, desc->rowcount, desc->columncount,
                            Qt::AlignTop);
        const QPair<DPushButton *, const KeyDescription *> hashValue(button, desc);
        m_keys.insert(desc->button, hashValue); //key为枚举值，value.first为DPushButton *, value.second为const KeyDescription *

        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::focus, this, &MemoryKeypadTab::getFocus); //获取上下左右键
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::updateInterface, [ = ] {update();}); //点击及焦点移除时update
        connect(button, &DPushButton::clicked, m_mapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::space, this, [ = ]() {
            Buttons spacekey = m_keys.key(hashValue);
            emit buttonPressedbySpace(spacekey);
        });
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::moveLeft, this, &MemoryKeypadTab::moveLeft);
        connect(static_cast<MemoryButtonTab *>(button), &MemoryButtonTab::moveRight, this, &MemoryKeypadTab::moveRight);
        m_mapper->setMapping(button, desc->button); //多个按钮绑定到一个mapper上
    }
}

/**
 * @brief 获取button上下左右键信号setfocus
 * @param direction 0-上　1-下　2-左　3-右
 */
void MemoryKeypadTab::getFocus(int direction)
{
    QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
    while (i.hasNext()) {
        i.next();
        if (i.value().first && i.value().first->hasFocus()) {
            break; //获取焦点按钮
        }
    }
    switch (direction) {
    case 2:
        if ((i.key() - 20) % 5 > 0)
            button(static_cast<Buttons>(i.key() - 1))->setFocus();
        break;
    case 3:
        if ((i.key() - 20) % 5 < 4)
            button(static_cast<Buttons>(i.key() + 1))->setFocus();
        break;
    default:
        break;
    }
}

void MemoryKeypadTab::resetWidgetSize(QSize size)
{
    if (size.width() < size.height()) {
        this->setFixedHeight(190 * size.height() / 1880);
        QSize btnsize;
        btnsize.setWidth(167 * size.width() / 1080);
        btnsize.setHeight(182 * size.height() / 1880);
        QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
        while (i.hasNext()) {
            i.next();
            if (i.value().first->text() == "M˅")
                i.value().first->show();
            i.value().first->setFixedSize(btnsize);
        }
    } else {
        this->setFixedHeight(95 * size.height() / 1055);
        QSize btnsize;
        btnsize.setWidth(279 * size.width() / 1920);
        btnsize.setHeight(87 * size.height() / 1055);
        QHashIterator<Buttons, QPair<DPushButton *, const KeyDescription *>> i(m_keys);
        while (i.hasNext()) {
            i.next();
            if (i.value().first->text() == "M˅")
                i.value().first->hide();
            i.value().first->setFixedSize(btnsize);
        }
    }

}
