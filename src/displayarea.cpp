#include <QApplication>
#include <QClipboard>
#include "displayarea.h"
#include "listitem.h"
#include "utils.h"

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
    if (isEnding()) {
        return;
    }

    if (!isContinue || lastItem()->expression == "0") {
        lastItem()->expression = nullptr;
        isContinue = true;
    }

    if (lastCharIsRightBracket()) {
        lastItem()->expression.append("×0");
    }

    isAllClear = false;
    lastItem()->expression.append(num);
    scrollToBottom();
}

void DisplayArea::enterPointEvent()
{
    if (isEnding()) {
        return;
    }

    if (lastCharIsNumber()) {
        bool flag = false;
        QString exp = lastItem()->expression;
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
            lastItem()->expression.append(".");
            isContinue = true;
            isAllClear = false;
        }
    } else if (lastCharIsSymbol() || lastCharIsLeftBracket()) {
        lastItem()->expression.append("0.");
    } else if (lastCharIsRightBracket()) {
        lastItem()->expression.append("×0.");
    }

    scrollToBottom();
}

void DisplayArea::enterSymbolEvent(const QString &str)
{
    if (isEnding()) {
        return;
    }

    if (lastCharIsSymbol()) {
        enterBackspaceEvent();
    } else if (lastCharIsPoint() || lastCharIsLeftBracket()) {
        lastItem()->expression.append("0");
    }

    isContinue = true;
    isAllClear = false;
    lastItem()->expression.append(str);
    scrollToBottom();
}

void DisplayArea::enterBracketsEvent()
{
    if (isEnding()) {
        return;
    }

    if (!isContinue || lastItem()->expression == "0") {
        lastItem()->expression = nullptr;
        isContinue = true;
    }

    if (isLeftBracket) {
        if (lastCharIsNumber()) {
            lastItem()->expression.append("×");
        } else if (lastCharIsPoint()) {
            lastItem()->expression.append("0×");
        }

        lastItem()->expression.append("(");
        isLeftBracket = false;
    } else {
        if (lastCharIsPoint() || lastCharIsSymbol()) {
            lastItem()->expression.append("0");
        }

        lastItem()->expression.append(")");
        isLeftBracket = true;
    }

    isAllClear = false;
    scrollToBottom();
}

void DisplayArea::enterBackspaceEvent()
{
    const QString exp = lastItem()->expression;

    if (exp.length() == 1) {
        if (lastCharIsLeftBracket()) {
            isLeftBracket = true;
        }
        lastItem()->expression = "0";
    }else {
        if (lastCharIsLeftBracket()) {
            isLeftBracket = true;
        } else if (lastCharIsRightBracket()) {
            isLeftBracket = false;
        }
        lastItem()->expression = exp.left(exp.length() - 1);
    }

    isContinue = true;
    scrollToBottom();
}

void DisplayArea::enterClearEvent()
{   
    if (isAllClear) {
        isAllClear = false;

        clearAllItems();
        addNewRow();
    } else {
        isAllClear = true;

        lastItem()->initFontSize();
        lastItem()->expression = "0";
    }

    isLeftBracket = true;
    scrollToBottom();
}

void DisplayArea::enterEqualEvent()
{
    if (lastItem()->expression != "0") {
        if (!isContinue || lastCharIsSymbol() || lastCharIsLeftBracket() || lastCharIsPoint()) {
            return;
        }

        const QString result = getResult();

        if (result == "inf" || result == "-inf" || lastItem()->expression == result) {
            return;
        }

        lastItem()->expression.append(" = " + result);
        addNewRow();
        lastItem()->expression = result;

        isContinue = false;
        isLeftBracket = true;
    }
}

void DisplayArea::copyResultToClipboard()
{
    if (lastCharIsSymbol() || lastCharIsLeftBracket() || lastCharIsPoint()) {
        return;
    }

    QApplication::clipboard()->setText(getResult());
}

QString DisplayArea::getResult()
{
    QString exp = lastItem()->expression;
    const double result = Utils::getResult(exp.replace("×", "*").replace("÷", "/").toStdString());

    return QString::number(result);
}

QChar DisplayArea::getLastChar()
{
    QString exp = lastItem()->expression;
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

    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/' || lastChar == '%') {
        return true;
    } else {
        return false;
    }
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
    return lastItem()->expression.count() > 35 ? true : false;
}
