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

#include "weatherworker.h"
#include "weatherairanalysiser.h"
#include "weathernowanalysiser.h"
#include "weatherforecastanalysiser.h"
#include "weatherlifestyleanalysiser.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QEventLoop>
#include <QFile>
#include <QApplication>
#include <QDomDocument>
#include <QLocale>

#include "automaticlocation.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

//TODO: m_networkManager->get cause the warning:     QString::arg: Argument missing: 无法解析res_nclose中的符号“res_nclose”：libresolv.so.2, (/lib/x86_64-linux-gnu/libresolv.so.2: undefined symbol: res_nclose)

WeatherWorker::WeatherWorker(QObject *parent) :
    QObject(parent)
//    , m_networkManager(new QNetworkAccessManager(this))
{
    m_forecastList.clear();
    m_weatherMap.clear();
    /*m_weatherMap.insert(W_Now_Type, new WeatherNowAnalysiser);
    m_weatherMap.insert(W_Forecast_Type, new WeatherForecastAnalysiser);
    m_weatherMap.insert(W_LifeStyle_Type, new WeatherLifestyleAnalysiser);
    m_weatherMap.insert(W_Air_Type, new WeatherAirAnalysiser);*/

    m_networkManager = new QNetworkAccessManager(this);
//    connect(m_networkManager, &QNetworkAccessManager::finished, this, [] (QNetworkReply *reply) {
//        reply->deleteLater();
//    });

    //TODO: test geo api
//    this->setCity("Langli");

    m_automatic = new AutomaticLocation(this);
    connect(m_automatic, &AutomaticLocation::do_finish, this, &WeatherWorker::setAutoCity);
    m_automatic->start();

    //this->requestAccessGeoNameIDByLongitudeAndLatitude(28.1792, 113.114);//QString::arg: Argument missing: 无法解析res_nclose中的符号“res_nclose”：libresolv.so.2, (/lib/x86_64-linux-gnu/libresolv.so.2: undefined symbol: res_nclose)
    //this->requestAccessGeoNameDataByGeonameId("1804526");
}

WeatherWorker *WeatherWorker::getInstance(void)
{
    static WeatherWorker instance;
    return &instance;
}

WeatherWorker::~WeatherWorker()
{
    m_networkManager->deleteLater();
}

QList<ForecastWeather> WeatherWorker::getForecastList()
{
    return m_forecastList;
}

void WeatherWorker::setAutoCity(const QString &cityName)
{
    if (cityName.isEmpty()) {//TODO
        //this->requestWeatherAndApiDataById(m_preferences->m_currentCityId);
        return;
    }
    //CN101250101,changsha,长沙,CN,China,中国,hunan,湖南,changsha,长沙,28.19409,112.98228,"430101,430100,430000",
    QFile file(":/data/data/china-city-list.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line = file.readLine();
        line = line.replace("\n", "");
        while (!line.isEmpty()) {
            QStringList resultList = line.split(",");
            if (resultList.length() < 10) {
                line = file.readLine();
                line = line.replace("\n", "");
                continue;
            }

            QString id = resultList.at(0);
            if (!id.startsWith("CN")) {
                line = file.readLine();
                line = line.replace("\n", "");
                continue;
            }

            if (resultList.at(1).compare(cityName, Qt::CaseInsensitive) == 0) {
                //qDebug() << "id=" << id;//id.remove(0, 2);//remove "CN"
                //qDebug() << "city=" << resultList.at(2);
                City city;
                city.id = id;
                city.name = resultList.at(2);
                m_preferences->addCityInfoToPref(city);
                m_preferences->setCurrentCityNameById(id);
                m_preferences->save();
                break;
            }

            line = file.readLine();
            line = line.replace("\n", "");
        }
        file.close();
    }

    this->requestWeatherAndApiDataById(m_preferences->m_currentCityId);
}

void WeatherWorker::setCity(const QString &city)
{
    m_city = city;
}

void WeatherWorker::requestSearchCityByInput(const QString &input)
{
    const QString current_lang = QLocale::system().name().split("_").at(0);
    //examples: http://api.geonames.org/search?q=changsha&maxRows=10&username=kobe&lang=zh
    QString url = QString("http://api.geonames.org/search?q=%1&maxRows=10&username=kobe&lang=%2").arg(input).arg(current_lang);
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url));
    //    QNetworkRequest request;
