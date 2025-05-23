// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "memorypublic.h"

#include "utils.h"
#include "../3rdparty/core/settings.h"

static MemoryPublic *INSTANCE = nullptr;
const int MAXSIZE = 500; //内存保存最大数

static void s_deletemempub()
{
    delete INSTANCE;
    INSTANCE = nullptr;
}

/**
 * @brief 实例
 */
MemoryPublic *MemoryPublic::instance()
{
    qDebug() << "MemoryPublic instance requested";
    if (!INSTANCE) {
        qInfo() << "Creating new MemoryPublic instance";
        INSTANCE = new MemoryPublic;
        qAddPostRoutine(s_deletemempub);
    }
    return INSTANCE;
}

void MemoryPublic::deleteInstance()
{
    qDebug() << "Deleting MemoryPublic instance";
    delete INSTANCE;
    INSTANCE = nullptr;
}

MemoryPublic::MemoryPublic(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Initializing MemoryPublic";
    m_standard_l = new MemoryWidget(0);
    m_scientific_r = new MemoryWidget(1);
    m_programmer_l = new MemoryWidget(2);
    m_evaluator = Evaluator::instance();
    m_standard_l->setMemoryPublic(this);
    m_scientific_r->setMemoryPublic(this);
    m_programmer_l->setMemoryPublic(this);
    initconnects();
    memoryclean();
    qInfo() << "MemoryPublic initialized with 3 memory widgets";
}

/**
 * @brief 相关信号槽
 */
void MemoryPublic::initconnects()
{
    connect(m_standard_l, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(m_scientific_r, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(m_programmer_l, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
}

/**
 * @brief 根据模式获取不同memorywidget
 */
MemoryWidget *MemoryPublic::getwidget(memorymode mode)
{
    switch (mode) {
    case standardleft:
        return m_standard_l;
    case programmerleft:
        return m_programmer_l;
    default:
        return m_scientific_r;
    }
}

QList<Quantity> MemoryPublic::getList()
{
    return m_list;
}

/**
 * @brief 向数字内存中同时增加一个数字
 */
void MemoryPublic::generateData(Quantity answer)
{
    qDebug() << "Generating memory data";
    bool ismax = false;
    if (m_list.count() == MAXSIZE) {
        qInfo() << "Memory list reached max size (" << MAXSIZE << "), removing oldest entry";
        m_list.pop_back();
        ismax = true;
    }
    emit generateDataSig(answer, ismax);
    m_list.insert(0, answer);
    qDebug() << "Memory data generated, new list size:" << m_list.count();
}

/**
 * @brief 同时进行M+操作
 */
void MemoryPublic::memoryplus(Quantity answer)
{
    qDebug() << "Memory plus operation requested.";
    if (!m_list.isEmpty()) {
        Quantity ans;
        ans = m_list.value(0) + answer;
        emit memoryAnsSig(0, ans);
        m_list.replace(0, ans);
    } else {
        qDebug() << "Memory list empty, generating new data";
        generateData(answer);
    }
}

/**
 * @brief 同时进行M-操作
 */
void MemoryPublic::memoryminus(Quantity answer)
{
    qDebug() << "Memory minus operation requested.";
    if (!m_list.isEmpty()) {
        Quantity ans;
        ans = m_list.value(0) - answer;
        emit memoryAnsSig(0, ans);
        m_list.replace(0, ans);
    } else {
        qDebug() << "Memory list empty, generating new data";
        generateData(Quantity(-1)*answer);
    }

}

/**
 * @brief 同时进行清除内存操作
 */
void MemoryPublic::memoryclean()
{
    qInfo() << "Clearing all memory data";
    m_list.clear();
    emit memorycleanSig();
}

/**
 * @brief 用于从列表中item里的按钮，对指定行号的内存数据进行M+
 */
void MemoryPublic::widgetplus(int row, Quantity answer)
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
void MemoryPublic::widgetminus(int row, Quantity answer)
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
void MemoryPublic::widgetclean(int row, int mode, bool ismenu)
{
    m_list.removeAt(row);
    emit widgetcleanSig(row, mode, ismenu);
}

/**
 * @brief 根据主题变换垃圾桶及内存中没有数据字体颜色
 */
void MemoryPublic::setThemeType(int type)
{
    emit setThemeTypeSig(type);
}

/**
 * @brief 返回内存中是否有数据
 */
bool MemoryPublic::isEmpty()
{
    bool empty = m_list.isEmpty();
    qDebug() << "Memory empty check:" << empty;
    return empty;
}

MemoryPublic::~MemoryPublic()
{

}
