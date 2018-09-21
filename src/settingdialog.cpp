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

#include "settingdialog.h"
#include "settingtitlebar.h"
#include "citywidget.h"

#include <QApplication>
#include <QDesktopServices>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QDebug>
#include <QPainter>

//#include "preferences.h"
//#include "global.h"
//using namespace Global;

namespace {
const int BUTTON_WIDGET_HEIGHT = 30;
const int GROUP_BOX_MARGIN = 20;
}

SettingDialog::SettingDialog(QWidget *parent):
    QDialog(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_stackedWidget(new QStackedWidget)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(479, 421);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setWindowTitle(tr("Xiaoming Weather - Setting"));
    this->setWindowIcon(QIcon(":/res/xiaoming-weather.png"));
    //Why????? setStyleSheet将导致添加城市后，列表没有自动拉伸，出现重叠，但是qDebug打印的高度确实增加了，但是使用paintEvent可以
    //this->setStyleSheet("QDialog{border:1px solid #000000;border-radius:2px;background:rgba(255, 255, 255, 0.7);}QDialog:hover{background: rgba(255, 255, 255, 1.0);}");

    m_mainLayout->setContentsMargins(1, 1, 1, 1);
    m_mainLayout->setSpacing(0);

    m_titleBar = new SettingTitleBar;
    m_titleBar->setFixedHeight(100);

    m_cityWidget = new CityWidget;
    m_cityWidget->setFixedHeight(this->height() - m_titleBar->height() - 2/* - BUTTON_WIDGET_HEIGHT*/);
    m_cityWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_systemWidget = new QWidget;
    m_systemWidget->setStyleSheet("QWidget{border-radius: 0px; background-color:#ffffff;}");
    m_systemWidget->setFixedHeight(this->height() - m_titleBar->height() - 2/* - BUTTON_WIDGET_HEIGHT*/);
    m_systemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

//    connect(m_cityWidget, &CityWidget::responseCityError, m_titleBar, &SettingTitleBar::showWarnInfo);

    m_stackedWidget->addWidget(m_cityWidget);
    m_stackedWidget->addWidget(m_systemWidget);
    m_stackedWidget->setCurrentWidget(m_cityWidget);

    m_mainLayout->addWidget(m_titleBar, 0, Qt::AlignTop);
    m_mainLayout->addWidget(m_stackedWidget, 1, Qt::AlignVCenter);
    m_mainLayout->addStretch();

    connect(m_titleBar, &SettingTitleBar::requestCloseDialog, this, [=] {
        this->reject();
    });
//    connect(m_cityWidget, &CityWidget::requestAddCity, this, [=] {
//    });

    //connect(m_cityWidget, &CityWidget::requestRefreshCityMenu, this, &SettingDialog::requestRefreshCityMenu);
    //connect(m_cityWidget, &CityWidget::requestRefreshWeatherById, this, &SettingDialog::requestRefreshWeatherById);
    connect(m_titleBar, &SettingTitleBar::requestSwitchPage, this, [=] (bool b) {
        if (b) {
            m_stackedWidget->setCurrentWidget(m_cityWidget);
        }
        else {
            m_stackedWidget->setCurrentWidget(m_systemWidget);
        }
    });
}

SettingDialog::~SettingDialog()
{
    QLayoutItem *child;
    while ((child = m_mainLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void SettingDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void SettingDialog::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePressed = false;
}

void SettingDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePressed) {
        QPoint move_pos = event->globalPos();
        move(move_pos - m_dragPosition);
        event->accept();
    }
}

void SettingDialog::setData()
{

}

void SettingDialog::refreshCityList(const QString &id)
{
//    if (m_cityWidget) {
//        m_cityWidget->refreshCityList(id);
//    }
}

void SettingDialog::accept()
{
    hide();
    setResult(QDialog::Accepted);

    emit applied();
}

void SettingDialog::reject()
{
    hide();
    setResult(QDialog::Rejected);
    setResult(QDialog::Accepted);
}

void SettingDialog::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect borderRect;
    borderRect.setRect(this->rect().x(), this->rect().y(), this->rect().width(), this->rect().height());
    QPainterPath inBorderPath;
    inBorderPath.addRoundedRect(borderRect, 2, 2);
    painter.setClipPath(inBorderPath);

    painter.fillRect(0, 0, width(), height(), QBrush(QColor("#000000")));
}
