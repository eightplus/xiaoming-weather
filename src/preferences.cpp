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

    load();
}

Preferences::~Preferences()
{
    save();
    m_cityMap.clear();
}

void Preferences::setDefaultValue()
{
    m_currentCityId = "CN101250101";
    m_currentCity = "长沙";

    City city;
    city.id = m_currentCityId;
    city.name = m_currentCity;
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
    set->setValue("current_city_id", m_currentCityId);
    set->setValue("current_city", m_currentCity);

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
    m_currentCityId = set->value("current_city_id", m_currentCityId).toString();
    if (m_currentCityId.isEmpty()) {
        this->setDefaultValue();
    }
    else {
        //TODO: 可能会出现id和cityname不匹配的情况
        m_currentCity = set->value("current_city", m_currentCity).toString();

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
            m_cityMap.insert(m_currentCityId, m_currentCity);
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
        this->m_currentCityId = m_cityMap.firstKey();
        this->m_currentCity = this->getCityNameById(this->m_currentCityId);
    }
}

void Preferences::setCurrentCityIdByName(const QString &name)
{
    this->m_currentCity = name;

    bool idOk = false;
    QMap<QString,QString>::iterator iter;
    for (iter = m_cityMap.begin(); iter != m_cityMap.end(); ++iter) {
        if (iter.value() == name) {
            this->m_currentCityId = iter.key();
            idOk = true;
            break;
        }
    }

    if (idOk) {
        qDebug() << "Set current city id success! id=" << this->m_currentCityId << ",city=" << this->m_currentCity;
    }
    else {
        qDebug() << "Set current city id success!, city=" << this->m_currentCity;
    }
}

void Preferences::setCurrentCityNameById(const QString &id)
{
    this->m_currentCityId = id;

    QMap<QString,QString>::iterator iter;
    for (iter = m_cityMap.begin(); iter != m_cityMap.end(); ++iter) {
        if (iter.key() == id) {
            this->m_currentCity = iter.key();
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
                m_cityMap.erase(iter++);//删除节点并指向下一个结点
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
