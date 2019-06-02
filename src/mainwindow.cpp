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
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

inline void animationFromBottomToTop(QWidget *topWidget, QWidget *bottomWidget, int delay)
{
    /*
     * 页面切换的动画效果为从下往上滑
     * 显示bottomWidget，隐藏topWidget
     * 主界面为900×600,标题栏高度为39,所以天气显示界面为900×561
     * topWidget从QRect(0,0 900x561)处移动到QRect(0,-561 900x561)
     * bottomWidget从QRect(0,561 900x561)处移动到QRect(0,0 900x561)
     */

    bottomWidget->show();
    topWidget->show();

    QRect topStartRect = QRect(0, 0, topWidget->width(), topWidget->height());
    QRect topEndRect = QRect(0, -topWidget->height(), topWidget->width(), topWidget->height());
//    qDebug() << "FromBottomToTop: topStartRect" << topStartRect << ",topEndRect" << topEndRect;

    QPropertyAnimation *topAnimation = new QPropertyAnimation(topWidget, "geometry");
    topAnimation->setDuration(delay);
    topAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    topAnimation->setStartValue(topStartRect);
    topAnimation->setEndValue(topEndRect);
    topAnimation->start();
    topAnimation->connect(topAnimation, &QPropertyAnimation::finished, topAnimation, &QPropertyAnimation::deleteLater);
    topAnimation->connect(topAnimation, &QPropertyAnimation::finished, topWidget, &QWidget::hide);

    QRect bottomStartRect = QRect(0, topWidget->height(), bottomWidget->width(), bottomWidget->height());
    QRect bottomEndRect = QRect(0, 0, bottomWidget->width(), bottomWidget->height());
//    qDebug() << "FromBottomToTop: bottomStartRect" << bottomStartRect << ",bottomEndRect" << bottomEndRect;
    QPropertyAnimation *bottomAnimation = new QPropertyAnimation(bottomWidget, "geometry");
    bottomAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    bottomAnimation->setDuration(delay);
    bottomAnimation->setStartValue(bottomStartRect);
    bottomAnimation->setEndValue(bottomEndRect);
    bottomAnimation->start();
    bottomAnimation->connect(bottomAnimation, &QPropertyAnimation::finished, bottomAnimation, &QPropertyAnimation::deleteLater);

    auto bottomOpacity = new QGraphicsOpacityEffect(bottomWidget);
    bottomWidget->setGraphicsEffect(bottomOpacity);
    bottomOpacity->setOpacity(0);

    QPropertyAnimation *bottomOpacityAnimation = new QPropertyAnimation(bottomOpacity, "opacity");
    bottomOpacityAnimation->setEasingCurve(QEasingCurve::InCubic);
    bottomOpacityAnimation->setDuration(delay);
    bottomOpacityAnimation->setStartValue(0.0);
    bottomOpacityAnimation->setEndValue(1.0);
    bottomOpacityAnimation->start();
    bottomOpacityAnimation->connect(bottomOpacityAnimation, &QPropertyAnimation::finished, bottomOpacityAnimation, &QPropertyAnimation::deleteLater);
    bottomOpacityAnimation->connect(bottomOpacityAnimation, &QPropertyAnimation::finished, bottomWidget, [ = ]() {
        bottomWidget->setGraphicsEffect(nullptr);
    });

    auto topOpacity = new QGraphicsOpacityEffect();
    topWidget->setGraphicsEffect(topOpacity);
    topOpacity->setOpacity(1);

    QPropertyAnimation *topOpacityAnimation = new QPropertyAnimation(topOpacity, "opacity");
    topOpacityAnimation->setEasingCurve(QEasingCurve::InCubic);
    topOpacityAnimation->setDuration(delay);
    topOpacityAnimation->setStartValue(1.0);
    topOpacityAnimation->setEndValue(0.0);
    topOpacityAnimation->start();
    topOpacityAnimation->connect(topOpacityAnimation, &QPropertyAnimation::finished, topOpacityAnimation, &QPropertyAnimation::deleteLater);
    topOpacityAnimation->connect(topOpacityAnimation, &QPropertyAnimation::finished, topWidget, [ = ]() {
        topWidget->setGraphicsEffect(nullptr);
    });
}

