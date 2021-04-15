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
const int MAXSIZE = 500; //内存保存最大数

static void s_deletemempub()
{
    delete INSTANCE;
}

/**
 * @brief 实例
 */
MemoryPublicTab *MemoryPublicTab::instance()
{
    if (!INSTANCE) {
        INSTANCE = new MemoryPublicTab();
        qAddPostRoutine(s_deletemempub);
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
    m_standard_ver = new MemoryWidgetTab(2);
    m_scientific_ver = new MemoryWidgetTab(3);
    m_evaluator = Evaluator::instance();
    m_standard_l->setMemoryPublic(this);
    m_scientific_r->setMemoryPublic(this);
    m_standard_ver->setMemoryPublic(this);
    m_scientific_ver->setMemoryPublic(this);

    initconnects();
    memoryclean();
}

/**
 * @brief 相关信号槽
 */
void MemoryPublicTab::initconnects()
{
    connect(m_standard_l, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
    connect(m_scientific_r, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
    connect(m_standard_ver, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
    connect(m_scientific_ver, &MemoryWidgetTab::widgetclean, this, &MemoryPublicTab::widgetclean);
}

/**
 * @brief 根据模式获取不同memorywidget
 */
MemoryWidgetTab *MemoryPublicTab::getwidget(int mode)
{
    switch (mode) {
    case 0:
        return m_standard_l;
    case 1:
        return m_scientific_r;
    case 2:
        return m_standard_ver;
    case 3:
        return m_scientific_ver;
    default:
        return m_scientific_r;
    }
}

QList<Quantity> MemoryPublicTab::getList()
{
    return m_list;
}

/**
 * @brief 向数字内存中同时增加一个数字
 */
void MemoryPublicTab::generateData(Quantity answer)
{
    bool ismax = false;
    if (m_list.count() == MAXSIZE) {
        m_list.pop_back();
        ismax = true;
    }
    emit generateDataSig(answer, ismax);
    m_list.insert(0, answer);
}

/**
 * @brief 同时进行M+操作
 */
void MemoryPublicTab::memoryplus(Quantity answer)
{
    if (!m_list.isEmpty()) {
        Quantity ans;
        ans = m_list.value(0) + answer;
        emit memoryAnsSig(0, ans);
        m_list.replace(0, ans);
    } else
        generateData(answer);
}

/**
 * @brief 同时进行M-操作
 */
void MemoryPublicTab::memoryminus(Quantity answer)
{
    if (!m_list.isEmpty()) {
        Quantity ans;
        ans = m_list.value(0) - answer;
        emit memoryAnsSig(0, ans);
        m_list.replace(0, ans);
    } else
        generateData(Quantity(-1)*answer);
}

/**
 * @brief 同时进行清除内存操作
 */
void MemoryPublicTab::memoryclean()
{
    m_list.clear();
    emit memorycleanSig();
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M+
 */
void MemoryPublicTab::widgetplus(int row, Quantity answer)
{
    if (!answer.isZero()) {
        Quantity ans;
        ans = m_list.value(row) + answer;
        emit memoryAnsSig(row, ans);
        m_list.replace(row, ans);
    }
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M-
 */
void MemoryPublicTab::widgetminus(int row, Quantity answer)
{
    if (!answer.isZero()) {
        Quantity ans;
        ans = m_list.value(row) - answer;
        emit memoryAnsSig(row, ans);
        m_list.replace(row, ans);
    }
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行MC
 */
void MemoryPublicTab::widgetclean(int row, int mode, bool ismenu)
{
    m_list.removeAt(row);
    emit widgetcleanSig(row, mode, ismenu);
}

/**
 * @brief 根据主题变换垃圾桶及内存中没有数据字体颜色
 */
void MemoryPublicTab::setThemeType(int type)
{
    emit setThemeTypeSig(type);
}

/**
 * @brief 返回内存中是否有数据
 */
bool MemoryPublicTab::isEmpty()
{
    return m_list.isEmpty();
}

MemoryPublicTab::~MemoryPublicTab()
{

}
