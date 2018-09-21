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

#ifndef WEATHER_NOW_WIDGET_H
#define WEATHER_NOW_WIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QTime>

class AqiToolTip;
class ImageButton;

class WeatherNowWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WeatherNowWidget(QWidget *parent = 0);
    ~ WeatherNowWidget();

    void refreshNowWeatherData();

signals:
    void locationBtnClicked();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    AqiToolTip *m_aqiToolTip = nullptr;
    ImageButton *m_locationBtn = nullptr;
    QTimer *m_refreshTimer = nullptr;
    QTime m_time;
    int m_timeInterval;
};

#endif // WEATHER_NOW_WIDGET_H
