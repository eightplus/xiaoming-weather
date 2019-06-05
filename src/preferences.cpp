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

#include "preferences.h"

#include <QSettings>
#include <QDebug>

#include "global.h"
using namespace Global;

Preferences::Preferences()
{
    m_maxCityItems = 10;

    //TODO
    load();
}

Preferences::~Preferences()
{
    save();
    m_cityMap.clear();
}

void Preferences::setDefaultValue()
{
    m_defaultId = "CN101250101";
    m_currentId = m_defaultId;
    m_defaultCity = "长沙";

    City city;
    city.id = m_defaultId;
    city.name = m_defaultCity;
    m_cityMap.insert(city.id, city.name);

    City city1;
    city1.id = "CN101010100";
    city1.name = "北京";
    m_cityMap.insert(city1.id, city1.name);

    City city2;
    city2.id = "CN101020100";
    city2.name = "上海";
    m_cityMap.insert(city2.id, city2.name);
}

void Preferences::save()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    set->setValue("default_city_id", m_defaultId);
    set->setValue("default_city", m_defaultCity);

    set->remove("city_list");
    bool save_city_list = true;
    QMap<QString, QString>::const_iterator iter = m_cityMap.constBegin();
    if (m_cityMap.size() == 1 && iter.key().isEmpty())
        save_city_list = false;
    if (save_city_list) {
        set->beginWriteArray("city_list");
        int count = 0;
        while (iter != m_cityMap.constEnd()) {
            set->setArrayIndex(count);
            set->setValue("id", iter.key());
            set->setValue("name", iter.value());
            iter++;
            count++;
        }
        set->endArray();
    }
    set->endGroup();

    set->sync();
}

void Preferences::load()
{
    QSettings *set = m_settings;

    set->beginGroup("City");
    m_defaultId = set->value("default_city_id", m_defaultId).toString();
    if (m_defaultId.isEmpty()) {
        qDebug() << "AAAAAAAAAAAAAAAA";
        this->setDefaultValue();
    }
    else {
        m_currentId = m_defaultId;
        //TODO: 可能会出现id和cityname不匹配的情况
        m_defaultCity = set->value("default_city", m_defaultCity).toString();

        m_cityMap.clear();
        int cityCount = set->beginReadArray("city_list");
        if (cityCount > 0) {
            for (int i = 0; i < cityCount; ++i) {
                set->setArrayIndex(i);
                QString id = set->value("id").toString();
                QString name = set->value("name").toString();
                m_cityMap.insert(id, name);
            }
        }
        else {
            set->setArrayIndex(0);
            m_cityMap.insert(m_defaultId, m_defaultCity);
        }
        set->endArray();
    }

    set->endGroup();
}

bool Preferences::isCityContains(const QString &id)
{
    return m_cityMap.keys().contains(id);
}

QList<QString> Preferences::getCitiesList()
{
    return m_cityMap.values();
}

QList<QString> Preferences::getCityIdList()
{
    return m_cityMap.keys();
}

QString Preferences::getCityNameById(const QString &id)
{
    return m_cityMap.value(id, QString());

//    foreach (QString city, m_cityMap.values()) {
//    }
}

int Preferences::citiesCount()
{
    return m_cityMap.size();
}

void Preferences::setDefaultCity()
{
    if (m_cityMap.size() > 0) {
        this->m_defaultId = m_cityMap.firstKey();
        this->m_currentId = this->m_defaultId;
        this->m_defaultCity = this->getCityNameById(this->m_defaultId);
    }
}

void Preferences::setCurrentCityIdByName(const QString &name)
{
    this->m_defaultCity = name;

    bool idOk = false;
    QMap<QString,QString>::iterator iter;
    for (iter = m_cityMap.begin(); iter != m_cityMap.end(); ++iter) {
        if (iter.value() == name) {
            this->m_defaultId = iter.key();
            this->m_currentId = this->m_defaultId;
            idOk = true;
            break;
        }
    }

    if (idOk) {
        qDebug() << "Set current city id success! id=" << this->m_defaultId << ",city=" << this->m_defaultCity;
    }
    else {
        qDebug() << "Set current city id success!, city=" << this->m_defaultCity;
    }
}

void Preferences::setCurrentCityNameById(const QString &id)
{
    this->m_defaultId = id;
    this->m_currentId = this->m_defaultId;

    QMap<QString,QString>::iterator iter;
    for (iter = m_cityMap.begin(); iter != m_cityMap.end(); ++iter) {
        if (iter.key() == id) {
            this->m_defaultCity = iter.value();
            break;
        }
    }
}

void Preferences::addCityInfoToPref(City city)
{
    if (m_cityMap.size() == 0) {
        m_cityMap.insert(city.id, city.name);
    }
    else {
        bool isExist = false;
        QMap<QString, QString>::const_iterator iter = m_cityMap.constBegin();
        while (iter != m_cityMap.constEnd()) {
            if (iter.key() == city.id) {
                isExist = true;
                break;
            }
            iter++;
        }
        if (!isExist) {
            m_cityMap.insert(city.id, city.name);
        }
    }
}

