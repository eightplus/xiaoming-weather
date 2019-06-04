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

#include "weatherforecastwidget.h"
#include "forecastitemwidget.h"
#include "scroller.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QPainter>
#include <QDebug>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QGridLayout>

namespace {

int ITEM_COUNTS = 3;

}

WeatherForecastWidget::WeatherForecastWidget(QWidget *parent)
    : QFrame(parent)
{
    this->setMouseTracking(true);

    m_displayWidget = new QWidget();
    m_displayWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
#if QT_VERSION >= 0x050000
    Scroller::setScroller(m_displayWidget);
#endif
//    QGridLayout *m_gridLayout = new QGridLayout();
//    for(int i=0; i<ITEM_COUNTS; i++) {
//        ForecastItemWidget *item = new ForecastItemWidget;
//        m_items.append(item);
//        m_gridLayout->addWidget(item, 0, i%ITEM_COUNTS);//, i/ITEM_COUNTS, i%ITEM_COUNTS
//    }
//    m_gridLayout->setVerticalSpacing(5);
//    m_gridLayout->setContentsMargins(0, 0, 0, 0);
//    m_displayWidget->setLayout(m_gridLayout);
    m_gridLayout = new QGridLayout();
    m_gridLayout->setVerticalSpacing(5);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_displayWidget->setLayout(m_gridLayout);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->addWidget(m_displayWidget);
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
}

WeatherForecastWidget::~WeatherForecastWidget()
{
//    QLayoutItem *child;
//    while ((child = m_layout->takeAt(0)) != 0) {
//        if (child->widget())
//            child->widget()->deleteLater();
//        delete child;
//    }
    for (int i=0; i<m_items.count(); i++) {
        ForecastItemWidget *item = m_items.at(i);
        delete item;
        item = nullptr;
    }
    m_items.clear();

    // clear
    while (QLayoutItem *item = m_gridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }

    delete m_displayWidget;
}


void WeatherForecastWidget::refreshForecastWeatherData()
{
    for (int i=0; i<m_items.count(); i++) {
        ForecastItemWidget *item = m_items.at(i);
        delete item;
        item = nullptr;
    }
    m_items.clear();

    // 清空原来的界面(为了动态适应api获取的天气预报的天数)
    while (QLayoutItem *item = m_gridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }

    // 根据新的预报设置新的界面
    int count = m_preferences->m_forecasts.size();
    for(int i=0; i<count; i++) {
        ForecastItemWidget *item = new ForecastItemWidget;
        m_items.append(item);
        m_gridLayout->addWidget(item, 0, i%count);//, i/ITEM_COUNTS, i%ITEM_COUNTS
    }

    for (int i=0; i<m_items.count(); i++) {
        ForecastItemWidget *item = m_items.at(i);
        if (m_preferences->m_observeWeather.cond_code.contains(QChar('n'))) {
            item->setNightStyleSheets();
        }
        else {
            item->setDayStyleSheets();
        }
        item->resetForecastData(m_preferences->m_forecasts.at(i), i);
    }
}
