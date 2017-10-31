#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "listview.h"

class DisplayArea : public ListView
{
    Q_OBJECT

public:
    DisplayArea(QWidget *parent=0);
    ~DisplayArea();

    void enterNumberEvent(const QString &num);
    void enterPointEvent();
    void enterSymbolEvent(const QString &str);
    void enterBracketsEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();

private:
    bool isLeftBracket;
};

#endif
