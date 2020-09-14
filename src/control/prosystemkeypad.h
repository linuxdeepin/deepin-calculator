#ifndef PROSYSTEMKEYPAD_H
#define PROSYSTEMKEYPAD_H

#include <DWidget>
#include <QGridLayout>
#include <DGuiApplicationHelper>

#include "src/widgets/probitwidget.h"

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

private:
    void initUI();

    QGridLayout *m_layout;
};

#endif // PROSYSTEMKEYPAD_H
