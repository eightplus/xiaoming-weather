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

#include "backgroundwidget.h"
#include "utils.h"
#include "parseinterface.h"

#include <QPainter>
#include <QDebug>

namespace {

const QString WEATHER_DEFAULT_IMG = ":/res/background/weather-default-background.jpg";
const QString WEATHER_CLEAR_DAY_BACKGROUND_IMG = ":/res/background/weather-clear.jpg";
const QString WEATHER_CLEAR_NIGHT_BACKGROUND_IMG = ":/res/background/weather-clear-night.jpg";
const QString WEATHER_FEW_CLOUDS_DAY_BACKGROUND_IMG = ":/res/background/weather-few-clouds.jpg";
const QString WEATHER_FEW_CLOUDS_NIGHT_BACKGROUND_IMG = ":/res/background/weather-few-clouds-night.jpg";
const QString WEATHER_FOG_BACKGROUND_IMG = ":/res/background/weather-fog.jpg";
const QString WEATHER_OVERCAST_BACKGROUND_IMG = ":/res/background/weather-overcast.jpg";
const QString WEATHER_SHOWERS_BACKGROUND_IMG = ":/res/background/weather-showers.jpg";
const QString WEATHER_SNOW_BACKGROUND_IMG = ":/res/background/weather-snow.jpg";
const QString WEATHER_STORM_BACKGROUND_IMG = ":/res/background/weather-storm.jpg";

}

BackgroundWidget::BackgroundWidget(QWidget *parent) :
    QWidget(parent)
    , m_isNight(false)
    , m_radius(0)
    , m_background(QBrush(QColor(255,255,255,230)))
    , m_borderColor(QColor(224,224,224,130))
{
    //this->setObjectName("WeatherBackground");

    m_color = QColor("#FFFAFA");
    m_opacity = 1.0;

    //this->setStyleSheet("QWidget{border-image: url(:/res/background/weather-clear-night.jpg);}");//color:rgb(255, 255, 255);

    m_weatherBgPixmap = QPixmap(WEATHER_DEFAULT_IMG);
    if (!m_weatherBgPixmap.isNull())
        m_weatherBgPixmap = m_weatherBgPixmap.scaled(WIDGET_WIDTH, WIDGET_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//Qt::KeepAspectRatio

}

void BackgroundWidget::setColor(const QColor &color)
{
    m_color = color;

    update();
}

void BackgroundWidget::setBackPixmap(const QString &pix)
{
    m_backPixmap = pix;

    update();
}

void BackgroundWidget::setForNight(bool isNight)
{
    this->m_isNight = isNight;

    /*QMap<QString, QString> weather_iconMap = global_ParseInterface->getweatherIconMap();
    for (auto iter = weather_iconMap.begin(); iter != weather_iconMap.end(); ++iter) {
        QString key = iter.key();
        QString value = iter.value();
        qDebug() << "key=" << key << ",value=" << value;
    }*/

    /*if (this->m_isNight) {
        m_weatherBgPixmap = QPixmap(WEATHER_DEFAULT_IMG);
        if (!m_weatherBgPixmap.isNull())
            m_weatherBgPixmap = m_weatherBgPixmap.scaled(WIDGET_WIDTH, WIDGET_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else {
        m_weatherBgPixmap = QPixmap(WEATHER_CLEAR_NIGHT_BACKGROUND_IMG);
        if (!m_weatherBgPixmap.isNull())
            m_weatherBgPixmap = m_weatherBgPixmap.scaled(WIDGET_WIDTH, WIDGET_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }*/
}

void BackgroundWidget::paintEvent(QPaintEvent *event)
{
    /*
    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // 使用图片背景时qss文件中的qproperty-backPixmap的属性必须为"default"或者空，如果设置为图片路径，则仅仅使用该图片作为背景，而不设置背景颜色
    if(m_backPixmap.isEmpty() || m_backPixmap == "default") {
        painter.setBrush(m_color);
        painter.drawRect(rect());
        painter.setOpacity(m_opacity);
        painter.drawPixmap(rect(), m_weatherBgPixmap, rect());
    }
    else {
        painter.drawPixmap(rect(), QPixmap(m_backPixmap), rect());
    }

    update();
    */

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath bgPath;
    bgPath.addRoundedRect(bgRect, this->m_radius, this->m_radius);
    painter.fillPath(bgPath, background);

    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    int borderRadius = this->m_radius;
    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
    borderRadius += 0.5;
    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1.0);
    painter.strokePath(borderPath, borderPen);
}