//    request.setUrl(url);
//    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onSearchCityReply);
}

void WeatherWorker::requestAccessGeoNameDataByGeonameId(const QString &geonameId)
{
    //examples: http://api.geonames.org/get?geonameId=1804526&username=kobe&lang=zh
    const QString current_lang = QLocale::system().name().split("_").at(0);
    QString url = QString("http://api.geonames.org/get?geonameId=%1&username=kobe&lang=%2").arg(geonameId).arg(current_lang);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onGeoNameDataByGeonameIdReply);
}

void WeatherWorker::requestAccessGeoNameIDByLongitudeAndLatitude(double latitude, double longitude)
{
    //examples: http://api.geonames.org/extendedFindNearby?lat=28.1792&lng=113.114&username=kobe
    QString url = QString("http://api.geonames.org/extendedFindNearby?lat=%1&lng=%2&username=kobe").arg(latitude).arg(longitude);
//    QNetworkRequest request;
//    request.setUrl(url);
//    QNetworkReply *reply = m_networkManager->get(request);
    QNetworkReply *reply = m_networkManager->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onGeoNameIdByLongitudeAndLatitudeReply);
}

void WeatherWorker::requestWeatherDataByLongitudeAndLatitude(double latitude, double longitude)
{
    QString url = QString("https://free-api.heweather.com/v5/weather?city=%1,%2&key=4ff2e595e593439380e52d2519523d0a").arg(latitude).arg(longitude);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply);
}

void WeatherWorker::requestWeatherAndApiDataById(const QString &cityId)
{
    /*
     * https://free-api.heweather.com/s6/weather?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a
     * https://free-api.heweather.net/s6/air/now?location=CN101250101&key=4ff2e595e593439380e52d2519523d0a
     */

    // 获取实时天气 + 3天天气预报 + 生活指数
    QString url = QString("https://free-api.heweather.com/s6/weather?location=%1&key=4ff2e595e593439380e52d2519523d0a").arg(cityId);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply);

    // 获取空气质量数据
    url = QString("https://free-api.heweather.net/s6/air/now?location=%1&key=4ff2e595e593439380e52d2519523d0a").arg(cityId);
    request.setUrl(url);
    //QEventLoop loop;
    QNetworkReply *apiReply = m_networkManager->get(request);
    connect(apiReply, &QNetworkReply::finished, this, &WeatherWorker::onApiDataReply);
    //connect(apiReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    //loop.exec();
}

// 解析实时天气 + 3天天气预报 + 生活指数
void WeatherWorker::onWeatherDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//QVariant to int

    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        //qDebug() << "weather request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            reply->close();
            reply->deleteLater();
        }

        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
    //qDebug() << "weather size: " << ba.size();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (jsonDocument.isNull() || !jsonDocument.isObject() || err.error != QJsonParseError::NoError) {// Json type error
        qDebug() << "Json type error or null";
        return;
    }


