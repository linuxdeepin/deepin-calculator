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
    MemoryWidget *standard_l;
    MemoryWidget *scientific_r;
    QList<Quantity> list;

signals:
    void widgetplus(int row);
    void widgetminus(int row);

public slots:
};

#endif // MEMORYPUBLIC_H
