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
    QGridLayout *grid_layout = new QGridLayout();
    for(int i=0; i<ITEM_COUNTS; i++) {
        ForecastItemWidget *item = new ForecastItemWidget;
        m_items.append(item);
        grid_layout->addWidget(item, 0, i%ITEM_COUNTS);//, i/ITEM_COUNTS, i%ITEM_COUNTS
    }
    grid_layout->setVerticalSpacing(5);
    grid_layout->setContentsMargins(0, 0, 0, 0);
    m_displayWidget->setLayout(grid_layout);

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

    delete m_displayWidget;
}


void WeatherForecastWidget::refreshForecastWeatherData()
{
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