//    m_weatherType = W_Air_Type;
//    m_weatherMap[m_weatherType]->parseJson(mainDataJsonObject);

    QJsonArray mainDataJsonArray;
    QJsonObject jsonObject = jsonDocument.object();
    //qDebug() << "jsonObject" << jsonObject;
    if (jsonObject.contains("HeWeather6")) {
        QJsonValue jsonValue = jsonObject.value("HeWeather6");
        if (jsonValue.isArray()) {
            QString m_status;
            mainDataJsonArray = jsonValue.toArray();
            QJsonObject mainDataJsonObject = mainDataJsonArray.at(0).toObject();

            // basic
            if (mainDataJsonObject.contains("basic")) {
                QJsonObject basicJsonObject = mainDataJsonObject.value("basic").toObject();
                m_preferences->m_currentCityId = basicJsonObject.value("cid").toString();
                m_preferences->m_currentCity = basicJsonObject.value("location").toString();
//                qDebug() << "cid=" << basicJsonObject.value("cid").toString();
//                qDebug() << "location=" << basicJsonObject.value("location").toString();
//                qDebug() << "parent_city=" << basicJsonObject.value("parent_city").toString();
//                qDebug() << "admin_area=" << basicJsonObject.value("admin_area").toString();
//                qDebug() << "cnty=" << basicJsonObject.value("cnty").toString();
//                qDebug() << "lat=" << basicJsonObject.value("lat").toString().toDouble();
//                qDebug() << "lon=" << basicJsonObject.value("lon").toString().toDouble();
//                qDebug() << "tz=" << basicJsonObject.value("tz").toString();
            }

            // update time
            if (mainDataJsonObject.contains("update")) {
                QJsonObject updateJsonObject = mainDataJsonObject.value("update").toObject();
                m_preferences->m_updateTime = updateJsonObject.value("loc").toString();
//                qDebug() << "loc =" << updateJsonObject.value("loc").toString();//QDateTime::fromString()
//                qDebug() << "utc =" << updateJsonObject.value("utc").toString();
            }

            // status
            if (mainDataJsonObject.contains("status")) {
                m_status = mainDataJsonObject.value("status").toString();
//                qDebug() << "status=" << mainDataJsonObject.value("status").toString();
            }

            // now
            if (mainDataJsonObject.contains("now")) {
                QJsonObject nowJsonObject = mainDataJsonObject.value("now").toObject();
//                qDebug() << "cloud=" << nowJsonObject.value("cloud").toString();
//                qDebug() << "cond_code=" << nowJsonObject.value("cond_code").toString();
//                qDebug() << "cond_txt=" << nowJsonObject.value("cond_txt").toString();
//                qDebug() << "fl=" << nowJsonObject.value("fl").toString();
//                qDebug() << "hum=" << nowJsonObject.value("hum").toString();
//                qDebug() << "pcpn=" << nowJsonObject.value("pcpn").toString();
//                qDebug() << "pres=" << nowJsonObject.value("pres").toString();
//                qDebug() << "tmp=" << nowJsonObject.value("tmp").toString();
//                qDebug() << "vis=" << nowJsonObject.value("vis").toString();
//                qDebug() << "wind_deg=" << nowJsonObject.value("wind_deg").toString();
//                qDebug() << "wind_dir=" << nowJsonObject.value("wind_dir").toString();
//                qDebug() << "wind_sc=" << nowJsonObject.value("wind_sc").toString();
//                qDebug() << "wind_spd=" << nowJsonObject.value("wind_spd").toString();

                m_preferences->m_observeWeather.cloud = nowJsonObject.value("cloud").toString();
                m_preferences->m_observeWeather.cond_code = nowJsonObject.value("cond_code").toString();
                m_preferences->m_observeWeather.cond_txt = nowJsonObject.value("cond_txt").toString();
                m_preferences->m_observeWeather.fl = nowJsonObject.value("fl").toString();
                m_preferences->m_observeWeather.hum = nowJsonObject.value("hum").toString();
                m_preferences->m_observeWeather.pcpn = nowJsonObject.value("pcpn").toString();
                m_preferences->m_observeWeather.pres = nowJsonObject.value("pres").toString();
                m_preferences->m_observeWeather.tmp = nowJsonObject.value("tmp").toString();
                m_preferences->m_observeWeather.vis = nowJsonObject.value("vis").toString();
                m_preferences->m_observeWeather.wind_deg = nowJsonObject.value("wind_deg").toString();
                m_preferences->m_observeWeather.wind_dir = nowJsonObject.value("wind_dir").toString();
                m_preferences->m_observeWeather.wind_sc = nowJsonObject.value("wind_sc").toString();
                m_preferences->m_observeWeather.wind_spd = nowJsonObject.value("wind_spd").toString();
            }

            // daily_forecast
            if (mainDataJsonObject.contains("daily_forecast")) {
                QJsonArray dailyArray = mainDataJsonObject.value("daily_forecast").toArray();
                foreach (QJsonValue val, dailyArray) {
                    QJsonObject dailyJsonObject = val.toObject();

//                    qDebug() << "Daily cond_code_d=" << dailyJsonObject.value("cond_code_d").toString();
//                    qDebug() << "Daily cond_code_n=" << dailyJsonObject.value("cond_code_n").toString();
//                    qDebug() << "Daily cond_txt_d=" << dailyJsonObject.value("cond_txt_d").toString();
//                    qDebug() << "Daily cond_txt_n=" << dailyJsonObject.value("cond_txt_n").toString();
//                    qDebug() << "Daily date=" << dailyJsonObject.value("date").toString();
//                    qDebug() << "Daily hum=" << dailyJsonObject.value("hum").toString();
//                    qDebug() << "Daily mr=" << dailyJsonObject.value("mr").toString();//QTime::fromString(dailyJsonObject.value("mr").toString(), "hh:mm")
//                    qDebug() << "Daily ms=" << dailyJsonObject.value("ms").toString();
//                    qDebug() << "Daily pcpn=" << dailyJsonObject.value("pcpn").toString();
//                    qDebug() << "Daily pop=" << dailyJsonObject.value("pop").toString();
//                    qDebug() << "Daily pres=" << dailyJsonObject.value("pres").toString();
//                    qDebug() << "Daily sr=" << dailyJsonObject.value("sr").toString();
//                    qDebug() << "Daily ss=" << dailyJsonObject.value("ss").toString();

//                    qDebug() << "Daily tmp_max=" << dailyJsonObject.value("tmp_max").toString();
//                    qDebug() << "Daily tmp_min=" << dailyJsonObject.value("tmp_min").toString();
//                    qDebug() << "Daily uv_index=" << dailyJsonObject.value("uv_index").toString();
//                    qDebug() << "Daily vis=" << dailyJsonObject.value("vis").toString();
//                    qDebug() << "Daily wind_deg=" << dailyJsonObject.value("wind_deg").toString();
//                    qDebug() << "Daily wind_dir=" << dailyJsonObject.value("wind_dir").toString();
//                    qDebug() << "Daily wind_sc=" << dailyJsonObject.value("wind_sc").toString();
//                    qDebug() << "Daily wind_spd=" << dailyJsonObject.value("wind_spd").toString();

                    ForecastWeather forecast;
                    forecast.cond_code_d = dailyJsonObject.value("cond_code_d").toString();
                    forecast.cond_code_n = dailyJsonObject.value("cond_code_n").toString();
                    forecast.cond_txt_d = dailyJsonObject.value("cond_txt_d").toString();
                    forecast.cond_txt_n = dailyJsonObject.value("cond_txt_n").toString();
                    forecast.forcast_date = dailyJsonObject.value("date").toString();
                    forecast.hum = dailyJsonObject.value("hum").toString();
                    forecast.mr_ms = dailyJsonObject.value("mr").toString() + "+" + dailyJsonObject.value("ms").toString();
                    forecast.pcpn = dailyJsonObject.value("pcpn").toString();
                    forecast.pop = dailyJsonObject.value("pop").toString();
                    forecast.pres = dailyJsonObject.value("pres").toString();
                    forecast.sr_ss = dailyJsonObject.value("sr").toString() + "+" + dailyJsonObject.value("ss").toString();
                    forecast.tmp_max = dailyJsonObject.value("tmp_max").toString();
                    forecast.tmp_min = dailyJsonObject.value("tmp_min").toString();
                    forecast.uv_index = dailyJsonObject.value("uv_index").toString();
                    forecast.vis = dailyJsonObject.value("vis").toString();
                    forecast.wind_deg = dailyJsonObject.value("wind_deg").toString();
                    forecast.wind_dir = dailyJsonObject.value("wind_dir").toString();
                    forecast.wind_sc = dailyJsonObject.value("wind_sc").toString();
                    forecast.wind_spd = dailyJsonObject.value("wind_spd").toString();
                    //m_forecastList.push_back(forecast);
                    m_preferences->m_forecasts.push_back(forecast);
//                    for (int i=0;i<m_preferences->m_forecasts.size();i++) {
//                        qDebug() << "000 " << m_preferences->m_forecasts.at(i).forcast_date;
//                    }
//                    qDebug() << "111a" << forecast.forcast_date << m_preferences->m_forecasts.size();
                }
            }

            //lifestyle
            if (mainDataJsonObject.contains("lifestyle")) {
                QJsonArray lifestyleArray = mainDataJsonObject.value("lifestyle").toArray();
                foreach (QJsonValue val, lifestyleArray) {//lifestyleArray.isArray()
                    QJsonObject lifestyleObject = val.toObject();
                    if (lifestyleObject.value("type").toString() == "comf") {
                        m_preferences->m_lifestyle.comf_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.comf_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "drsg") {
                        m_preferences->m_lifestyle.drsg_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.drsg_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "flu") {
                        m_preferences->m_lifestyle.flu_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.flu_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "sport") {
                        m_preferences->m_lifestyle.sport_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.sport_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "trav") {
                        m_preferences->m_lifestyle.trav_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.trav_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "uv") {
                        m_preferences->m_lifestyle.uv_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.uv_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "cw") {
                        m_preferences->m_lifestyle.cw_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.cw_txt = lifestyleObject.value("txt").toString();
                    }
                    else if (lifestyleObject.value("type").toString() == "air") {
                        m_preferences->m_lifestyle.air_brf = lifestyleObject.value("brf").toString();
                        m_preferences->m_lifestyle.air_txt = lifestyleObject.value("txt").toString();
                    }
//                    qDebug() << "type=" << lifestyleObject.value("type").toString();//comf drsg flu sport trav uv cw  air
//                    qDebug() << "brf=" << lifestyleObject.value("brf").toString();
//                    qDebug() << "txt=" << lifestyleObject.value("txt").toString();
                }
                //QJsonObject comfObject = lifestyleArray.at(0).toObject();
                //QJsonObject drsgObject = lifestyleArray.at(1).toObject();
            }

            if (m_status == "ok") {
                emit this->readyUpdateWeather();
            }
        }
    }
}

