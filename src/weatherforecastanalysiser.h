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

#ifndef WEATHERFORECASTANALYSISER_H
#define WEATHERFORECASTANALYSISER_H

#include "weatherbaseanalysiser.h"
#include "data.h"

class WeatherForecastAnalysiser : public WeatherBaseAnalysiser
{
    Q_OBJECT

public:
    explicit WeatherForecastAnalysiser(QObject *parent = 0);
    ~WeatherForecastAnalysiser();

    QList<ForecastWeather> getForecastData();

protected:
    void parseJson(QJsonObject &jsonObject) Q_DECL_OVERRIDE;

private:
    QList<ForecastWeather> m_forecastListData;
};

#endif // WEATHERFORECASTANALYSISER_H
