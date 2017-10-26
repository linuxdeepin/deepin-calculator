#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <QVBoxLayout>
#include "titlebar.h"
#include "displayarea.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(DMainWindow *parent=0);
	~MainWindow();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    TitleBar *titleBar;
    DisplayArea *displayArea;
};	

#endif
