#include "proexpressionbar.h"

#include <QVBoxLayout>

const int LIST_HEIGHT = 35; //输入栏上方表达式的高度
const int INPUTEDIT_HEIGHT = 55;

ProExpressionBar::ProExpressionBar(QWidget *parent)
    : DWidget(parent)
{
    m_listView = new SimpleListView;
    m_listDelegate = new SimpleListDelegate(0, this);
    m_listModel = new SimpleListModel(0, this);
    m_listView->setModel(m_listModel);
    m_listView->setItemDelegate(m_listDelegate);
    m_inputEdit = new InputEdit;
    m_listView->setFixedHeight(LIST_HEIGHT);
    m_inputEdit->setFixedHeight(INPUTEDIT_HEIGHT);
    m_inputEdit->setAlignment(Qt::AlignRight);
    m_inputEdit->setTextMargins(10, 0, 10, 6);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_listView);
    layout->addWidget(m_inputEdit);
    layout->setMargin(0);
    layout->setSpacing(0);
}

ProExpressionBar::~ProExpressionBar()
{

}

void ProExpressionBar::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void ProExpressionBar::enterEqualEvent()
{

}

void ProExpressionBar::initTheme(int type)
{
    m_listDelegate->setThemeType(type);
    m_inputEdit->themetypechanged(type);
}

void ProExpressionBar::revisionResults(const QModelIndex &index)
{
    QString text = index.data(SimpleListModel::ExpressionRole).toString();
    QStringList historic = text.split(QString("＝"), QString::SkipEmptyParts);
    if (historic.size() != 2)
        return;
    QString expression = historic.at(0);
//    m_hisRevision = index.row();
    m_inputEdit->setText(expression);
//    m_Selected = m_hisRevision;
    m_isResult = false;
    // fix addundo for history revision
    m_isUndo = false;
    addUndo();

    emit clearStateChanged(false);
}

void ProExpressionBar::addUndo()
{
    m_undo.append(m_inputEdit->text());
    m_redo.clear();
    m_inputEdit->setUndoAction(true);
    SSelection selection;
    m_inputEdit->setSelection(selection);
}

void ProExpressionBar::copyResultToClipboard()
{
    if (m_inputEdit->text().isEmpty())
        return;

    SSelection selection = m_inputEdit->getSelection();
    QApplication::clipboard()->setText(selection.selected); //将选中项放入粘贴板
}

void ProExpressionBar::copyClipboard2Result()
{
    QString oldText = m_inputEdit->text(); //未粘贴操作的text
    int curpos = m_inputEdit->cursorPosition(); //未粘贴操作的光标位
    replaceSelection(oldText);
    QString exp = m_inputEdit->text();
    QString text = QApplication::clipboard()->text();
    text = text.replace('+', QString::fromUtf8("＋"))
           .replace('-', QString::fromUtf8("－"))
           .replace("_", QString::fromUtf8("－"))
           .replace('*', QString::fromUtf8("×"))
           .replace('x', QString::fromUtf8("×"))
           .replace('X', QString::fromUtf8("×"))
           .replace(QString::fromUtf8("＊"), QString::fromUtf8("×"))
           .replace(QString::fromUtf8("（"), "(")
           .replace(QString::fromUtf8("）"), ")")
           .replace(QString::fromUtf8("。"), ".")
           .replace(QString::fromUtf8("——"), QString::fromUtf8("－"))
           .replace(QString::fromUtf8("％"), "%")
           .replace('/', QString::fromUtf8("÷")); //对粘贴板中的内容进行英替中

    m_inputEdit->insert(text);

    QString faulttolerance = pointFaultTolerance(m_inputEdit->text());
    faulttolerance = m_inputEdit->symbolFaultTolerance(faulttolerance);
    if (faulttolerance != m_inputEdit->text())
        m_inputEdit->setText(faulttolerance); //如果经过容错处理的表达式有改变，重新设置表达式，不设置光标
    if (m_inputEdit->text() == exp) {
        m_inputEdit->setText(oldText);
        m_inputEdit->setCursorPosition(curpos);
    }
    if (!m_inputEdit->text().isEmpty())
        emit clearStateChanged(false);
    m_isResult = false;
    m_isUndo = false;
    addUndo();
}

void ProExpressionBar::allElection()
{

}

