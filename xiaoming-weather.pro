#-------------------------------------------------
#
# Project created by QtCreator 2018-08-10T09:33:44
#
#-------------------------------------------------

QT       += network xml svg

isEqual(QT_MAJOR_VERSION, 5) {
    QT += widgets gui core dbus
}

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
CONFIG += link_pkgconfig
PKGCONFIG += geoip

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
    src/weathercategorywidget.cpp \
    src/navigationwidget.cpp \
    src/citynavigation.cpp \
    src/imagebutton.cpp \
    src/locationwidget.cpp \
    src/forecastitemwidget.cpp \
    src/translucentlabel.cpp \
    src/aqitooltip.cpp \
    src/categorybutton.cpp \
    src/indexitemwidget.cpp \
    src/lifestyletip.cpp \
    src/tipmodule.cpp \
    src/windtooltip.cpp \
    src/cityview.cpp \
    src/citymodel.cpp \
    src/citydelegate.cpp \
    src/settingdialog.cpp \
    src/settingtitlebar.cpp \
    src/citywidget.cpp \
    src/activebutton.cpp \
    src/citybtngroups.cpp \
    src/preferences.cpp \
    src/global.cpp \
    src/weatherworker.cpp \
    src/automaticlocation.cpp \
    src/weatherbaseanalysiser.cpp \
    src/weatherairanalysiser.cpp \
    src/weathernowanalysiser.cpp \
    src/weatherforecastanalysiser.cpp \
    src/weatherlifestyleanalysiser.cpp \
    src/forecasttip.cpp

HEADERS  += src/mainwindow.h \
    src/titlebar.h \
    src/toolbutton.h \
    src/weatherwidget.h \
    src/backgroundwidget.h \
    src/parseinterface.h \
    src/weathernowwidget.h \
    src/weatherforecastwidget.h \
    src/weathercategorywidget.h \
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
    src/lifestyletip.h \
    src/tipmodule.h \
    src/windtooltip.h \
    src/cityview.h \
    src/citymodel.h \
    src/citydelegate.h \
    src/settingdialog.h \
    src/settingtitlebar.h \
    src/citywidget.h \
    src/activebutton.h \
    src/citybtngroups.h \
    src/preferences.h \
    src/global.h \
    src/weatherworker.h \
    src/automaticlocation.h \
    src/weatherbaseanalysiser.h \
    src/weatherairanalysiser.h \
    src/weathernowanalysiser.h \
    src/weatherforecastanalysiser.h \
    src/weatherlifestyleanalysiser.h \
    src/forecasttip.h

TRANSLATIONS += \
    translation/xiaoming-weather_zh_CN.ts

RESOURCES += \
    res.qrc
