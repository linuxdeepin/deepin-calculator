#ifndef PROBITWIDGET_H
#define PROBITWIDGET_H

#include <DWidget>
#include <DLabel>
#include <DGuiApplicationHelper>

#include "control/bitbutton.h"

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 一块二进制键盘控件
 */
class ProBitWidget : public DWidget
{
    Q_OBJECT

public:
    ProBitWidget(QWidget *parent = nullptr);
    ~ProBitWidget();
    void setButtonEnabled(bool enabled);

private:
    BitButton *m_firstbtn;
    BitButton *m_secondbtn;
    BitButton *m_thirdbtn;
    BitButton *m_fourthbtn;
    DLabel *m_label;
};

#endif // PROBITWIDGET_H
