#ifndef EXPRESSIONLIST_H
#define EXPRESSIONLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include "listview.h"
#include "inputedit.h"

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

private slots:
    void inputEditChanged(const QString &text);

signals:
    void clearStateChanged(bool isAllClear);

private:
    int defaultFontSize;
    int minFontSize;
    int fontSize;
    bool isLeftBracket;
    bool isContinue;
    bool isAllClear;
    QVBoxLayout *layout;
    ListView *listView;
    InputEdit *inputEdit;
    void initFontSize();
    QString formatExp(const QString &exp);
    QChar getLastChar();
    bool lastCharIsNumber();
    bool lastCharIsSymbol();
    bool lastCharIsPoint();
    bool lastCharIsLeftBracket();
    bool lastCharIsRightBracket();
};

#endif

