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

#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

class WeatherNowWidget;
class WeatherForecastWidget;
class WeatherNowCategoryWidget;
class NavigationWidget;

class WeatherWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherWidget(QWidget *parent = 0);
    ~ WeatherWidget();

signals:
    void locationBtnClicked();

private:
    QVBoxLayout *m_layout = nullptr;
//    QLabel *m_label = nullptr;

    WeatherNowWidget *m_nowWidget = nullptr;
    WeatherForecastWidget *m_forecastWidget = nullptr;
    WeatherNowCategoryWidget *m_nowCategoryWidget = nullptr;
    NavigationWidget *m_navigationWidget = nullptr;
};

#endif // WEATHERWIDGET_H
