#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "listview.h"

class DisplayArea : public ListView
{
    Q_OBJECT

public:
    DisplayArea(QWidget *parent=0);
    ~DisplayArea();

    void addNewRow();
    void enterNumberEvent(const QString &num);
    void enterPointEvent();
    void enterSymbolEvent(const QString &str);
    void enterBracketsEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    QString getResult();
    void copyResultToClipboard();

private:
    bool isLeftBracket;
    bool isContinue;
    bool isAllClear;

    QChar getLastChar();
    bool lastCharIsSymbol();
    bool lastCharIsPoint();
    bool isEnding();
};

#endif

