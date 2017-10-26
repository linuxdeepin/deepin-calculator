#include <DApplication>
#include <DWidgetUtil>
#include "mainwindow.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setTheme("light");

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);
    
    return app.exec();
}

