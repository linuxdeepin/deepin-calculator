#include <DApplication>
#include <DWidgetUtil>
#include <DHiDPIHelper>
#include "mainwindow.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);

    if (app.setSingleInstance("deepin-calculator")) {
        app.setAttribute(Qt::AA_UseHighDpiPixmaps);
        app.setOrganizationName("deepin");
        app.setApplicationDisplayName(QObject::tr("Deepin Calculator"));
        app.setApplicationVersion("1.0");
        app.setProductIcon(DHiDPIHelper::loadNxPixmap(":/images/icon.svg"));
        app.setProductName(QObject::tr("Deepin Calculator"));
        app.setApplicationDescription(QObject::tr("Deepin calculator is an easy to use calculator for ordinary users."));
        app.setTheme("light");
        app.loadTranslator();

        MainWindow w;
        w.show();

        Dtk::Widget::moveToCenter(&w);
        qApp->setStyleSheet(Utils::getQssContent(":/qss/style.qss"));

        return app.exec();    
    }

    return 0;
}

