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
#CONFIG += link_pkgconfig
#PKGCONFIG += geoip#libgeoip-dev

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
    src/parseinterface.cpp

HEADERS  += src/mainwindow.h \
    src/titlebar.h \
    src/toolbutton.h \
    src/weatherwidget.h \
    src/backgroundwidget.h \
    src/parseinterface.h \
    src/utils.h

TRANSLATIONS += \
    translation/xiaoming-weather_zh_CN.ts

RESOURCES += \
    res.qrc
