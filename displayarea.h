#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "listview.h"
#include "listitem.h"

class DisplayArea : public ListView
{
    Q_OBJECT

public:
    DisplayArea(QWidget *parent=0);
    ~DisplayArea();

    void addNextLine(const QString &str);

private:
    QList<ListItem *> listItems;
};	

#endif
