#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include "math/quantity.h"
#include <QLineEdit>

class InputEdit : public QLineEdit
{
    Q_OBJECT

public:
    InputEdit(QLineEdit *parent = nullptr);
    ~InputEdit();
    void setAnswer(QString & str, const Quantity & ans);
    void clear();
    QString expressionText();

signals:
    void inputKeyPressEvent(QKeyEvent *e);

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void handleTextChanged(const QString &text);
    void handleCursorPositionChanged(int old_pos, int new_pos);

private:
    Quantity m_ans;  //Previously calculated answer
    int m_ansStart; //Index of ans in expression
    int m_ansLength;
    bool m_ansVaild;
    bool m_curInAns;
    bool m_curOnAnsLeft;
    QString m_oldText;
    bool isSymbolCategoryChanged(int pos1, int pos2);
    int findWordBeginPosition(int pos);
    int findWordEndPosition(int pos);
    void heilightAns(int start, int length);
};

#endif
