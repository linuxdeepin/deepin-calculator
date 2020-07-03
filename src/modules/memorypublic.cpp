/*
 * 1. @类名:    MemoryPublic
 * 2. @作者:    京洲 ut000490
 * 3. @日期:    2020-07-01
 * 4. @说明:    同步处理数字内存
 */
#include "memorypublic.h"

static MemoryPublic *INSTANCE = nullptr;

MemoryPublic *MemoryPublic::instance(QObject *p)
{
    if (!INSTANCE) {
        INSTANCE = new MemoryPublic(p);
    }
    return INSTANCE;
}

MemoryPublic::MemoryPublic(QObject *parent)
    : QObject(parent)
{
    m_standard_l = new MemoryWidget(0);
    m_scientific_r = new MemoryWidget(1);
    initconnects();
}

void MemoryPublic::initconnects()
{
//    connect(standard_l, &MemoryWidget::widgetplus, this, [ = ](int row) {
//        emit widgetplus(row);
//    });
//    connect(scientific_r, &MemoryWidget::widgetplus, this, [ = ](int row) {
//        emit widgetplus(row);
//    });
//    connect(standard_l, &MemoryWidget::widgetminus, this, [ = ](int row) {
//        emit widgetminus(row);
//    });
//    connect(scientific_r, &MemoryWidget::widgetminus, this, [ = ](int row) {
//        emit widgetminus(row);
//    });
    connect(m_standard_l, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(m_scientific_r, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(m_standard_l, &MemoryWidget::memorycleansignal, this, &MemoryPublic::memoryclean);
    connect(m_standard_l, &MemoryWidget::mListAvailable, this, &MemoryPublic::filledMem);
    connect(m_standard_l, &MemoryWidget::mListUnavailable, this, &MemoryPublic::emptyMem);
}

MemoryWidget *MemoryPublic::getwidget(memorymode mode)
{
    switch (mode) {
    case standardleft:
        return m_standard_l;
    default:
        return m_scientific_r;
    }
}

void MemoryPublic::generateData(Quantity answer)
{
    m_standard_l->generateData(answer);
    m_scientific_r->generateData(answer);
}

void MemoryPublic::memoryplus(Quantity answer)
{
    m_standard_l->memoryplus(answer);
    m_scientific_r->memoryplus(answer);
}

void MemoryPublic::memoryminus(Quantity answer)
{
    m_standard_l->memoryminus(answer);
    m_scientific_r->memoryminus(answer);
}

void MemoryPublic::memoryclean()
{
    m_standard_l->memoryclean();
    m_scientific_r->memoryclean();
}

void MemoryPublic::widgetplus(int row, Quantity answer)
{
    m_standard_l->widgetplusslot(row, answer);
    m_scientific_r->widgetplusslot(row, answer);
}

void MemoryPublic::widgetminus(int row, Quantity answer)
{
    m_standard_l->widgetminusslot(row, answer);
    m_scientific_r->widgetminusslot(row, answer);
}

void MemoryPublic::widgetclean(int row, int mode)
{
    m_standard_l->widgetcleanslot(row, mode);
    m_scientific_r->widgetcleanslot(row, mode);
}

void MemoryPublic::setThemeType(int type)
{
    m_standard_l->setThemeType(type);
    m_scientific_r->setThemeType(type);
}

MemoryPublic::~MemoryPublic()
{

}
