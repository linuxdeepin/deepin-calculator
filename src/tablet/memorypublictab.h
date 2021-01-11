/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     jingzhou <jingzhou@uniontech.com>
*
* Maintainer: xiajing <xiajing@uniontech.com>
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

#ifndef MEMORYPUBLICTAB_H
#define MEMORYPUBLICTAB_H

#include <QObject>
#include <QList>

#include "tablet/views/memorywidgettab.h"
#include "core/evaluator.h"

/**
 * @brief 同步处理数字内存
 */
class MemoryPublicTab : public QObject
{
    Q_OBJECT
private:
    explicit MemoryPublicTab(QObject *parent = nullptr);
    void initconnects();
public:
    /**
     * @brief 当前内存界面所在计算器模式
     */
    enum memorymode {
        standardleft,  //标准模式
        scientificright //科学模式
    };
    static MemoryPublicTab *instance(QObject *p);
    static void deleteInstance();
    ~MemoryPublicTab();
    MemoryWidgetTab *getwidget(memorymode mode);

    void generateData(Quantity answer);
    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();
    void widgetplus(int row, Quantity answer);
    void widgetminus(int row, Quantity answer);
    void widgetclean(int row, int mode, bool ismenu);
    void setThemeType(int type);
    bool isWidgetEmpty(int mode); //0-标准 1-科学
private:
    MemoryWidgetTab *m_standard_l;
    MemoryWidgetTab *m_scientific_r;


signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void filledMem();
    void emptyMem();
    void publicwidgetclean(int mode);

public slots:
};

#endif // MEMORYPUBLICTAB_H
