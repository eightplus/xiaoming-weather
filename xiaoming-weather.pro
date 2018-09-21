#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T09:33:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiaoming-weather
TEMPLATE = app

inst1.files += res/xiaoming-weather.png
inst1.path = /usr/share/pixmaps
inst2.files += xiaoming-weather.desktop
inst2.path = /usr/share/applications
target.source += $$TARGET
target.path = /usr/bin
INSTALLS += inst1 \
    inst2 \
    target

CONFIG += c++14
CONFIG += qt warn_on
CONFIG += release

QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/titlebar.cpp \
    src/toolbutton.cpp \
    src/weatherwidget.cpp \
    src/backgroundwidget.cpp \
    src/parseinterface.cpp \
    src/weathernowwidget.cpp \
    src/weatherforecastwidget.cpp \
    src/weathernowcategorywidget.cpp \
    src/navigationwidget.cpp \
    src/citynavigation.cpp \
    src/imagebutton.cpp \
    src/locationwidget.cpp \
    src/forecastitemwidget.cpp \
    src/translucentlabel.cpp \
    src/aqitooltip.cpp \
    src/categorybutton.cpp \
    src/indexitemwidget.cpp \
    src/texttip.cpp \
    src/tipmodule.cpp \
    src/windtooltip.cpp \
    src/cityview.cpp \
    src/citymodel.cpp \
    src/citydelegate.cpp \
    src/settingdialog.cpp \
    src/settingtitlebar.cpp \
    src/citywidget.cpp \
    src/activebutton.cpp \
    src/citybtngroups.cpp

HEADERS  += src/mainwindow.h \
    src/titlebar.h \
    src/toolbutton.h \
    src/weatherwidget.h \
    src/backgroundwidget.h \
    src/parseinterface.h \
    src/weathernowwidget.h \
    src/weatherforecastwidget.h \
    src/weathernowcategorywidget.h \
    src/navigationwidget.h \
    src/citynavigation.h \
    src/imagebutton.h \
    src/locationwidget.h \
    src/forecastitemwidget.h \
    src/utils.h \
    src/translucentlabel.h \
    src/data.h \
    src/aqitooltip.h \
    src/categorybutton.h \
    src/indexitemwidget.h \
    src/texttip.h \
    src/tipmodule.h \
    src/windtooltip.h \
    src/cityview.h \
    src/citymodel.h \
    src/citydelegate.h \
    src/settingdialog.h \
    src/settingtitlebar.h \
    src/citywidget.h \
    src/activebutton.h \
    src/citybtngroups.h

TRANSLATIONS += \
    translation/xiaoming-weather_zh_CN.ts

RESOURCES += \
    res.qrc
