#ifndef SEPARATOR_H
#define SEPARATOR_H

#include <QWidget>

class Separator : public QWidget
{
    Q_OBJECT
    
public:
    Separator(QWidget *parent=0);
    ~Separator();

protected:
    void paintEvent(QPaintEvent *);
};	

#endif
