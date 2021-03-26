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

#ifndef MEMORYPUBLIC_H
#define MEMORYPUBLIC_H

#include <QObject>
#include <QList>

#include "../3rdparty/core/evaluator.h"
#include "views/memorywidget.h"

/**
 * @brief 同步处理数字内存
 */
class MemoryPublic : public QObject
{
    Q_OBJECT
private:
    explicit MemoryPublic(QObject *parent = nullptr);
    void initconnects();
public:
    /**
     * @brief 当前内存界面所在计算器模式
     */
    enum memorymode {
        standardleft,  //标准模式
        scientificright, //科学模式
        programmerleft //程序员
    };
    static MemoryPublic *instance();
    static void deleteInstance();
    ~MemoryPublic();
    MemoryWidget *getwidget(memorymode mode);
    QList<Quantity> getList();

    void generateData(Quantity answer);
    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();
    void widgetplus(int row, Quantity answer);
    void widgetminus(int row, Quantity answer);
    void widgetclean(int row, int mode, bool ismenu);
    void setThemeType(int type);
    bool isEmpty();
private:
    MemoryWidget *m_standard_l;
    MemoryWidget *m_scientific_r;
    MemoryWidget *m_programmer_l;

    QList<Quantity> m_list;
    Evaluator *m_evaluator;

signals:
    void generateDataSig(Quantity answer, bool ismax);
    void memorycleanSig();
    void memoryAnsSig(int row, Quantity answer);
    void widgetcleanSig(int row, int mode, bool ismenu);
    void setThemeTypeSig(int type);

public slots:
};

#endif // MEMORYPUBLIC_H
