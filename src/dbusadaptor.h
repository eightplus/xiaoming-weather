/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.eightplus.weather.xml -a dbusadaptor -c DbusAdaptor -l MainWindow
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSADAPTOR_H
#define DBUSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.eightplus.weather
 */

#include "mainwindow.h"//added the code manually by lixiang

class DbusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.eightplus.weather")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.eightplus.weather\">\n"
"    <method name=\"showMainWindow\"/>\n"
"  </interface>\n"
        "")
public:
    DbusAdaptor(MainWindow *parent);
    virtual ~DbusAdaptor();

    inline MainWindow *parent() const
    { return static_cast<MainWindow *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void showMainWindow();
Q_SIGNALS: // SIGNALS
};

#endif
