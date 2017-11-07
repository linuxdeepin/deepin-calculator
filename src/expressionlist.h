#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include "listview.h"

class ExpressionList : public ListView
{
    Q_OBJECT

public:
    ExpressionList(QWidget *parent=0);
    ~ExpressionList();

    void addNewRow();
    void enterNumberEvent(const QString &num);
    void enterPointEvent();
    void enterSymbolEvent(const QString &str);
    void enterBracketsEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    void copyResultToClipboard();

private:
    bool isLeftBracket;
    bool isContinue;
    bool isAllClear;

    QString getResult();
    QChar getLastChar();
    bool lastCharIsNumber();
    bool lastCharIsSymbol();
    bool lastCharIsPoint();
    bool lastCharIsLeftBracket();
    bool lastCharIsRightBracket();
    bool isEnding();
};

#endif

