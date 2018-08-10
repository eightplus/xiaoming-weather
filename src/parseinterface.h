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

#ifndef PARSE_INTERFACE_H
#define PARSE_INTERFACE_H

#include <QObject>
#include <QMap>

#define global_ParseInterface ParseInterface::getInstance()

class ParseInterface : public QObject
{
    Q_OBJECT

public:
    static ParseInterface *getInstance(void);

    QMap<QString, QString> getweatherIconMap() { return m_weatherIconMap; }

private:
    ParseInterface(QObject *parent = nullptr);
    ~ParseInterface();

    void initData();

    QMap<QString, QString> m_weatherIconMap;
};

#endif // PARSE_INTERFACE_H