// 解析空气质量数据
void WeatherWorker::onApiDataReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        //qDebug() << "weather request error:" << reply->error() << ", statusCode=" << statusCode;
        if (statusCode == 301 || statusCode == 302) {//redirect
            reply->close();
            reply->deleteLater();
        }

        return;
    }

    QByteArray ba = reply->readAll();
    //QString reply_content = QString::fromUtf8(ba);
    reply->close();
    reply->deleteLater();
//    qDebug() << "api size: " << ba.size();

    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::NoError) {// Json type error
        qDebug() << "Json type error";
        return;
    }
    if (jsonDocument.isNull() || jsonDocument.isEmpty()) {
        qDebug() << "Json null or empty!";
        return;
    }

    QJsonArray mainDataJsonArray;
    QJsonObject jsonObject = jsonDocument.object();
//    qDebug() << "jsonObject" << jsonObject;
    if (jsonObject.contains("HeWeather6")) {
        QJsonValue jsonValue = jsonObject.value("HeWeather6");
        if (jsonValue.isArray()) {
            QString m_status;
            mainDataJsonArray = jsonValue.toArray();
            QJsonObject mainDataJsonObject = mainDataJsonArray.at(0).toObject();

            // basic
            if (mainDataJsonObject.contains("basic")) {
                QJsonObject basicJsonObject = mainDataJsonObject.value("basic").toObject();
                m_preferences->m_air.lat = basicJsonObject.value("lat").toString();
                m_preferences->m_air.lon = basicJsonObject.value("lon").toString();
                m_preferences->m_air.tz = basicJsonObject.value("tz").toString();
//                qDebug() << "cid=" << basicJsonObject.value("cid").toString();
//                qDebug() << "location=" << basicJsonObject.value("location").toString();
//                qDebug() << "parent_city=" << basicJsonObject.value("parent_city").toString();
//                qDebug() << "admin_area=" << basicJsonObject.value("admin_area").toString();
//                qDebug() << "cnty=" << basicJsonObject.value("cnty").toString();
//                qDebug() << "lat=" << basicJsonObject.value("lat").toString().toDouble();
//                qDebug() << "lon=" << basicJsonObject.value("lon").toString().toDouble();
//                qDebug() << "tz=" << basicJsonObject.value("tz").toString();
            }

            // update time
            if (mainDataJsonObject.contains("update")) {
                QJsonObject updateJsonObject = mainDataJsonObject.value("update").toObject();
                m_preferences->m_air.updateTime = updateJsonObject.value("loc").toString();
//                qDebug() << "loc =" << updateJsonObject.value("loc").toString();
//                qDebug() << "utc =" << updateJsonObject.value("utc").toString();
            }

            // status
            if (mainDataJsonObject.contains("status")) {
                m_status = mainDataJsonObject.value("status").toString();
//                qDebug() << "air aqi status=" << mainDataJsonObject.value("status").toString();
            }

            // air_now_city
            if (mainDataJsonObject.contains("air_now_city")) {
                QJsonObject ariNowJsonObject = mainDataJsonObject.value("air_now_city").toObject();
                m_preferences->m_air.aqi = ariNowJsonObject.value("aqi").toString();
                m_preferences->m_air.qlty = ariNowJsonObject.value("qlty").toString();
                m_preferences->m_air.main = ariNowJsonObject.value("main").toString();
                m_preferences->m_air.pm25 = ariNowJsonObject.value("pm25").toString();
                m_preferences->m_air.pm10 = ariNowJsonObject.value("pm10").toString();
                m_preferences->m_air.no2 = ariNowJsonObject.value("no2").toString();
                m_preferences->m_air.so2 = ariNowJsonObject.value("so2").toString();
                m_preferences->m_air.co = ariNowJsonObject.value("co").toString();
                m_preferences->m_air.o3 = ariNowJsonObject.value("o3").toString();
                m_preferences->m_air.pubTime = ariNowJsonObject.value("pub_time").toString();

//                qDebug() << "aqi=" << ariNowJsonObject.value("aqi").toString();
//                qDebug() << "qlty=" << ariNowJsonObject.value("qlty").toString();
//                qDebug() << "main=" << ariNowJsonObject.value("main").toString();
//                qDebug() << "pm25=" << ariNowJsonObject.value("pm25").toString();
//                qDebug() << "pm10=" << ariNowJsonObject.value("pm10").toString();
//                qDebug() << "no2=" << ariNowJsonObject.value("no2").toString();
//                qDebug() << "so2=" << ariNowJsonObject.value("so2").toString();
//                qDebug() << "co=" << ariNowJsonObject.value("co").toString();
//                qDebug() << "o3=" << ariNowJsonObject.value("o3").toString();
//                qDebug() << "pub_time=" << ariNowJsonObject.value("pub_time").toString();
            }

            // air_now_station
            if (mainDataJsonObject.contains("air_now_station")) {
                QJsonArray airNowArray = mainDataJsonObject.value("air_now_station").toArray();
                foreach (QJsonValue val, airNowArray) {//airNowArray.isArray()
                    QJsonObject airNowObject = val.toObject();
                    DistrictAir districtAir;
                    districtAir.air_sta = airNowObject.value("air_sta").toString();
                    districtAir.aqi = airNowObject.value("aqi").toString();
                    districtAir.asid = airNowObject.value("asid").toString();
                    districtAir.co = airNowObject.value("co").toString();
                    districtAir.lat = airNowObject.value("lat").toString();
                    districtAir.lon = airNowObject.value("lon").toString();
                    districtAir.main = airNowObject.value("main").toString();
                    districtAir.no2 = airNowObject.value("no2").toString();
                    districtAir.o3 = airNowObject.value("o3").toString();
                    districtAir.pm10 = airNowObject.value("pm10").toString();
                    districtAir.pm25 = airNowObject.value("pm25").toString();
                    districtAir.pub_time = airNowObject.value("pub_time").toString();
                    districtAir.qlty = airNowObject.value("qlty").toString();
                    districtAir.so2 = airNowObject.value("so2").toString();
                    m_preferences->m_air.districtAirs.push_back(districtAir);

//                    qDebug() << "air_sta=" << airNowObject.value("air_sta").toString();
//                    qDebug() << "aqi=" << airNowObject.value("aqi").toString();
//                    qDebug() << "asid=" << airNowObject.value("asid").toString();
//                    qDebug() << "co=" << airNowObject.value("co").toString();
//                    qDebug() << "lat=" << airNowObject.value("lat").toString();
//                    qDebug() << "lon=" << airNowObject.value("lon").toString();
//                    qDebug() << "main=" << airNowObject.value("main").toString();
//                    qDebug() << "no2=" << airNowObject.value("no2").toString();
//                    qDebug() << "o3=" << airNowObject.value("o3").toString();
//                    qDebug() << "pm10=" << airNowObject.value("pm10").toString();
//                    qDebug() << "pm25=" << airNowObject.value("pm25").toString();
//                    qDebug() << "pub_time=" << airNowObject.value("pub_time").toString();
//                    qDebug() << "qlty=" << airNowObject.value("qlty").toString();
//                    qDebug() << "so2=" << airNowObject.value("so2").toString();
                }
            }
            if (m_status == "ok") {
                emit this->readyUpdateAqi();
            }
        }
    }
}

