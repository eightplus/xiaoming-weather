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

#include "citynavigation.h"

#include <QPainter>
#include <QDebug>

CityNavigation::CityNavigation(QWidget *parent)
    : QWidget(parent)
    , m_cityCount(0)
    , m_currentCityIndex(-1)
    , m_activeDiameter(8)
    , m_unactiveDiameter(5)
    , m_itemSize(16)
{

}

void CityNavigation::activeNextNavigationDot()
{
    this->m_currentCityIndex = (m_currentCityIndex + 1) % m_cityCount;

    update();
}

void CityNavigation::activePrevNavigationDot()
{
    this->m_currentCityIndex = (m_currentCityIndex ? m_currentCityIndex : m_cityCount) - 1;

    update();
}

void CityNavigation::setCityCount(const int count)
{
    this->m_cityCount = count;

    update();
}

void CityNavigation::setCurrentCityIndex(const int index)
{
    if (index < -1 || index >= m_cityCount) {
        return;
    }

    this->m_currentCityIndex = index;

    update();
}


void CityNavigation::setActivePointColor(QColor color)
{
    this->m_activePointColor = color;
}

void CityNavigation::setUnactivePointColor(QColor color)
{
    this->m_unactivePointColor = color;
}

void CityNavigation::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QColor activeColor = this->m_activePointColor;
    QColor unactiveColor = this->m_unactivePointColor;

    const QPoint offset = QPoint((width() - (this->m_cityCount * this->m_itemSize)) / 2, height() / 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);

    for (int i = 0; i != this->m_cityCount; ++i) {
        if (i == this->m_currentCityIndex) {
            painter.setBrush(activeColor);
            painter.drawEllipse(offset + QPoint(this->m_itemSize / 2 + this->m_itemSize * i, 0), this->m_activeDiameter, this->m_activeDiameter);
        }
        else {
            painter.setBrush(unactiveColor);
            painter.drawEllipse(offset + QPoint(this->m_itemSize / 2 + this->m_itemSize * i, 0), this->m_unactiveDiameter, this->m_unactiveDiameter);
        }
    }
}
