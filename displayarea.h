#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "listview.h"

class DisplayArea : public ListView
{
    Q_OBJECT

public:
    DisplayArea(QWidget *parent=0);
    ~DisplayArea(); 
};	

#endif
