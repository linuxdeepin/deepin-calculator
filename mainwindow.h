#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <QGridLayout>
#include "titlebar.h"
#include "displayarea.h"
#include "textbutton.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(DMainWindow *parent=0);
    ~MainWindow();

private:
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    TitleBar *titleBar;
    DisplayArea *displayArea;
};	

#endif
