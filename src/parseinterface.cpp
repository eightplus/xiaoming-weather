/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
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

#include "parseinterface.h"

ParseInterface::ParseInterface(QObject *parent)
    :QObject(parent)
{
    initData();
}

ParseInterface::~ParseInterface()
{

}

ParseInterface *ParseInterface::getInstance(void)
{
    static ParseInterface m_instance;
    return &m_instance;
}

void ParseInterface::initData()
{
    m_weatherIconMap.clear();

    m_weatherIconMap.insert("d0.gif", "weather-clear");
    m_weatherIconMap.insert("d1.gif", "weather-few-clouds");
    m_weatherIconMap.insert("d2.gif", "weather-overcast");
    m_weatherIconMap.insert("d3.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("d4.gif", "weather-storm");
    m_weatherIconMap.insert("d5.gif", "weather-storm");
    m_weatherIconMap.insert("d6.gif", "weather-snow");
    m_weatherIconMap.insert("d7.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("d8.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("d9.gif", "weather-showers");
    m_weatherIconMap.insert("d10.gif", "weather-showers");
    m_weatherIconMap.insert("d11.gif", "weather-showers");
    m_weatherIconMap.insert("d12.gif", "weather-severe-alert");
    m_weatherIconMap.insert("d13.gif", "weather-snow");
    m_weatherIconMap.insert("d14.gif", "weather-snow");
    m_weatherIconMap.insert("d15.gif", "weather-snow");
    m_weatherIconMap.insert("d16.gif", "weather-snow");
    m_weatherIconMap.insert("d17.gif", "weather-snow");
    m_weatherIconMap.insert("d18.gif", "weather-fog");
    m_weatherIconMap.insert("d19.gif", "weather-snow");
    m_weatherIconMap.insert("d20.gif", "weather-fog");
    m_weatherIconMap.insert("d21.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("d22.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("d23.gif", "weather-showers");
    m_weatherIconMap.insert("d24.gif", "weather-showers");
    m_weatherIconMap.insert("d25.gif", "weather-showers");
    m_weatherIconMap.insert("d26.gif", "weather-snow");
    m_weatherIconMap.insert("d27.gif", "weather-snow");
    m_weatherIconMap.insert("d28.gif", "weather-snow");
    m_weatherIconMap.insert("d29.gif", "weather-fog");
    m_weatherIconMap.insert("d30.gif", "weather-fog");
    m_weatherIconMap.insert("d31.gif", "weather-fog");
    m_weatherIconMap.insert("d53.gif", "weather-fog");
    m_weatherIconMap.insert("n0.gif", "weather-clear-night");
    m_weatherIconMap.insert("n1.gif", "weather-few-clouds-night");
    m_weatherIconMap.insert("n2.gif", "weather-overcast");
    m_weatherIconMap.insert("n3.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("n4.gif", "weather-storm");
    m_weatherIconMap.insert("n5.gif", "weather-storm");
    m_weatherIconMap.insert("n6.gif", "weather-snow");
    m_weatherIconMap.insert("n7.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("n8.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("n9.gif", "weather-showers");
    m_weatherIconMap.insert("n10.gif", "weather-showers");
    m_weatherIconMap.insert("n11.gif", "weather-showers");
    m_weatherIconMap.insert("n12.gif", "weather-severe-alert");
    m_weatherIconMap.insert("n13.gif", "weather-snow");
    m_weatherIconMap.insert("n14.gif", "weather-snow");
    m_weatherIconMap.insert("n15.gif", "weather-snow");
    m_weatherIconMap.insert("n16.gif", "weather-snow");
    m_weatherIconMap.insert("n17.gif", "weather-snow");
    m_weatherIconMap.insert("n18.gif", "weather-fog");
    m_weatherIconMap.insert("n19.gif", "weather-snow");
    m_weatherIconMap.insert("n20.gif", "weather-fog");
    m_weatherIconMap.insert("n21.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("n22.gif", "weather-showers-scattered");
    m_weatherIconMap.insert("n23.gif", "weather-showers");
    m_weatherIconMap.insert("n24.gif", "weather-showers");
    m_weatherIconMap.insert("n25.gif", "weather-showers");
    m_weatherIconMap.insert("n26.gif", "weather-snow");
    m_weatherIconMap.insert("n27.gif", "weather-snow");
    m_weatherIconMap.insert("n28.gif", "weather-snow");
    m_weatherIconMap.insert("n29.gif", "weather-fog");
    m_weatherIconMap.insert("n30.gif", "weather-fog");
    m_weatherIconMap.insert("n31.gif", "weather-fog");
    m_weatherIconMap.insert("n53.gif", "weather-fog");
}
