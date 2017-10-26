#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSvgWidget>

class TitleBar : public QWidget
{
    Q_OBJECT
    
public:
    TitleBar(QWidget *parent=0);
	~TitleBar();

private:
    QHBoxLayout *layout;
    QSvgWidget *iconWidget;
};	

#endif
