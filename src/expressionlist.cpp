#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include "dthememanager.h"
#include "expressionlist.h"
#include "utils.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>

DWIDGET_USE_NAMESPACE

ExpressionList::ExpressionList(QWidget *parent) : QWidget(parent)
{
    eval = Evaluator::instance();
    layout = new QVBoxLayout(this);
    listView = new ListView;
    inputEdit = new InputEdit;

    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(listView);
    layout->addWidget(inputEdit);

    inputEdit->setTextMargins(10, 0, 10, 8);
    inputEdit->setFixedHeight(55);
    inputEdit->setAlignment(Qt::AlignRight);

    defaultFontSize = 25;
    minFontSize = 10;
    fontSize = defaultFontSize;
    isContinue = true;
    isAllClear = false;

    setFixedHeight(160);
    initFontSize();

    connect(inputEdit, &InputEdit::textChanged, this, &ExpressionList::inputEditChanged);
    connect(inputEdit, &InputEdit::inputKeyPressEvent, this, &ExpressionList::inputKeyPressEvent);
}

ExpressionList::~ExpressionList()
{
}

void ExpressionList::setContinue(const bool &mark)
{
    isContinue = mark;
}

QString ExpressionList::getInputEditText() const
{
    return inputEdit->text();
}

void ExpressionList::enterNumberEvent(const QString &num, bool isKeyPress)
{
    if (!isContinue) {
        inputEdit->setText("");
        isContinue = true;
    }

    if (!isKeyPress) {
        inputEdit->insert(num);
    }

    isAllClear = false;

    emit clearStateChanged(false);
}

void ExpressionList::enterPointEvent()
{
    inputEdit->insert(".");
}

void ExpressionList::enterSymbolEvent(const QString &str)
{
    inputEdit->insert(str);
    isContinue = true;
}

void ExpressionList::enterBracketsEvent()
{
    const int currentPos = inputEdit->cursorPosition();
    inputEdit->insert("()");
    inputEdit->setCursorPosition(currentPos + 1);

    isAllClear = false;
    isContinue = true;
}

void ExpressionList::enterBackspaceEvent()
{
    inputEdit->backspace();

    isContinue = true;
    isAllClear = false;
}

void ExpressionList::enterClearEvent()
{
    if (isAllClear) {
        listView->clearItems();
        isAllClear = false;

        emit clearStateChanged(false);
    } else {
        inputEdit->setText("");
        isAllClear = true;

        initFontSize();
        emit clearStateChanged(true);
    }
}

void ExpressionList::enterEqualEvent()
{
    QString str = eval->autoFix(formatExp(inputEdit->text()));
    if (getLastChar(str) == '%') {
        str = str.left(str.count() - 1);
        str.append(" percent ");
    }

    eval->setExpression(str);
    auto quantity = eval->evalNoAssign();

    if (eval->error().isEmpty()) {
        if (quantity.isNan() && eval->isUserFunctionAssign()) {

        } else {
            const QString result = DMath::format(eval->evalUpdateAns(), Quantity::Format::Fixed());
            const double resultNum = result.toDouble();
            if (result == inputEdit->text()) {
                return;
            }
            listView->addItem(inputEdit->text() + " ＝ " + QString::number(resultNum));
            inputEdit->setText(result);
            isContinue = false;
        }
    } else {
        inputEdit->setStyleSheet("QLineEdit { color: #FB6A6A }");
        QTimer::singleShot(200, this, [=] { inputEdit->setStyleSheet("QLineEdit { color: 000000; }"); });
    }
}

void ExpressionList::copyResultToClipboard()
{
    const auto str = eval->autoFix(formatExp(inputEdit->text()));
    eval->setExpression(str);

    if (!eval->error().isEmpty()) {
        QApplication::clipboard()->setText(inputEdit->text());
    } else {
        const QString result = DMath::format(eval->evalUpdateAns(), Quantity::Format::Fixed());
        QApplication::clipboard()->setText(result);
    }
}

int ExpressionList::getItemsCount()
{
    return listView->getItemsCount();
}

void ExpressionList::inputEditChanged(const QString &text)
{
    // using setText() will move the cursor pos to end.
    const int currentPos = inputEdit->cursorPosition();

    // replace expression string.
    inputEdit->setText(QString(text).replace("+", "＋").replace("-", "－")
                                    .replace(QRegExp("[x|X|*]"), "×").replace("/", "÷")
                                    .replace("（", "(").replace("）", ")")
                                    .replace("。", ".").replace("——", "－")
                                    .replace(" ", ""));
    inputEdit->setCursorPosition(currentPos);

    // make font size of inputEdit fit text content.
    // QFontMetrics fm = inputEdit->fontMetrics();
    // int w = fm.boundingRect(inputEdit->text()).width();

    // if (w >= inputEdit->width() - 30) {
    //     fontSize -= 2;
    //     QFont font;
    //     font.setPointSize(qMax(fontSize, minFontSize));
    //     inputEdit->setFont(font);
    // }

    // when text is empty, the clear button status will changed.
    // if (text.isEmpty()) {
    //     isAllClear = true;
    //     initFontSize();
    //     emit clearStateChanged(true);
    // } else {
    //     isAllClear = false;
    //     emit clearStateChanged(false);
    // }

    isAllClear = false;
    isContinue = true;

    emit clearStateChanged(false);
}

void ExpressionList::initFontSize()
{
    fontSize = defaultFontSize;
    QFont font;
    font.setPointSize(fontSize);
    inputEdit->setFont(font);
}

QString ExpressionList::formatExp(const QString &exp)
{
    return QString(exp).replace("＋", "+").replace("－", "-").replace("×", "*").replace("÷", "/")
                       .replace("%+", " percent + ").replace("%-", " percent - ").replace("%*", " percent * ")
                       .replace("%/", " percent / ");
}

QChar ExpressionList::getLastChar(const QString &str)
{
    QString exp = formatExp(str);
    QString::const_iterator laster = exp.end();
    laster--;

    return *laster;
}
