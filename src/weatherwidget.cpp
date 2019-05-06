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

#include "weatherwidget.h"
#include "weathernowwidget.h"
#include "weatherforecastwidget.h"
#include "weathercategorywidget.h"
#include "navigationwidget.h"

#include <QDebug>
#include <QHBoxLayout>

WeatherWidget::WeatherWidget(QWidget *parent)
    : QFrame(parent)
{
//    this->setFixedSize(900, 600);
//    this->setStyleSheet("QFrame {border-image: url(':/res/background/weather-snow.jpg');color:rgb(255, 255, 255);}");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(1);

    m_nowWidget = new WeatherNowWidget(this);
    m_nowWidget->setFixedSize(300, 340);
    m_forecastWidget = new WeatherForecastWidget(this);
    m_forecastWidget->setFixedSize(600, 340);
    m_categoryWidget = new WeatherCategoryWidget(this);
    m_categoryWidget->setFixedSize(900, 160);
    m_navigationWidget = new NavigationWidget(this, 5);//5是默认城市的个数
    m_navigationWidget->setFixedWidth(900);

    QWidget *w = new QWidget;
    QHBoxLayout *m_topLayout = new QHBoxLayout(w);
    m_topLayout->setContentsMargins(0, 0, 0, 0);
    m_topLayout->setSpacing(1);
    m_layout->addWidget(w, 0, Qt::AlignTop);
    m_topLayout->addWidget(m_nowWidget);
    m_topLayout->addWidget(m_forecastWidget);

    m_layout->addWidget(m_categoryWidget);
    m_layout->addWidget(m_navigationWidget/*, 0, Qt::AlignBottom*/);

    connect(m_navigationWidget, &NavigationWidget::requestNextCity, this, [=] {
        //TODO:刷新城市天气信息 & 设置所有页面模块的数据
        this->onUpdateWeather();
    });
    connect(m_navigationWidget, &NavigationWidget::requestPrevCity, this, [=] {
        //TODO:刷新城市天气信息 & 设置所有页面模块的数据
        this->onUpdateWeather();
    });

    connect(m_nowWidget, &WeatherNowWidget::locationBtnClicked, this, &WeatherWidget::locationBtnClicked);
}

WeatherWidget::~WeatherWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

//天气数据获取完毕后开始更新界面数据
void WeatherWidget::onUpdateWeather()
{
    m_nowWidget->refreshNowWeatherData();
    m_forecastWidget->refreshForecastWeatherData();
    m_categoryWidget->refreshLifestyleData();
}

//空气质量数据获取完毕后开始更新界面数据
void WeatherWidget::onUpdateAqi()
{

}
