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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "data.h"

class Preferences {
public:
    Preferences();
    ~Preferences();

    void setDefaultValue();
    void save();
    void load();

    bool isCityContains(const QString &id);
    QList<QString> getCitiesList();
    QList<QString> getCityIdList();
    QString getCityNameById(const QString &id);
    int citiesCount();

    void setCurrentCityIdByName(const QString &name);
    void setCurrentCityNameById(const QString &id);
    void addCityInfoToPref(City city);
    void removeCityInfoFromPref(const QString &id, bool isActive);
    bool isCityIdExist(const QString &id);
    bool isCitiesCountOverMax();
    void setDefaultCity();

    QString m_currentCityId;
    QString m_currentCity;
    QString m_updateTime;
    int m_maxCityItems;
    QMap<QString, QString> m_cityMap;

    ObserveWeather m_observeWeather;
    QList<ForecastWeather> m_forecasts;
    LifeStyle m_lifestyle;
    Air m_air;
};

#endif // PREFERENCES_H
