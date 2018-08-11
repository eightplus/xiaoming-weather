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

#ifndef WEATHER_FORECAST_WIDGET_H
#define WEATHER_FORECAST_WIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

class WeatherForecastWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherForecastWidget(QWidget *parent = 0);
    ~ WeatherForecastWidget();

private:
    QVBoxLayout *m_layout = nullptr;
    QLabel *m_label = nullptr;
};

#endif // WEATHER_FORECAST_WIDGET_H