void ProExpressionBar::shear()
{
    QString text = m_inputEdit->text();
    int selcurPos = m_inputEdit->cursorPosition();
    QString selectText = m_inputEdit->selectedText();
    selectText = selectText.replace(",", "");
    QApplication::clipboard()->setText(selectText);
    int start = m_inputEdit->selectionStart();
    int length = m_inputEdit->selectionLength();
    text.remove(start, length);
    m_inputEdit->setText(text);
    addUndo();
    m_isUndo = false;
    //设置剪切后光标位置
    if (text.mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()E]")).length() ==
            m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()E]")).length())
        m_inputEdit->setCursorPosition(selcurPos);
    else if (text.mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()E]")).length() >
             m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[＋－×÷,.%()E]")).length())
        m_inputEdit->setCursorPosition(selcurPos + 1);
    else
        m_inputEdit->setCursorPosition(selcurPos - 1);

    //发送C/AC切换信号
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }
}

void ProExpressionBar::deleteText()
{

}

void ProExpressionBar::Undo()
{
    if (m_undo.isEmpty())
        return;
    //    clearLinkageCache();
    m_redo.append(m_undo.last());
    m_inputEdit->setRedoAction(true);
    m_undo.removeLast();
    m_isUndo = true;
    //20200619 从结果撤销到上一次表达式按下数字键不清空输入框
    if (m_isResult)
        m_isResult = false;
    // 20200319修复选中某一数字按下相同数字无法清除选中内容的问题
    if (!m_undo.isEmpty()) {
        if (m_undo.size() > 1) {
            for (int i = m_undo.size() - 1; i > 0; i--) {
                if (m_undo.at(i) == m_inputEdit->text() && m_undo.at(i - 1) == m_inputEdit->text())
                    m_undo.pop_back();
            }
        }
        m_inputEdit->setText(m_undo.last());
    } else {
        m_inputEdit->clear();
        m_inputEdit->setUndoAction(false);
    }
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }
}

void ProExpressionBar::Redo()
{
    if (m_redo.isEmpty())
        return;
    //    clearLinkageCache();
    m_inputEdit->setText(m_redo.last());
    m_undo.append(m_inputEdit->text());
    m_redo.removeLast();
    if (m_redo.isEmpty())
        m_inputEdit->setRedoAction(false);
    if (m_inputEdit->text().isEmpty() && m_listModel->rowCount(QModelIndex()) != 0) {
        emit clearStateChanged(true);
        m_isAllClear = true;
    } else {
        emit clearStateChanged(false);
        m_isAllClear = false;
    }
}

void ProExpressionBar::setResultFalse()
{
    m_isResult = false;
}

void ProExpressionBar::initConnect()
{
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, this,
            &ProExpressionBar::revisionResults);
    connect(m_listDelegate, &SimpleListDelegate::obtainingHistorical, m_inputEdit,
            &InputEdit::hisexpression);
    connect(m_inputEdit, &InputEdit::textChanged, this, &ProExpressionBar::handleTextChanged);
//    connect(m_inputEdit, &InputEdit::keyPress, this, &ProExpressionBar::keyPress);
    connect(m_inputEdit, &InputEdit::equal, this, &ProExpressionBar::enterEqualEvent);
    connect(m_inputEdit, &InputEdit::cut, this, &ProExpressionBar::shear);
    connect(m_inputEdit, &InputEdit::copy, this, &ProExpressionBar::copyResultToClipboard);
    connect(m_inputEdit, &InputEdit::paste, this, &ProExpressionBar::copyClipboard2Result);
    connect(m_inputEdit, &InputEdit::deleteText, this, &ProExpressionBar::deleteText);
    connect(m_inputEdit, &InputEdit::selectAllText, this, &ProExpressionBar::allElection);
    connect(m_inputEdit, &InputEdit::undo, this, &ProExpressionBar::Undo);
    connect(m_inputEdit, &InputEdit::redo, this, &ProExpressionBar::Redo);
    connect(m_inputEdit, &InputEdit::setResult, this, &ProExpressionBar::setResultFalse);
}

