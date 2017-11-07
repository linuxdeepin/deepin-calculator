#include <DApplication>
#include <DWidgetUtil>
#include <DHiDPIHelper>
#include "mainwindow.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
//    qputenv("QT_SCALE_FACTOR", "2");
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    if (app.setSingleInstance("deepin-calculator")) {
        app.loadTranslator();
        app.setOrganizationName("deepin");
        app.setApplicationVersion("1.0");
        app.setProductIcon(DHiDPIHelper::loadNxPixmap(":/images/icon.svg"));
        app.setProductName(DApplication::translate("Main", "Deepin Calculator"));
        app.setApplicationDescription(DApplication::translate("Main", "Deepin calculator is an easy to use calculator for ordinary user."));
        app.setTheme("light");

        MainWindow w;
        w.show();

        Dtk::Widget::moveToCenter(&w);
        qApp->setStyleSheet(Utils::getQssContent(":/qss/style.qss"));

        return app.exec();
    }

    return 0;
}