void Preferences::removeCityInfoFromPref(const QString &id, bool isActive)
{
//    bool found = false;
//    for (QString cityId : m_cityMap.keys()) {
//        if (cityId == id) {
//            found = true;
//            break;
//        }
//    }
//    if (found) {
//        m_cityMap.remove(id);
//    }

    if (m_cityMap.size() <= 1) {
        return;
    }

    if (m_cityMap.contains(id)) {
        QMap<QString, QString>::iterator iter;
        for (iter = m_cityMap.begin(); iter != m_cityMap.end();) {
            if (id == iter.key()) {
                m_cityMap.erase(iter++);//删除节点并指向下一个结点  iter = m_cityMap.erase(iter);
                break;
            }
            else {
                iter++;//指向下一个结点
            }
        }
    }

    if (isActive) {
        this->setDefaultCity();
    }

    /*QMap<QString, QString>::iterator iter2;
    for (iter2 = m_cityMap.begin(); iter2 != m_cityMap.end(); ++iter2) {
        qDebug() << "test data" << iter2.key() << iter2.value();
    }*/
}

bool Preferences::isCityIdExist(const QString &id)
{
    return m_cityMap.keys().contains(id);
}

bool Preferences::isCitiesCountOverMax()
{
    bool b = false;
    if (m_cityMap.size() >= 10) {
        b = true;
        return b;
    }

    return b;
}

QString Preferences::getNextId()
{
    if (m_cityMap.size() <= 1) {
        return QString();
    }

    QString nextId;
    int index = 0;
    QMap<QString, QString>::iterator it = m_cityMap.begin();
    for(; it != m_cityMap.end(); ++it) {
        if(it.key() == this->m_currentId) {
            break;
        }
        index++;
    }
    if (index == m_cityMap.size()-1) {
        nextId = m_cityMap.firstKey();
    }
    else {
        nextId = m_cityMap.keys().at(index+1);
    }


    /*for (auto it(m_cityMap.begin()); it != m_cityMap.end();) {
        if (this->m_currentId == it.key()) {
            it++;//指向下一个结点
            nextId = it.key();
            break;
        }
        else {
            it++;//指向下一个结点
        }
    }*/

    qDebug() << "org m_currentId:" << this->m_currentId;
    qDebug() << "nextId:" << nextId;
    this->m_currentId = nextId;
    qDebug() << "new m_currentId:" << this->m_currentId;
//    QList<QString> idList = m_cityMap.keys();
//    foreach (QString id, idList) {

//    }

    return nextId;
}

QString Preferences::getPrevId()
{
    if (m_cityMap.size() <= 1) {
        return QString();
    }

    QString prevId;
    int index = 0;
    QMap<QString, QString>::iterator it = m_cityMap.begin();
    for(; it != m_cityMap.end(); ++it) {
        if(it.key() == this->m_currentId) {
            break;
        }
        index++;
    }
    if (index == 0) {
        prevId = m_cityMap.lastKey();
    }
    else {
        prevId = m_cityMap.keys().at(index-1);
    }

//    for (auto it(m_cityMap.begin()); it != m_cityMap.end();) {
//        if (this->m_currentId == it.key()) {
//            break;
//        }
//        it++;//指向上一个结点
//        index++;
//    }

    qDebug() << "org m_currentId:" << this->m_currentId;
    qDebug() << "prevId:" << prevId;
    this->m_currentId = prevId;
    qDebug() << "new m_currentId:" << this->m_currentId;
    return prevId;
}

void Preferences::clearAirData()
{
    m_air.lat.clear();
    m_air.lon.clear();
    m_air.tz.clear();
    m_air.updateTime.clear();
    m_air.pubTime.clear();
    m_air.aqi.clear();
    m_air.qlty.clear();
    m_air.main.clear();
    m_air.pm25.clear();
    m_air.pm10.clear();
    m_air.no2.clear();
    m_air.so2.clear();
    m_air.co.clear();
    m_air.o3.clear();
    m_air.districtAirs.clear();
}

void Preferences::clearWeatherData()
{
    m_observeWeather.cloud.clear();
    m_observeWeather.cond_code.clear();
    m_observeWeather.cond_txt.clear();
    m_observeWeather.fl.clear();
    m_observeWeather.hum.clear();
    m_observeWeather.pcpn.clear();
    m_observeWeather.pres.clear();
    m_observeWeather.tmp.clear();
    m_observeWeather.vis.clear();
    m_observeWeather.wind_deg.clear();
    m_observeWeather.wind_dir.clear();
    m_observeWeather.wind_sc.clear();
    m_observeWeather.wind_spd.clear();


    m_forecasts.clear();
    m_hourlyWeather.clear();


    m_lifestyle.air_brf.clear();
    m_lifestyle.air_txt.clear();

    m_lifestyle.comf_brf.clear();
    m_lifestyle.comf_txt.clear();
    m_lifestyle.cw_brf.clear();
    m_lifestyle.cw_txt.clear();
    m_lifestyle.drsg_brf.clear();
    m_lifestyle.drsg_txt.clear();
    m_lifestyle.flu_brf.clear();
    m_lifestyle.flu_txt.clear();
    m_lifestyle.sport_brf.clear();
    m_lifestyle.sport_txt.clear();
    m_lifestyle.trav_brf.clear();
    m_lifestyle.trav_txt.clear();
    m_lifestyle.uv_brf.clear();
    m_lifestyle.uv_txt.clear();
}
