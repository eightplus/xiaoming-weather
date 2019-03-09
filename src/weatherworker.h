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

#ifndef WEATHERWORKER_H
#define WEATHERWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QHostInfo>

#include "data.h"

class AutomaticLocation;
class WeatherBaseAnalysiser;
class WeatherAirAnalysiser;
class WeatherNowAnalysiser;
class WeatherForecastAnalysiser;
class WeatherLifestyleAnalysiser;

#define g_weatherWorker WeatherWorker::getInstance()

class WeatherWorker : public QObject
{
    Q_OBJECT

public:
    enum WeatherType
    {
        W_Now_Type = 0,
        W_Forecast_Type,
        W_LifeStyle_Type,
        W_Air_Type
    };

    explicit WeatherWorker(QObject *parent = 0);
    ~WeatherWorker();

    static WeatherWorker *getInstance(void);
    QList<ForecastWeather> getForecastList();
    void requestSearchCityByInput(const QString &input);
    void requestWeatherDataByLongitudeAndLatitude(double latitude, double longitude);
    void requestWeatherAndApiDataById(const QString &cityId);
    void setCity(const QString &city);
    void requestAccessGeoNameIDByLongitudeAndLatitude(double latitude, double longitude);
    void requestAccessGeoNameDataByGeonameId(const QString &geonameId);

    ObserveWeather getObserveData();
    QList<ForecastWeather> getForecastData();
    QList<LifeStyle> getLifestyleData();
    Air getAirData();

public slots:
    void onWeatherDataReply();
    void onApiDataReply();
    void onSearchCityReply();
    void setAutoCity(const QString& cityName);

private slots:
    void onGeoNameIdByLongitudeAndLatitudeReply();
    void onGeoNameDataByGeonameIdReply();

signals:
    void readyUpdateWeather();

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_city;
    AutomaticLocation *m_automatic = nullptr;
    QList<ForecastWeather> m_forecastList;
    QMap<WeatherType, WeatherBaseAnalysiser*> m_weatherMap;
    WeatherType m_weatherType;
};

#endif // WEATHERWORKER_H
