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

#ifndef WEATHER_FORECAST_WIDGET_H
#define WEATHER_FORECAST_WIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

#include "data.h"

class ForecastItemWidget;
class TranslucentLabel;

class QSplitter;

class WeatherForecastWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherForecastWidget(QWidget *parent = 0);
    ~ WeatherForecastWidget();

    void initTopWidget();
    void initBottomWidget();
    void paintTemperatureCurve();
    void onDataChanged();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
//    void paintEvent(QPaintEvent *event);

private:
    QSplitter *main_splitter = nullptr;
    QWidget *m_topWidget;
    QWidget *m_bottomWidget;
    QList<ForecastItemWidget *> button_list;
    QLabel *m_temperatureCurveLabel;
    Forecast forecast[6];
};

#endif // WEATHER_FORECAST_WIDGET_H
