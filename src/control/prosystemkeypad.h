#ifndef PROSYSTEMKEYPAD_H
#define PROSYSTEMKEYPAD_H

#include <DWidget>
#include <DGuiApplicationHelper>
#include <QGridLayout>

#include "widgets/probitwidget.h"
//#include "control/bitbutton.h"

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
    void setSystem(int system, int oldsystem); //system-64, 32, 16, 8 oldsystem-转换前的位数
    bool buttonHasFocus();
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void valuechanged(QString num);
    void bitbuttonclicked();

public slots:
    void getFocus(int dir, int key);
    void setvalue(QString num);

private:
    void initUI();
    void initconnects();
    void changeBinaryValue(int i);

    QGridLayout *m_layout;
    QHash<int, ProBitWidget *> m_keys; //存储每一块ProBitWidget，以便设置不同位数
    QHash<int, BitButton *> m_buttons; //存储所有按键
//    QChar m_binaryValue[64];
    QString m_binaryValue;
};

#endif // PROSYSTEMKEYPAD_H
