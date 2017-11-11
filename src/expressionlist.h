#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QWidget>
#include <QLineEdit>
#include "listview.h"

class ExpressionList : public QWidget
{
    Q_OBJECT

public:
    ExpressionList(QWidget *parent=0);
    ~ExpressionList();

    void enterNumberEvent(const QString &num);
    void enterPointEvent();
    void enterSymbolEvent(const QString &str);
    void enterBracketsEvent();
    void enterBackspaceEvent();
    void enterClearEvent();
    void enterEqualEvent();
    void copyResultToClipboard();
    int getItemsCount();

signals:
    void clearStateChanged(bool isAllClear);

private:
    bool isLeftBracket;
    bool isContinue;
    bool isAllClear;

    ListView *listView;
    QLineEdit *inputEdit;
    QString getResult();
    QString formatExp(const QString &exp);
    QChar getLastChar();
    bool lastCharIsNumber();
    bool lastCharIsSymbol();
    bool lastCharIsPoint();
    bool lastCharIsLeftBracket();
    bool lastCharIsRightBracket();
};

#endif

