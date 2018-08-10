/*
 * Copyright (C) 2018 kobe24_lixiang@126.com
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
#include "backgroundwidget.h"
#include "utils.h"

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QScreen>
#include <QDesktopServices>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_mousePressed(false)
    , m_isNight(false)
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

    m_layout->addWidget(m_titleBar);
    m_layout->addWidget(m_weatherWidget);

    this->setCentralWidget(m_centerWidget);

    this->moveCenter();

    connect(m_titleBar, &TitleBar::switchDayOrNight, this, [=] {
        m_isNight = !m_isNight;
        m_backgroundWidget->setForNight(m_isNight);
    });
}

MainWindow::~MainWindow()
{

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
