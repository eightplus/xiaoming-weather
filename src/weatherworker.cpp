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
    , m_networkManager(new QNetworkAccessManager(this))
{
//    connect(m_networkManager, &QNetworkAccessManager::finished, this, [] (QNetworkReply *reply) {
//        reply->deleteLater();
//    });

    //TODO: test geo api
//    this->setCity("Langli");

    m_automatic = new AutomaticLocation(this);
    connect(m_automatic, &AutomaticLocation::do_finish, this, &WeatherWorker::setAutoCity);
    m_automatic->start();

    //this->requestAccessGeoNameIDByLongitudeAndLatitude(28.1792, 113.114);//QString::arg: Argument missing: 无法解析res_nclose中的符号“res_nclose”：libresolv.so.2, (/lib/x86_64-linux-gnu/libresolv.so.2: undefined symbol: res_nclose)


    //    this->requestAccessGeoNameDataByGeonameId("1804526");
}

WeatherWorker::~WeatherWorker()
{
    m_networkManager->deleteLater();
}

void WeatherWorker::setAutoCity(const QString &cityName)
{
    if (cityName.isEmpty()) {//TODO
        //this->requestWeatherDataById(m_preferences->m_currentCityId);
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
                qDebug() << "id=" << id;//id.remove(0, 2);//remove "CN"
                qDebug() << "city=" << resultList.at(2);
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

    //TODO:
//    this->requestWeatherDataById(m_preferences->m_currentCityId);
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

void WeatherWorker::requestWeatherDataById(const QString &cityId)
{
    QString url = QString("https://free-api.heweather.com/s6/weather?location=%1&key=4ff2e595e593439380e52d2519523d0a").arg(cityId);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WeatherWorker::onWeatherDataReply);
}

void WeatherWorker::onWeatherDataReply()
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
    //qDebug() << "weather size: " << ba.size();

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

    QJsonObject jsonObject = jsonDocument.object();
    qDebug() << "jsonObject" << jsonObject;

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
