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

#include "weatherlifestyleanalysiser.h"

WeatherLifestyleAnalysiser::WeatherLifestyleAnalysiser(QObject *parent)
    :WeatherBaseAnalysiser(parent)
{
    m_lifestyleData.clear();
}

WeatherLifestyleAnalysiser::~WeatherLifestyleAnalysiser()
{

}

void WeatherLifestyleAnalysiser::parseJson(QJsonObject &jsonObject)
{
    m_lifestyleData.clear();
//    LifeStyle lifeStyle;
//    m_lifestyleData.push_back(lifeStyle);
}

QList<LifeStyle> WeatherLifestyleAnalysiser::getLifestyleData()
{
    return m_lifestyleData;
}
