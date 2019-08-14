/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
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

#include <signal.h>

int main(int argc, char *argv[])
{
    signal(SIGINT, [](int) { QApplication::quit(); });// 设置退出信号

    QApplication a(argc, argv);

    a.setOrganizationName("eightplus");
    a.setApplicationName("xiaoming-weather");
    a.setApplicationVersion("0.0.1");
    //a.setQuitOnLastWindowClosed(false);//Avoid that after hiding mainwindow, close the sub window would cause the program exit

    QTranslator app_trans;
    QTranslator qt_trans;
    QString locale = QLocale::system().name();

    QString trans_path;
    if (QDir("/usr/share/xiaoming-weather/translations").exists()) {
        trans_path = "/usr/share/xiaoming-weather/translations";
    }
    else {
        trans_path = qApp->applicationDirPath() + "/translations";
    }

    //QString qt_trans_path;
    //qt_trans_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);// /usr/share/qt5/translations

    if (locale == "zh_CN") {
        if(!app_trans.load("xiaoming-weather_" + locale + ".qm", trans_path))
            qDebug() << "Load translation file："<< "xiaoming-weather_" + locale + ".qm from" << trans_path << "failed!";
        else
            a.installTranslator(&app_trans);

        /*if(!qt_trans.load("qt_" + locale + ".qm", qt_trans_path))
            qDebug() << "Load translation file："<< "qt_" + locale + ".qm from" << qt_trans_path << "failed!";
        else
            a.installTranslator(&qt_trans);*/
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
