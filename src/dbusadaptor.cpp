/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.eightplus.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "dbusadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DbusAdaptor
 */

DbusAdaptor::DbusAdaptor(MainWindow *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DbusAdaptor::~DbusAdaptor()
{
    // destructor
}

void DbusAdaptor::showMainWindow()
{
    // handle method call com.eightplus.weather.showMainWindow
    //edited the code manually by lixiang
//    parent()->movePosition();
}

