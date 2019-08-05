/*
 * Copyright (C) 2018 ~ 2019 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "dbusadaptor.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("eightplus");
    a.setApplicationName("xiaoming-weather");
    a.setApplicationVersion("0.0.1");
    a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit

    QString locale = QLocale::system().name();
    QTranslator translator;
    if(locale == "zh_CN") {
        if(!translator.load("xiaoming-weather_" + locale + ".qm", ":/qm/translation/"))
            qDebug() << "Load translation file："<< "xiaoming-weather_" + locale + ".qm" << " failed!";
        else
            a.installTranslator(&translator);
    }

    QFile qss(":/qss/res/weather.qss");
    if (!qss.open(QFile::ReadOnly)) {
        qWarning("Can't open the style sheet file: :/qss/res/weather.qss.");
    }
    else {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }

    MainWindow w;
    DbusAdaptor adaptor(&w);
    Q_UNUSED(adaptor);
    auto connection = QDBusConnection::sessionBus();
    if (!connection.registerService("com.eightplus.weather") || !connection.registerObject("/com/eightplus/weather", &w/*, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals*/)) {
        qCritical() << "QDbus register service failed reason:" << connection.lastError();
        QDBusInterface iface("com.eightplus.weather",
                                       "/com/eightplus/weather",
                                       "com.eightplus.weather",
                                       connection);
        iface.call("showMainWindow");

        return 0;
    }//QDBusConnection::sessionBus().unregisterService("com.eightplus.weather");

    return a.exec();
}
