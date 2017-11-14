#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include "textbutton.h"

class BackButton : public TextButton
{
    Q_OBJECT

public:
    BackButton(QWidget *parent = 0);
    ~BackButton();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    void init();
};

#endif
