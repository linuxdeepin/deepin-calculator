#include <DApplication>
#include <DWidgetUtil>
#include <DHiDPIHelper>
#include "mainwindow.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationVersion("1.0");
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-calculator");
    app.setProductIcon(QIcon(":/images/deepin-calculator.svg"));
    app.setProductName(DApplication::translate("Main", "Deepin Calculator"));
    app.setApplicationDescription(DApplication::translate("Main", "Deepin calculator is an easy to use calculator for ordinary user."));

    MainWindow w;
    w.show();

    if (app.setSingleInstance("deepin-calculator")) {
        Dtk::Widget::moveToCenter(&w);
    }

    return app.exec();
}
