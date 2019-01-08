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

#ifndef CITYNAVIGATION_H
#define CITYNAVIGATION_H

#include <QWidget>

class CityNavigation : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor activePointColor READ activePointColor WRITE setActivePointColor DESIGNABLE true)
    Q_PROPERTY(QColor unactivePointColor READ unactivePointColor WRITE setUnactivePointColor DESIGNABLE true)

public:
    explicit CityNavigation(QWidget *parent = 0);

    void setCityCount(const int count);

    void activeNextNavigationDot();
    void activePrevNavigationDot();

    void setCurrentCityIndex(const int index);


    QColor activePointColor() const { return this->m_activePointColor; }
    void setActivePointColor(QColor color);

    QColor unactivePointColor() const { return this->m_unactivePointColor; }
    void setUnactivePointColor(QColor color);

public:
    int m_itemSize;
    int m_cityCount;
    int m_currentCityIndex;

    int m_activeDiameter;
    int m_unactiveDiameter;
    QColor m_activePointColor;
    QColor m_unactivePointColor;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // CITYNAVIGATION_H
