/*
* Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROSYSTEMKEYPAD_H
#define PROSYSTEMKEYPAD_H

#include "../../3rdparty/core/evaluator.h"
#include "../widgets/probitwidget.h"

#include <DWidget>
#include <DGuiApplicationHelper>
#include <QGridLayout>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

/**
 * @brief 二进制按钮键盘
 */
class ProSystemKeypad : public DWidget
{
    Q_OBJECT

public:
    explicit ProSystemKeypad(QWidget *parent = nullptr);
    ~ProSystemKeypad() override;
    void setSystem(int system, int oldsystem); //system-64, 32, 16, 8 oldsystem-转换前的位数
    bool buttonHasFocus();
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void valuechanged(QString num);
    void bitbuttonclicked();
    void longbitcut(Quantity ans);

public slots:
    void getFocus(int dir, int key);
    void setvalue(QString num);
    void longBitCut(const QString &num);

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
