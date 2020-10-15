#ifndef PROSYSTEMKEYPAD_H
#define PROSYSTEMKEYPAD_H

#include <DWidget>
#include <DGuiApplicationHelper>
#include <QGridLayout>

#include "widgets/probitwidget.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 二进制按钮键盘
 */
class ProSystemKeypad : public DWidget
{
    Q_OBJECT

public:
    ProSystemKeypad(QWidget *parent = nullptr);
    ~ProSystemKeypad();
    void setSystem(int system); //system-64, 32, 16, 8
    void mouseMoveEvent(QMouseEvent *event);

private:
    void initUI();

    QGridLayout *m_layout;
    QHash<int, ProBitWidget *> m_keys; //存储每一块ProBitWidget，以便设置不同位数
};

#endif // PROSYSTEMKEYPAD_H
