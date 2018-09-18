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

#include "weatherforecastwidget.h"
#include "forecastitemwidget.h"
#include "translucentlabel.h"

#include <QPainter>
#include <QDebug>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QGridLayout>

static const int CycleDotSize = 3;
static const int TemperatureStartXPos = 40;

WeatherForecastWidget::WeatherForecastWidget(QWidget *parent)
    : QFrame(parent)
{
    setMouseTracking(true);

    main_splitter = new QSplitter();
    main_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main_splitter->setOrientation(Qt::Vertical);
    main_splitter->setHandleWidth(1);

    this->initTopWidget();
    this->initBottomWidget();

    m_bottomWidget->setFixedSize(413, 105);
    main_splitter->addWidget(m_topWidget);
    main_splitter->addWidget(m_bottomWidget);

    for(int i = 0; i<main_splitter->count();i++) {
        QSplitterHandle *handle = main_splitter->handle(i);
        handle->setEnabled(false);
    }

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(main_splitter);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(main_layout);
}

WeatherForecastWidget::~WeatherForecastWidget()
{

}

void WeatherForecastWidget::initTopWidget()
{
    m_topWidget = new QWidget();

    QGridLayout *grid_layout = new QGridLayout();
    for(int i=0; i<3; i++) {
        ForecastItemWidget *item = new ForecastItemWidget;
        button_list.append(item);
        grid_layout->addWidget(item, 0, i%3);//, i/3, i%3
    }
    grid_layout->setVerticalSpacing(10);
    grid_layout->setContentsMargins(5, 0, 5, 5);

    QVBoxLayout *v_layout = new QVBoxLayout();
    v_layout->addStretch();
    v_layout->addLayout(grid_layout);
    v_layout->addStretch();
    v_layout->setSpacing(10);
    v_layout->setContentsMargins(0, 0, 0, 0);
    m_topWidget->setLayout(v_layout);
}

void WeatherForecastWidget::initBottomWidget()
{
    m_bottomWidget = new QWidget();
    //test background
//    m_bottomWidget->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::red));
//    m_bottomWidget->setPalette(palette);

    m_temperatureCurveLabel = new QLabel;
    m_temperatureCurveLabel->setFixedSize(413, 105);
    m_temperatureCurveLabel->installEventFilter(this);
    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addWidget(m_temperatureCurveLabel);
    h_layout->setSpacing(0);
    h_layout->setContentsMargins(0, 0, 0, 0);

    m_bottomWidget->setLayout(h_layout);
}

bool WeatherForecastWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_temperatureCurveLabel && event->type() == QEvent::Paint) {
        paintTemperatureCurve();
    }

    return QWidget::eventFilter(obj, event);
}

void WeatherForecastWidget::paintTemperatureCurve()
{
    int i = 0;
    forecast[i].aqi = "23";
    forecast[i].date = "20180807";
    forecast[i].high = "33";
    forecast[i].low = "24";
    forecast[i].type = "undefined";

    i = 1;
    forecast[i].aqi = "25";
    forecast[i].date = "20180808";
    forecast[i].high = "32";
    forecast[i].low = "26";
    forecast[i].type = "undefined";

    i = 2;
    forecast[i].aqi = "28";
    forecast[i].date = "20180809";
    forecast[i].high = "37";
    forecast[i].low = "26";
    forecast[i].type = "undefined";

    QPainter painter(m_temperatureCurveLabel);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int tempTotal = 0;
    int highTemp[3] = {};
    int lowTemp[3] = {};

    for (int i = 0; i < 3; i++) {
        highTemp[i] = forecast[i].high.toInt();
        tempTotal += highTemp[i];
        lowTemp[i] = forecast[i].low.toInt();
    }
    int tempAverage = (int)(tempTotal / 3);//最高温的平均值

    int pointX[3] = {51, 151, 251};//X
    int pointHY[3] = {0};
    int pointLY[3] = {0};
    for (int i = 0; i < 3; i++) {
        pointHY[i] = TemperatureStartXPos - ((highTemp[i] - tempAverage) * 2);
        pointLY[i] = TemperatureStartXPos + ((tempAverage - lowTemp[i]) * 2);
    }
    QPen pen = painter.pen();
    pen.setWidth(1);

    painter.save();
    pen.setColor(QColor("#fff222"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(QColor("#fff222"));
    painter.drawEllipse(QPoint(pointX[0], pointHY[0]), CycleDotSize, CycleDotSize);

    for (int i = 0; i < 2; i++) {
        painter.drawEllipse(QPoint(pointX[i+1], pointHY[i+1]), CycleDotSize, CycleDotSize);
        painter.drawLine(pointX[i], pointHY[i], pointX[i+1], pointHY[i+1]);
    }
    painter.restore();

    QPen penAve;
    penAve.setColor(Qt::red);
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);
    painter.setPen(penAve);
    painter.drawLine(51,TemperatureStartXPos,251,TemperatureStartXPos);

    pen.setColor(QColor("#00609a"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.setBrush(QColor("#00609a"));
    painter.drawEllipse(QPoint(pointX[0], pointLY[0]), CycleDotSize, CycleDotSize);

    for (int i = 0; i < 2; i++) {
        painter.drawEllipse(QPoint(pointX[i+1], pointLY[i+1]), CycleDotSize, CycleDotSize);
        painter.drawLine(pointX[i], pointLY[i], pointX[i+1], pointLY[i+1]);
    }
}

void WeatherForecastWidget::onDataChanged()
{

}