void WeatherWorker::onSearchCityReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        if (statusCode == 301 || statusCode == 302) {//redirect
            reply->close();
            reply->deleteLater();
        }

        return;
    }

    QByteArray ba = reply->readAll();

    QDomDocument domDocument;
    QString errorMsg;
    if (!domDocument.setContent(ba, false, &errorMsg)) {
        qDebug() << "read xml error: " << errorMsg;
    }

    QDomElement rootElement = domDocument.documentElement();
    QDomElement geonameElement = rootElement.firstChildElement("geoname");
    while (!geonameElement.isNull()) {
        QString name = geonameElement.firstChildElement("name").text();
        QString geonameId = geonameElement.firstChildElement("geonameId").text();
        QString countryName = geonameElement.firstChildElement("countryName").text();
        double lat = geonameElement.firstChildElement("lat").text().toDouble();
        double lng = geonameElement.firstChildElement("lng").text().toDouble();
        qDebug() << "name=" << name << ",geonameId=" << geonameId << ",countryName=" << countryName << ",lat=" << lat << ",lng=" << lng;
        geonameElement = geonameElement.nextSiblingElement("geoname");
    }
}

void WeatherWorker::onGeoNameIdByLongitudeAndLatitudeReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        if (statusCode == 301 || statusCode == 302) {//redirect
            reply->close();
            reply->deleteLater();
        }

        return;
    }

    QByteArray ba = reply->readAll();
    QDomDocument domDocument;
    QString errorMsg;
    if (!domDocument.setContent(ba, false, &errorMsg)) {
        qDebug() << "read xml error: " << errorMsg;
    }
    QDomElement rootElement = domDocument.documentElement();
    QDomElement geonameElement = rootElement.firstChildElement("geoname");
    while (!geonameElement.isNull()) {
        QString name = geonameElement.firstChildElement("name").text();
        if (name.toLower() == m_city.toLower()) {
            QString geonameId = geonameElement.firstChildElement("geonameId").text();
            this->requestAccessGeoNameDataByGeonameId(geonameId);
            break;
        }

        geonameElement = geonameElement.nextSiblingElement("geoname");
    }
}


