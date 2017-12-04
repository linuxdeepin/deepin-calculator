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

private:
    Quantity m_ans; /* Previously calculated answer */
    int m_ans_start; /* Index of ans in expression */
    int m_ans_length;
    bool m_ans_vaild;
    bool m_cur_in_ans;
    bool m_cur_on_ans_left;
    QString m_old_text;
    bool isSymbolCategoryChanged(int pos1, int pos2);
    int findWordBeginPosition(int pos);
    int findWordEndPosition(int pos);
    void heilightAns(int start, int length);
};

#endif
