#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QWidget>

class DisplayArea : public QWidget
{
    Q_OBJECT

public:
    DisplayArea(QWidget *parent=0);
	~DisplayArea(); 

protected:
    void paintEvent(QPaintEvent *);
};	

#endif