void WeatherWorker::onGeoNameDataByGeonameIdReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(reply->error() != QNetworkReply::NoError || statusCode != 200) {//200 is normal status
        if (statusCode == 301 || statusCode == 302) {//redirect
            reply->close();
            reply->deleteLater();
        }

        return;
    }

    QByteArray ba = reply->readAll();

    QDomDocument domDocument;
    QString errorMsg;
    if (!domDocument.setContent(ba, false, &errorMsg)) {
        qDebug() << "read xml error: " << errorMsg;
    }

    QDomElement rootElement = domDocument.documentElement();
    QDomElement nameElement = rootElement.firstChildElement("name");
    if (!nameElement.text().isEmpty()) {
        qDebug() << "City=" << nameElement.text();
    }
}

QList<ForecastWeather> WeatherWorker::getForecastData()
{
    WeatherForecastAnalysiser *forecastAnalysiser = dynamic_cast<WeatherForecastAnalysiser*>(m_weatherMap[W_Forecast_Type]);
    if (forecastAnalysiser) {
        return forecastAnalysiser->getForecastData();
    }
    else {
        return QList<ForecastWeather>();
    }
}

ObserveWeather WeatherWorker::getObserveData()
{
    WeatherNowAnalysiser *observeAnalysiser = dynamic_cast<WeatherNowAnalysiser*>(m_weatherMap[W_Now_Type]);
    if (observeAnalysiser) {
        return observeAnalysiser->getObserveData();
    }
    else {
        return ObserveWeather();
    }
}

QList<LifeStyle> WeatherWorker::getLifestyleData()
{
    WeatherLifestyleAnalysiser *lifestyleAnalysiser = dynamic_cast<WeatherLifestyleAnalysiser*>(m_weatherMap[W_LifeStyle_Type]);
    if (lifestyleAnalysiser) {
        return lifestyleAnalysiser->getLifestyleData();
    }
    else {
        return QList<LifeStyle>();
    }
}

Air WeatherWorker::getAirData()
{
    WeatherAirAnalysiser *airAnalysiser = dynamic_cast<WeatherAirAnalysiser*>(m_weatherMap[W_Air_Type]);
    if (airAnalysiser) {
        return airAnalysiser->getAirData();
    }
    else {
        return Air();
    }
}
