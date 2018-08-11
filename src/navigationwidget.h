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

#ifndef NAVIGATION_WIDGET_H
#define NAVIGATION_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class CityNavigation;
class ImageButton;

class NavigationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationWidget(QWidget *parent = 0, int cityCount = 0);
    ~ NavigationWidget();

signals:
    void requestPrevCity();
    void requestNextCity();

private:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    CityNavigation *m_cityNavigation = nullptr;
    ImageButton *m_prevCityBtn = nullptr;
    ImageButton *m_nextCityBtn = nullptr;
};

#endif // NAVIGATION_WIDGET_H
