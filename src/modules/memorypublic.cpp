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
    standard_l = new MemoryWidget(0);
    scientific_r = new MemoryWidget(1);
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
    connect(standard_l, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(scientific_r, &MemoryWidget::widgetclean, this, &MemoryPublic::widgetclean);
    connect(standard_l, &MemoryWidget::memorycleansig, this, &MemoryPublic::memoryclean);
    connect(standard_l, &MemoryWidget::mListAvailable, this, &MemoryPublic::filledMem);
    connect(standard_l, &MemoryWidget::mListUnavailable, this, &MemoryPublic::emptyMem);
}

MemoryWidget *MemoryPublic::getwidget(memorymode mode)
{
    switch (mode) {
    case standardleft:
        return standard_l;
    case scientificright:
        return scientific_r;
    }
}

void MemoryPublic::generateData(Quantity answer)
{
    standard_l->generateData(answer);
    scientific_r->generateData(answer);
}

void MemoryPublic::memoryplus(Quantity answer)
{
    standard_l->memoryplus(answer);
    scientific_r->memoryplus(answer);
}

void MemoryPublic::memoryminus(Quantity answer)
{
    standard_l->memoryminus(answer);
    scientific_r->memoryminus(answer);
}

void MemoryPublic::memoryclean()
{
    standard_l->memoryclean();
    scientific_r->memoryclean();
}

void MemoryPublic::widgetplus(int row, Quantity answer)
{
    standard_l->widgetplusslot(row, answer);
    scientific_r->widgetplusslot(row, answer);
}

void MemoryPublic::widgetminus(int row, Quantity answer)
{
    standard_l->widgetminusslot(row, answer);
    scientific_r->widgetminusslot(row, answer);
}

void MemoryPublic::widgetclean(int row, int mode)
{
    standard_l->widgetcleanslot(row, mode);
    scientific_r->widgetcleanslot(row, mode);
}

void MemoryPublic::setThemeType(int type)
{
    standard_l->setThemeType(type);
    scientific_r->setThemeType(type);
}

MemoryPublic::~MemoryPublic()
{

}
