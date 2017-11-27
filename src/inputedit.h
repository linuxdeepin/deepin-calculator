#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <QLineEdit>

class InputEdit : public QLineEdit
{
    Q_OBJECT

public:
    InputEdit(QLineEdit *parent = nullptr);
    ~InputEdit();

signals:
    void inputKeyPressEvent(QKeyEvent *e);

protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
};

#endif
