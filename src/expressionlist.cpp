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

    isContinue = true;
    isAllClear = false;

    setFixedHeight(160);
    autoZoomFontSize();

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
        autoZoomFontSize();
        QTimer::singleShot(200, this, [=] { inputEdit->setStyleSheet("QLineEdit { color: 000000; }");});
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
    autoZoomFontSize();

    isAllClear = false;
    isContinue = true;

    emit clearStateChanged(false);
}

void ExpressionList::autoZoomFontSize()
{
    QFont font;
    for (int i = 27; i > 9; --i) {
        font.setPointSize(i);
        const QFontMetrics fm(font);
        const int fontWidth = fm.width(inputEdit->text());
        const int editWidth = inputEdit->width() - 40;

        if (fontWidth < editWidth) {
            inputEdit->setFont(font);
            break;
        }
    }
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
