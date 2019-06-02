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

#include "locationworker.h"

#include <QDebug>
#include <QFile>

WorkerThread::WorkerThread(LocationWorker *parent) :
    QThread(parent)
{

}

void WorkerThread::run()
{
    LocationWorker *m_worker = qobject_cast<LocationWorker*>(parent());
    if (!m_worker)
        return;

    /*QFile file(":/data/data/china-city-list.csv");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString content = file.readAll();
        QStringList resultList = content.split("\n");
        qDebug() << resultList.length();
        file.close();
    }*/

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

            LocationData data;
            data.id = id.remove(0, 2);//remove "CN"
            data.city_en = resultList.at(1);
            data.city = resultList.at(2);
            data.country_en = resultList.at(4);
            data.country = resultList.at(5);
            data.province_en = resultList.at(6);
            data.province = resultList.at(7);
            data.admin_district_en = resultList.at(8);
            data.admin_district = resultList.at(9);

            m_worker->m_locatonList << data;

            line = file.readLine();
            line = line.replace("\n", "");
        }
        file.close();
    }
}

//--------------------------------------------------------------
LocationWorker::LocationWorker(QObject *parent)
    : QObject(parent),
      m_workerThread(new WorkerThread(this))
{
    m_workerThread->start();
}

LocationWorker::~LocationWorker()
{
    m_workerThread->deleteLater();
}

QList<LocationData> LocationWorker::exactMatchCity(const QString &inputText) const
{
    QList<LocationData> searchedList;

    for (const LocationData line : m_locatonList) {
        if (line.id == inputText || line.city.contains(inputText) || line.city_en.contains(inputText) || line.province == inputText || line.province_en == inputText || line.admin_district.contains(inputText) || line.admin_district_en.contains(inputText)) {
            searchedList.append(line);
        }
    }

    return searchedList;
}
