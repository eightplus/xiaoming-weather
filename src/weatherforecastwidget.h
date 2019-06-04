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

#ifndef WEATHER_FORECAST_WIDGET_H
#define WEATHER_FORECAST_WIDGET_H

#include <QFrame>

#include "data.h"

class ForecastItemWidget;
class QGridLayout;

class WeatherForecastWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherForecastWidget(QWidget *parent = 0);
    ~ WeatherForecastWidget();

    //void onDataChanged();
    void refreshForecastWeatherData();

private:
    QWidget *m_displayWidget = nullptr;
    QGridLayout *m_gridLayout = nullptr;
    QList<ForecastItemWidget *> m_items;
    Forecast forecast[6];
};

#endif // WEATHER_FORECAST_WIDGET_H
