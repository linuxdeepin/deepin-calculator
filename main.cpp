/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <DApplication>
#include <DWidgetUtil>
#include "mainwindow.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-calculator");
    app.setApplicationVersion(DApplication::buildVersion("1.0.1"));
    app.setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/deepin-calculator");
    app.setProductIcon(QIcon(":/images/deepin-calculator.svg"));
    app.setProductName(DApplication::translate("MainWindow", "Deepin Calculator"));
    app.setApplicationDescription(DApplication::translate("MainWindow", "Deepin Calculator is a simple and easy to use desktop calculator. It supports addition, subtraction, multiplication and division."));

    MainWindow w;
    w.show();

    if (app.setSingleInstance("deepin-calculator")) {
        Dtk::Widget::moveToCenter(&w);
    }

    return app.exec();
}
