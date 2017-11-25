#ifndef EXPRESSIONLIST_H
#define EXPRESSIONLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include "listview.h"
#include "inputedit.h"
#include "core/evaluator.h"

typedef Quantity::Format Format;

class ExpressionList : public QWidget
{
    Q_OBJECT

public:
    ExpressionList(QWidget *parent=0);
    ~ExpressionList();

    void setContinue(const bool &mark);
    QString getInputEditText() const;

public slots:
    void enterNumberEvent(const QString &num, bool isKeyPress = false);
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
    void inputKeyPressEvent(QKeyEvent *);

private:
    Evaluator *m_eval;
    bool m_isContinue;
    bool m_isAllClear;
    QVBoxLayout *m_layout;
    ListView *m_listView;
    InputEdit *m_inputEdit;
    void autoZoomFontSize();
    QString formatExp(const QString &exp);
    QChar getLastChar(const QString &str);
    QChar getPosLastChar();
    bool cursorPosAtEnd();
};

#endif