void ProExpressionBar::replaceSelection(QString text)
{
    QString seloldtext = text;
    SSelection selection = m_inputEdit->getSelection();
    int selcurPos = m_inputEdit->cursorPosition();
    if (selection.selected != "") {
        text.remove(selection.curpos, selection.selected.size());
        m_inputEdit->setText(text);
        if (selcurPos > selection.curpos &&
                selcurPos <= selection.curpos + selection.selected.size())
            selcurPos = selection.curpos;
        // 20200313选中部分光标置位问题修复
        if (seloldtext.mid(0, selcurPos).remove(QRegExp("[,]")).length() ==
                m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[,]")).length())
            m_inputEdit->setCursorPosition(selcurPos);
        else if (seloldtext.mid(0, selcurPos).remove(QRegExp("[,]")).length() >
                 m_inputEdit->text().mid(0, selcurPos).remove(QRegExp("[,]")).length())
            m_inputEdit->setCursorPosition(selcurPos + 1);
        else
            m_inputEdit->setCursorPosition(selcurPos - 1);
    }
    // reset selection
    selection = SSelection();
    m_inputEdit->setSelection(selection);
}

QString ProExpressionBar::symbolComplement(const QString exp)
{
    QString text = exp;
    int index = text.indexOf("(", 0);
    while (index != -1) {
        if (index >= 1 && text.at(index - 1).isNumber()) {
            text.insert(index, "×");
            ++index;
        }
        ++index;
        index = text.indexOf("(", index);
    }
    index = text.indexOf(")", 0);
    while (index != -1) {
        if (index < text.length() - 1 && text.at(index + 1).isNumber()) {
            text.insert(index + 1, "×");
            ++index;
        }
        ++index;
        index = text.indexOf(")", index);
    }
    //20200811　fix bug-42274 e,π,lastans跟随函数表达式错误问题
    index = text.indexOf("e", 0);
    while (index != -1) {
        text.insert(index, "(");
        text.insert(index + 2, ")");
        index = text.indexOf("e", index + 3);
    }
    index = text.indexOf("pi", 0);
    while (index != -1) {
        text.insert(index, "(");
        text.insert(index + 3, ")");
        index = text.indexOf("pi", index + 4);
    }
    index = text.indexOf("lastans", 0);
    while (index != -1) {
        text.insert(index, "(");
        text.insert(index + 8, ")");
        index = text.indexOf("lastans", index + 9);
    }
    return text;
}

QString ProExpressionBar::pointFaultTolerance(const QString &text)
{
    QString oldText = text;
    // QString reformatStr = Utils::reformatSeparators(QString(text).remove(','));
    QString reformatStr = oldText.replace('+', QString::fromUtf8("＋"))
                          .replace('-', QString::fromUtf8("－"))
                          .replace("_", QString::fromUtf8("－"))
                          .replace('*', QString::fromUtf8("×"))
                          .replace('x', QString::fromUtf8("×"))
                          .replace('X', QString::fromUtf8("×"))
                          .replace(QString::fromUtf8("＊"), QString::fromUtf8("×"))
                          .replace(QString::fromUtf8("（"), "(")
                          .replace(QString::fromUtf8("）"), ")")
                          .replace(QString::fromUtf8("。"), ".")
                          .replace(QString::fromUtf8("——"), QString::fromUtf8("－"))
                          .replace(QString::fromUtf8("％"), "%")
                          /*.replace('/', QString::fromUtf8("÷"))*/; //对内容进行英替中
    QStringList list = reformatStr.split(QRegExp("[＋－×÷/(]")); //20200717去掉),否则下方)小数点容错无法进入
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        int firstPoint = item.indexOf(".");
        if (firstPoint == -1)
            continue;
        if (firstPoint == 0) {
            item.insert(firstPoint, "0"); //小数点在数字前，进行补0;例:.123->0.123;此处未对reformatStr进行操作，导致只有两个.时才会进行补0
            ++firstPoint;
            // oldText.replace(list[i], item);
        } else {
            if (item.at(firstPoint - 1) == ")" || item.at(firstPoint - 1) == "%") {
                item.remove(firstPoint, 1);
                item.insert(firstPoint, "0."); //20200717)及%后小数点补0;与小数点输入处理一致
                reformatStr.replace(list[i], item);
            }
        }
        if (item.count(".") > 1) {
            item.remove(".");
            item.insert(firstPoint, ".");
            reformatStr.replace(list[i], item); //去除多余.
        }
    }
    for (int i = 0; i < reformatStr.size(); ++i) {
        //20200811避免e,π后的小数点补0
        if (reformatStr[i] == "." && (i == 0 || (!reformatStr[i - 1].isNumber() && reformatStr[i - 1] != "e" && reformatStr[i - 1] != "π"))) {
            reformatStr.insert(i, "0"); //补0操作，例:1+.2->1+0.2
            ++i;
        }
    }

    return reformatStr;
}

void ProExpressionBar::handleTextChanged()
{
    m_isAllClear = false;
    m_isContinue = true;
}
