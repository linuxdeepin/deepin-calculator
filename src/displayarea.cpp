#include <QApplication>
#include <QClipboard>
#include "displayarea.h"
#include "listitem.h"
#include "algorithm.h"

DisplayArea::DisplayArea(QWidget *parent) : ListView(parent)
{
    isLeftBracket = true;
    isContinue = true;
    isAllClear = false;

    setFixedHeight(175);
    addNewRow();
}

DisplayArea::~DisplayArea()
{
}

void DisplayArea::addNewRow()
{
    ListItem *item = new ListItem;
    addItem(item);

    scrollToBottom();
}

void DisplayArea::enterNumberEvent(const QString &num)
{
    if (!isEnding()) {
        if (listItems.last()->expression == "0")
            listItems.last()->expression = nullptr;
        if (!isContinue) {
            listItems.last()->expression = nullptr;
            isContinue = true;
        }

        isAllClear = false;
        listItems.last()->expression.append(num);
        scrollToBottom();
    }
}

void DisplayArea::enterPointEvent()
{
    if (!isEnding()) {
        if (lastCharIsSymbol()) {
            listItems.last()->expression.append("0");
        }else if (lastCharIsLeftBracket()) {
            listItems.last()->expression.append("0.");
        }
        if (!lastCharIsPoint()) {
            listItems.last()->expression.append(".");
            isContinue = true;
        }

        scrollToBottom();
    }
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    if (!isEnding()) {
        if (lastCharIsSymbol()) {
            enterBackspaceEvent();
        } else if (lastCharIsPoint()) {
            listItems.last()->expression.append("0");
        }
        
        isContinue = true;
        isAllClear = false;
        listItems.last()->expression.append(str);
        scrollToBottom();
    }
}

void DisplayArea::enterBracketsEvent()
{   
    if (!isEnding()) {
        if (!isContinue) {
            listItems.last()->expression = nullptr;
            isContinue = true;
        }
        
        if (listItems.last()->expression == "0") {
            listItems.last()->expression = nullptr;
        }

        if (isLeftBracket) {
            listItems.last()->expression.append("(");
            isLeftBracket = false;
        }else {
            listItems.last()->expression.append(")");
            isLeftBracket = true;
        }

        isAllClear = false;
        scrollToBottom();
    }
}

void DisplayArea::enterBackspaceEvent()
{
    const QString exp = listItems.last()->expression;
    const QChar lastChar = getLastChar();

    if (exp.length() == 1) {
        listItems.last()->expression = "0";
    }else {
        if (lastChar == '(')
            isLeftBracket = true;

        listItems.last()->expression = exp.left(exp.length() - 1);
    }

    isContinue = true;
    scrollToBottom();
}

void DisplayArea::enterClearEvent()
{   
    if (isAllClear) {
        clearAllItems();
        addNewRow();

        isAllClear = false;
    } else {
        listItems.last()->expression = "0";

        isAllClear = true;
    }

    isLeftBracket = true;
    scrollToBottom();
}

void DisplayArea::enterEqualEvent()
{
    if (listItems.last()->expression != "0") {
        const QString result = getResult();
        if (result == "inf" || result == "-inf")
            return;

        listItems.last()->expression.append(" = " + result);
        addNewRow();
        listItems.last()->expression = result;

        isContinue = false;
        isLeftBracket = true;
    }
}

QString DisplayArea::getResult()
{
    QString exp = listItems.last()->expression;
    const double result = Algorithm::getResult(exp.replace("×", "*").replace("÷", "/").toStdString());

    return QString::number(result);
}

void DisplayArea::copyResultToClipboard()
{
    QApplication::clipboard()->setText(getResult());
}

QChar DisplayArea::getLastChar()
{
    QString exp = listItems.last()->expression;
    QString::const_iterator laster = exp.replace("×", "*").replace("÷", "/").end();
    laster--;

    return *laster;
}

bool DisplayArea::lastCharIsSymbol()
{
    const QChar lastChar = getLastChar();

    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/')
        return true;
    else
        return false;
}

bool DisplayArea::lastCharIsPoint()
{
    return getLastChar() == '.' ? true : false;
}

bool DisplayArea::lastCharIsLeftBracket()
{
    return getLastChar() == '(' ? true : false;
}

bool DisplayArea::lastCharIsRightBracket()
{
    return getLastChar() == ')' ? true : false;
}

bool DisplayArea::isEnding()
{
    return listItems.last()->expression.count() > 35 ? true : false;
}
