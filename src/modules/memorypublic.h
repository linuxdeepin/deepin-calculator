/*
 * 1. @类名:    MemoryPublic
 * 2. @作者:    京洲 ut000490
 * 3. @日期:    2020-07-01
 * 4. @说明:    同步处理数字内存
 */
#ifndef MEMORYPUBLIC_H
#define MEMORYPUBLIC_H

#include <QObject>
#include <QList>
#include "memorywidget.h"
#include "../core/evaluator.h"

class MemoryPublic : public QObject
{
    Q_OBJECT
private:
    explicit MemoryPublic(QObject *parent = nullptr);
    void initconnects();
public:
    enum memorymode {
        standardleft, scientificright
    };
    static MemoryPublic *instance(QObject *p);
    ~MemoryPublic();
    MemoryWidget *getwidget(memorymode mode);

    void generateData(Quantity answer);
    void memoryplus(Quantity answer);
    void memoryminus(Quantity answer);
    void memoryclean();
    void widgetplus(int row, Quantity answer);
    void widgetminus(int row, Quantity answer);
    void widgetclean(int row, int mode);
    void setThemeType(int type);
private:
    MemoryWidget *m_standard_l;
    MemoryWidget *m_scientific_r;

signals:
    void widgetplus(int row);
    void widgetminus(int row);
    void filledMem();
    void emptyMem();

public slots:
};

#endif // MEMORYPUBLIC_H
