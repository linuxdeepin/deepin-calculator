#ifndef BACKBUTTON_H
#define BACKBUTTON_H

#include "textbutton.h"
#include <QSvgWidget>

class BackButton : public TextButton
{
    Q_OBJECT

public:
    BackButton(QWidget *parent = nullptr);
    ~BackButton();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

private:
    void init();

private:
    QSvgWidget *m_iconWidget;
};

#endif
