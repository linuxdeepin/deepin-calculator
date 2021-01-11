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

#include "memorypublictab.h"

static MemoryPublicTab *INSTANCE = nullptr;

/**
 * @brief 实例
 */
MemoryPublicTab *MemoryPublicTab::instance(QObject *p)
{
    if (!INSTANCE) {
        INSTANCE = new MemoryPublicTab(p);
    }
    return INSTANCE;
}

void MemoryPublicTab::deleteInstance()
{
    delete INSTANCE;
    INSTANCE = nullptr;
}

MemoryPublicTab::MemoryPublicTab(QObject *parent)
    : QObject(parent)
{
    m_standard_l = new MemoryWidgetTab(0);
    m_scientific_r = new MemoryWidgetTab(1);

    initconnects();
}

/**
 * @brief 相关信号槽
 */
void MemoryPublicTab::initconnects()
{
    connect(m_standard_l, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
    connect(m_standard_l, &MemoryWidgetTab::mListAvailable, this, &MemoryPublicTab::filledMem);
    connect(m_standard_l, &MemoryWidgetTab::mListUnavailable, this, &MemoryPublicTab::emptyMem);
    connect(m_scientific_r, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
    connect(m_scientific_r, &MemoryWidgetTab::mListAvailable, this, &MemoryPublicTab::filledMem);
    connect(m_scientific_r, &MemoryWidgetTab::mListUnavailable, this, &MemoryPublicTab::emptyMem);
}

/**
 * @brief 根据模式获取不同memorywidget
 */
MemoryWidgetTab *MemoryPublicTab::getwidget(memorymode mode)
{
    switch (mode) {
    case standardleft:
        return m_standard_l;
    default:
        return m_scientific_r;
    }
}

/**
 * @brief 向数字内存中同时增加一个数字
 */
void MemoryPublicTab::generateData(Quantity answer)
{
    m_standard_l->generateData(answer);
    m_scientific_r->generateData(answer);
}

/**
 * @brief 同时进行M+操作
 */
void MemoryPublicTab::memoryplus(Quantity answer)
{
    m_standard_l->memoryplus(answer);
    m_scientific_r->memoryplus(answer);
}

/**
 * @brief 同时进行M-操作
 */
void MemoryPublicTab::memoryminus(Quantity answer)
{
    m_standard_l->memoryminus(answer);
    m_scientific_r->memoryminus(answer);
}

/**
 * @brief 同时进行清除内存操作
 */
void MemoryPublicTab::memoryclean()
{
    m_standard_l->memoryclean();
    m_scientific_r->memoryclean();
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M+
 */
void MemoryPublicTab::widgetplus(int row, Quantity answer)
{
    m_standard_l->widgetplusslot(row, answer);
    m_scientific_r->widgetplusslot(row, answer);
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M-
 */
void MemoryPublicTab::widgetminus(int row, Quantity answer)
{
    m_standard_l->widgetminusslot(row, answer);
    m_scientific_r->widgetminusslot(row, answer);
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行MC
 */
void MemoryPublicTab::widgetclean(int row, int mode, bool ismenu)
{
    m_standard_l->widgetcleanslot(row, mode, ismenu);
    m_scientific_r->widgetcleanslot(row, mode, ismenu);
    emit publicwidgetclean(mode);
}

/**
 * @brief 根据主题变换垃圾桶及内存中没有数据字体颜色
 */
void MemoryPublicTab::setThemeType(int type)
{
    m_standard_l->setThemeType(type);
    m_scientific_r->setThemeType(type);
}

/**
 * @brief 返回内存中是否有数据
 */
bool MemoryPublicTab::isWidgetEmpty(int mode)
{
    return (mode == 0 ? m_scientific_r->isWidgetEmpty() : m_standard_l->isWidgetEmpty());
}

MemoryPublicTab::~MemoryPublicTab()
{

}
