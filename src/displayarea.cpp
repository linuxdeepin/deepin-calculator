#include <QApplication>
#include <QClipboard>
#include "displayarea.h"
#include "listitem.h"
#include "algorithm.h"
#include <QDebug>

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
    if (isEnding())
        return;

    if (!isContinue || listItems.last()->expression == "0") {
        listItems.last()->expression = nullptr;
        isContinue = true;
    }

    if (lastCharIsRightBracket()) {
        listItems.last()->expression.append("×0");
    }

    isAllClear = false;
    listItems.last()->expression.append(num);
    scrollToBottom();
}

void DisplayArea::enterPointEvent()
{
    if (isEnding())
        return;

    if (lastCharIsNumber()) {
        bool flag = false;
        QString exp = listItems.last()->expression;
        for (int i = 0; i < exp.count(); ++i) {
            if (exp.at(exp.count() - 1 - i) == QString("+") ||
                exp.at(exp.count() - 1 - i) == QString("-") ||
                exp.at(exp.count() - 1 - i) == QString("×") ||
                exp.at(exp.count() - 1 - i) == QString("÷")) {
                flag = true;
                break;
            } else if (exp.at(exp.count() - 1 - i) == QString(".")) {
                flag = false;
                break;
            } else {
                flag = true;
            }
        }

        if (flag) {
            listItems.last()->expression.append(".");
            isContinue = true;
            isAllClear = false;
        }
    } else if (lastCharIsSymbol() || lastCharIsLeftBracket()) {
        listItems.last()->expression.append("0.");
    } else if (lastCharIsRightBracket()) {
        listItems.last()->expression.append("×0.");
    }

    scrollToBottom();
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    if (isEnding())
        return;

    if (lastCharIsSymbol()) {
        enterBackspaceEvent();
    } else if (lastCharIsPoint() || lastCharIsLeftBracket()) {
        listItems.last()->expression.append("0");
    }

    isContinue = true;
    isAllClear = false;
    listItems.last()->expression.append(str);
    scrollToBottom();
}

void DisplayArea::enterBracketsEvent()
{
    if (isEnding())
        return;

    if (!isContinue || listItems.last()->expression == "0") {
        listItems.last()->expression = nullptr;
        isContinue = true;
    }

    if (isLeftBracket) {
        if (lastCharIsNumber()) {
            listItems.last()->expression.append("×");
        } else if (lastCharIsPoint()) {
            listItems.last()->expression.append("0×");
        }

        listItems.last()->expression.append("(");
        isLeftBracket = false;
    } else {
        if (lastCharIsPoint() || lastCharIsSymbol()) {
            listItems.last()->expression.append("0");
        }

        listItems.last()->expression.append(")");
        isLeftBracket = true;
    }

    isAllClear = false;
    scrollToBottom();
}

void DisplayArea::enterBackspaceEvent()
{
    const QString exp = listItems.last()->expression;

    if (exp.length() == 1) {
        if (lastCharIsLeftBracket()) {
            isLeftBracket = true;
        }
    
        listItems.last()->expression = "0";
    }else {
        if (lastCharIsLeftBracket()) {
            isLeftBracket = true;
        } else if (lastCharIsRightBracket()) {
            isLeftBracket = false;
        }

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

bool DisplayArea::lastCharIsNumber()
{
    const QChar lastChar = getLastChar();

    if (lastChar == '0' || lastChar == '1' || lastChar == '2' || lastChar == '3' ||
        lastChar == '4' || lastChar == '5' || lastChar == '6' || lastChar == '7' ||
        lastChar == '8' || lastChar == '9') {
        return true;
    } else {
        return false;
    }
}

bool DisplayArea::lastCharIsSymbol()
{
    const QChar lastChar = getLastChar();

    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/' || lastChar == '%')
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
