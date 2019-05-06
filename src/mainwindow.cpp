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
#include "titlebar.h"
#include "weatherwidget.h"
#include "locationwidget.h"
#include "backgroundwidget.h"
#include "settingdialog.h"
#include "utils.h"
#include "weatherworker.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QScreen>
#include <QDesktopServices>
#include <QStackedLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mousePressed(false)
    , m_isNight(false)
    , m_weatherWorker(new WeatherWorker(this))//g_weatherWorker
{
    this->setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);//需要加上Qt::WindowMinimizeButtonHint，否则showMinimized无效
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAutoFillBackground(true);
    this->setWindowTitle(tr("Xiaoming Weather"));
    this->setWindowIcon(QIcon(":/res/xiaoming-weather.png"));
//    this->setWindowOpacity(0.9);
    this->setWindowOpacity(1.0);

//    m_weatherWorker = new WeatherWorker(this);

    global_init();

    m_backgroundWidget = new BackgroundWidget(this);
    m_backgroundWidget->setGeometry(rect());
    m_backgroundWidget->lower();
    m_backgroundWidget->setForNight(m_isNight);

    //如果不使用BackgroundWidget类作为背景，则可用setStyleSheet来设置QMainWindow的背景图片
//    this->setStyleSheet("QMainWindow {color:white; background-image:url(':/res/background/weather-clear.jpg'); background-repeat:no-repeat; background-color:rgba(255, 255, 255, 230);}");

    m_centerWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centerWidget);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_titleBar = new TitleBar(this);
    m_titleBar->setFixedSize(width(), 39);

    m_weatherWidget = new WeatherWidget(this);
    m_weatherWidget->setFixedWidth(width());

    m_locationWidget = new LocationWidget(this);
    m_locationWidget->setFixedWidth(width());

    QStackedLayout *m_stackedLayout = new QStackedLayout;
    m_stackedLayout->setSpacing(0);
    m_stackedLayout->setMargin(0);
    m_stackedLayout->addWidget(m_weatherWidget);
    m_stackedLayout->addWidget(m_locationWidget);
    m_stackedLayout->setCurrentWidget(m_weatherWidget);

    m_layout->addWidget(m_titleBar);
    m_layout->addLayout(m_stackedLayout);
//    m_layout->addWidget(m_weatherWidget);

    this->setCentralWidget(m_centerWidget);

    this->createSettingDialog();

    this->moveCenter();

    connect(m_titleBar, &TitleBar::switchDayOrNight, this, [=] {
        m_isNight = !m_isNight;
        m_backgroundWidget->setForNight(m_isNight);
    });
    connect(m_titleBar, &TitleBar::requestDisplayAboutDialog, this, [=] {
        this->showSettingDialog();
    });
    connect(m_weatherWidget, &WeatherWidget::locationBtnClicked, this, [this, m_stackedLayout] {
        //TODO:弹出设置城市界面，城市设置完毕后，刷新城市天气信息 & 设置所有页面模块的数据
        m_stackedLayout->setCurrentWidget(m_locationWidget);
    });
    connect(m_locationWidget, &LocationWidget::backBtnClicked, this, [this, m_stackedLayout] {
        m_stackedLayout->setCurrentWidget(m_weatherWidget);
    });

//    system("xterm -e '"
//           "sudo service network-manager restart"
//           " && "
//           "exit"
//           "; exec bash'");

//    xterm -e "cd /etc; bash"

    connect(m_weatherWorker, &WeatherWorker::readyUpdateWeather, this, [=] () {
        m_weatherWidget->onUpdateWeather();
    });
    connect(m_weatherWorker, &WeatherWorker::readyUpdateAqi, this, [=] () {
        m_weatherWidget->onUpdateAqi();
    });
}

MainWindow::~MainWindow()
{
    global_end();
}


void MainWindow::showSettingDialog()
{
    m_setttingDialog->move((width() - m_setttingDialog->width()) / 2 + mapToGlobal(QPoint(0, 0)).x(),
                               (window()->height() - m_setttingDialog->height()) / 2 + mapToGlobal(QPoint(0, 0)).y());
    m_setttingDialog->show();
}

void MainWindow::createSettingDialog()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_setttingDialog = new SettingDialog;
    m_setttingDialog->setModal(false);
    m_setttingDialog->fillCityList();
//    connect(m_setttingDialog, SIGNAL(applied()), this, SLOT(applySettings()));
//    connect(m_setttingDialog, &SettingDialog::requestRefreshCityMenu, this, [this] (bool removedDefault) {
//        this->refreshCityActions();

//        if (removedDefault) {//刪除了默认城市后，重新设置了列表中第一个城市为默认城市后，从服务端获取该城市的天气
//            this->startGetWeather();
//        }
//    });
//    connect(m_setttingDialog, &SettingDialog::requestRefreshWeatherById, this, [this] (const QString &id) {
//        m_preferences->setCurrentCityNameById(id);
//        this->refreshCityActions();
//        this->startGetWeather();
//    });
//    connect(m_setttingDialog, &SettingDialog::requestChangeOpacity, this, [this] (int opcatity) {
//        double value = opcatity*0.01;
//        if (value < 0.6) {
//            value = 0.60;
//            m_preferences->m_opacity = 60;
//        }
//        this->setOpacity(value);
//    });



//    connect(m_setttingDialog, &SettingDialog::requestSetDefaultCity, this, [=] {
//        m_preferences->setDefaultCity();
//        m_setttingDialog->refreshCityList(m_preferences->m_currentCityId);
//        //this->startGetWeather();
//    });

    QApplication::restoreOverrideCursor();
}

void MainWindow::moveCenter()
{
    QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    this->move(primaryGeometry.x() + (primaryGeometry.width() - this->width())/2,
               primaryGeometry.y() + (primaryGeometry.height() - this->height())/2);
    this->show();
    this->raise();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        this->m_mousePressed = true;
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_mousePressed = false;

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (this->m_mousePressed) {
        move(event->globalPos() - this->m_dragPosition);
    }

    QMainWindow::mouseMoveEvent(event);
}