inline void animationFromTopToBottom(QWidget *topWidget, QWidget *bottomWidget, int delay)
{
    /*
     * 页面切换的动画效果为从上往上下滑
     * 显示bottomWidget，隐藏topWidget
     * 主界面为900×600,标题栏高度为39,所以天气显示界面为900×561
     * topWidget从QRect(0,0 900x561)处移动到QRect(0,561 900x561)
     * bottomWidget从QRect(0,-561 900x561)处移动到QRect(0,0 900x561)
     */

    bottomWidget->show();
    topWidget->show();
    bottomWidget->resize(topWidget->size());

    QRect topStartRect = QRect(0, 0, topWidget->width(), topWidget->height());
    QRect topEndRect = QRect(0, topWidget->height(), topWidget->width(), topWidget->height());
//    qDebug() << "FromTopToBottom: topStartRect" << topStartRect << ",topEndRect" << topEndRect;

    QPropertyAnimation *topAnimation = new QPropertyAnimation(topWidget, "geometry");
    topAnimation->setDuration(delay);
    topAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    topAnimation->setStartValue(topStartRect);
    topAnimation->setEndValue(topEndRect);
    topAnimation->start();
    topAnimation->connect(topAnimation, &QPropertyAnimation::finished, topAnimation, &QPropertyAnimation::deleteLater);
    topAnimation->connect(topAnimation, &QPropertyAnimation::finished, topWidget, &QWidget::hide);

    QRect bottomStartRect = QRect(0, -topWidget->height(), bottomWidget->width(), bottomWidget->height());
    QRect bottomEndRect = QRect(0, 0, bottomWidget->width(), bottomWidget->height());
//    qDebug() << "FromTopToBottom: bottomStartRect" << bottomStartRect << ",bottomEndRect" << bottomEndRect;

    QPropertyAnimation *bottomAnimation = new QPropertyAnimation(bottomWidget, "geometry");
    bottomAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    bottomAnimation->setDuration(delay);
    bottomAnimation->setStartValue(bottomStartRect);
    bottomAnimation->setEndValue(bottomEndRect);
    bottomAnimation->start();
    bottomAnimation->connect(bottomAnimation, &QPropertyAnimation::finished, bottomAnimation, &QPropertyAnimation::deleteLater);

    auto bottomOpacity = new QGraphicsOpacityEffect(bottomWidget);
    bottomWidget->setGraphicsEffect(bottomOpacity);
    bottomOpacity->setOpacity(0);

    QPropertyAnimation *bottomOpacityAnimation = new QPropertyAnimation(bottomOpacity, "opacity");
    bottomOpacityAnimation->setEasingCurve(QEasingCurve::InCubic);
    bottomOpacityAnimation->setDuration(delay);
    bottomOpacityAnimation->setStartValue(0.0);
    bottomOpacityAnimation->setEndValue(1.0);
    bottomOpacityAnimation->start();
    bottomOpacityAnimation->connect(bottomOpacityAnimation, &QPropertyAnimation::finished, bottomOpacityAnimation, &QPropertyAnimation::deleteLater);
    bottomOpacityAnimation->connect(bottomOpacityAnimation, &QPropertyAnimation::finished, bottomWidget, [ = ]() {
        bottomWidget->setGraphicsEffect(nullptr);
    });

    auto topOpacity = new QGraphicsOpacityEffect(topWidget);
    topWidget->setGraphicsEffect(topOpacity);
    topOpacity->setOpacity(1);

    QPropertyAnimation *topOpacityAnimation = new QPropertyAnimation(topOpacity, "opacity");
    topOpacityAnimation->setEasingCurve(QEasingCurve::InCubic);
    topOpacityAnimation->setDuration(delay);
    topOpacityAnimation->setStartValue(1.0);
    topOpacityAnimation->setEndValue(0.0);
    topOpacityAnimation->start();
    topOpacityAnimation->connect(topOpacityAnimation, &QPropertyAnimation::finished, topOpacityAnimation, &QPropertyAnimation::deleteLater);
    topOpacityAnimation->connect(topOpacityAnimation, &QPropertyAnimation::finished, bottomWidget, [ = ]() {
        topWidget->setGraphicsEffect(nullptr);
    });
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mousePressed(false)
    , m_isNight(false)
    , m_weatherWorker(new WeatherWorker(this))//g_weatherWorker
{
    this->setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);//需要加上Qt::WindowMinimizeButtonHint，否则showMinimized无效
    this->setMouseTracking(true);
    this->setFocusPolicy(Qt::ClickFocus);
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

    /*m_centerWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_centerWidget);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_titleBar = new TitleBar(this);
    m_titleBar->setFixedSize(width(), 39);
    this->setMenuWidget(m_titleBar);

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

    this->setCentralWidget(m_centerWidget);*/

    m_titleBar = new TitleBar(this);
    m_titleBar->setFixedSize(width(), TITLE_HEIGHT);
    this->setMenuWidget(m_titleBar);

    m_weatherWidget = new WeatherWidget(this);;
    m_weatherWidget->setFixedWidth(width());//高度上减去标题栏的高度
    m_weatherWidget->setContentsMargins(0, 0, 0, 0);

    m_locationWidget = new LocationWidget(this);
    m_locationWidget->setFixedWidth(width());//高度上减去标题栏的高度
    m_locationWidget->setContentsMargins(0, 0, 0, 0);
    m_centerWidget = new QWidget;
    QStackedLayout *m_stackedLayout = new QStackedLayout(m_centerWidget);
    this->setCentralWidget(m_centerWidget);
    m_stackedLayout->setContentsMargins(0, 0, 0, 0);
    m_stackedLayout->setMargin(0);
    m_stackedLayout->setSpacing(0);
    m_stackedLayout->addWidget(m_weatherWidget);
    m_stackedLayout->addWidget(m_locationWidget);
    //m_stackedLayout->setCurrentWidget(m_weatherWidget);
    m_weatherWidget->show();

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
//        m_stackedLayout->setCurrentWidget(m_locationWidget);
        animationFromTopToBottom(m_weatherWidget, m_locationWidget, 500);
        this->update();
    });
    connect(m_locationWidget, &LocationWidget::backBtnClicked, this, [this, m_stackedLayout] {
//        m_stackedLayout->setCurrentWidget(m_weatherWidget);
        animationFromBottomToTop(m_locationWidget, m_weatherWidget, 500);
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

    if (m_weatherWidget != nullptr) {
        delete m_weatherWidget;
        m_weatherWidget = nullptr;
    }
    if (m_locationWidget != nullptr) {
        delete m_locationWidget;
        m_locationWidget = nullptr;
    }
    if (m_centerWidget != nullptr) {
        delete m_centerWidget;
        m_centerWidget = nullptr;
    }
}

void MainWindow::showSettingDialog()
{

}

void MainWindow::createSettingDialog()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

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
