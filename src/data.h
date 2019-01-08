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

#ifndef DATA_H
#define DATA_H

#include <QObject>

struct City
{
    QString id;
    QString name;
    double latitude;
    double longitude;
};

struct Forecast
{
    QString date;
    QString high;
    QString low;
    QString aqi;
    QString type;
};

struct LifeStyle {
    QString air_brf;
    QString air_txt;
    QString comf_brf;
    QString comf_txt;
    QString cw_brf;
    QString cw_txt;
    QString drsg_brf;
    QString drsg_txt;
    QString flu_brf;
    QString flu_txt;
    QString sport_brf;
    QString sport_txt;
    QString trav_brf;
    QString trav_txt;
    QString uv_brf;
    QString uv_txt;
};

struct ForecastWeather
{
    QString id;
    QString city;
    QString cond_code_d;//白天天气状况代码 	100
    QString cond_txt_d;//白天天气状况描述 	晴
    QString forcast_date;//预报日期 	2013-12-30
    QString hum;//相对湿度 	37
    QString tmp_max;//最高温度 	4
    QString tmp_min;//最低温度 	-5
};

class CitySettingData
{
public:
    bool active;

    QString id;
    QString name;
    QString temperature;
    QString icon;

public:
    void setActive(bool b) { active = b; }
};

inline bool operator ==(const City &data, const City &other) {
    return data.id == other.id;
}

inline bool operator ==(const CitySettingData &data, const CitySettingData &other) {
    return data.id == other.id;
}

Q_DECLARE_METATYPE(City)
Q_DECLARE_METATYPE(LifeStyle)
Q_DECLARE_METATYPE(ForecastWeather)
Q_DECLARE_METATYPE(CitySettingData)

#endif // DATA_H
