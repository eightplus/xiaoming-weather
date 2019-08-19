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

#include "animationbackground.h"

#include <QLinearGradient>
#include <QPropertyAnimation>
#include <QPainter>
#include <QDateTime>
#include <QtMath>
#include <QDebug>

namespace {

static const char *m_backgroundColor[][2] = {
    { "#63B8FF", "#1E90FF" }, // Default
    { "#63B8FF", "#1E90FF" }, // Clear
    { "#6C7B8B", "#68838B" }, // Overcast
    { "#F2F2F2", "#EEE9E9" }, // Cloudy
    { "#E0EEEE", "#E0FFFF" }, // Rain
    { "#E6E6FA", "#D3D3D3" }, // Fog
    { "#FFFFFF", "#FFFAFA" }, // Snow
    { "#CCCCCC", "#B0B0B0" }  // Haze
};

}

inline WeatherState convertCodeToState(int code)
{
    if (code == 100 || code == 900) {
        return WeatherState::CLEAR;
    }
    else if (code <= 103 && code >= 101) {
        return WeatherState::CLOUDY;
    }
    else if (code == 104 || code == 901) {
        return WeatherState::OVERCAST;
    }
    else if (code <= 204 && code >= 200) {
        return WeatherState::CLEAR;
    }
    else if (code <= 213 && code >= 205) {
        return WeatherState::OVERCAST;
    }
    else if (code <= 399 && code >= 300) {
        return WeatherState::RAIN;
    }
    else if (code <= 499 && code >= 400) {
        return WeatherState::SNOW;
    }
    else if (code <= 502 && code >= 500) {
        return WeatherState::FOG;
    }
    else if (code <= 508 && code >= 503) {
        return WeatherState::SANDSTORM;
    }
    else if (code <= 515 && code >= 509) {
        return WeatherState::FOG;
    }
    else {
        return WeatherState::DEFAULT;
    }
}

AnimationBackground::AnimationBackground(QWidget *parent):
    QWidget(parent)
    ,m_value(0)
    ,m_weatherState(DEFAULT)
{
    // animation for m_value(value)
    QPropertyAnimation *start_animation = new QPropertyAnimation(this, "value");
    start_animation->setDuration(3000);
    start_animation->setStartValue(0);
    start_animation->setEndValue(24);

    QPropertyAnimation *end_animation = new QPropertyAnimation(this, "value");
    end_animation->setDuration(3000);
    end_animation->setStartValue(24);
    end_animation->setEndValue(0);

    m_sequentialAnimationGroup = new QSequentialAnimationGroup(this);
    m_sequentialAnimationGroup->setLoopCount(-1);
    m_sequentialAnimationGroup->addAnimation(start_animation);
    m_sequentialAnimationGroup->addAnimation(end_animation);

    connect(m_sequentialAnimationGroup, &QSequentialAnimationGroup::currentAnimationChanged, this, [=] (QAbstractAnimation *current) {
        QPropertyAnimation *animation = dynamic_cast<QPropertyAnimation *>(current);
        if (animation == NULL)
            return;
    });
}

void AnimationBackground::setWeatherCode(int code)
{
    WeatherState state = DEFAULT;

    bool isNight = false;
    int curHour = QDateTime::currentDateTime().time().hour();
    if (curHour < 6 || curHour >= 18)
        isNight = true;

    state = convertCodeToState(code);
    m_weatherState = state;
    update();

    if (m_weatherState == CLEAR || m_weatherState == CLOUDY || m_weatherState == OVERCAST || m_weatherState == OVERCAST) {
        m_sequentialAnimationGroup->start();
    }
    else {
        m_sequentialAnimationGroup->stop();
    }
}

void AnimationBackground::setValue(int value)
{
    if (m_value == value) {
        return;
    }

    m_value = value;
    update();
}

void AnimationBackground::drawClearEllipse(QPainter *painter)
{
    painter->setPen(Qt::NoPen);

    qreal radiusStep = qMin(width(), height()) / 5;
    qreal radius = radiusStep;
    for(int i=0; i<4; i++) {
        int alpha = 54 - i * 10 - m_value;
        painter->setBrush(QColor(255, 255, 255, alpha));
        painter->drawEllipse(-radius, -radius, radius*2, radius*2);
        radius += radiusStep;
    }
}

void AnimationBackground::drawCloudyEllipse(QPainter *painter)
{
    painter->setPen(Qt::NoPen);

    painter->setBrush(QColor("#F8F8FFFF"));
    painter->drawEllipse(width()/2 - m_value, -(width()/4*3), width(), width());

    painter->setBrush(QColor("#F0FFF0FF"));
    painter->drawEllipse(-width()/3 + m_value, -(width()/4*3), width(), width());

    painter->setBrush(QColor("#E0EEE0FF"));
    painter->drawEllipse(m_value*2, -(width()/4*3), width(), width());
}

void AnimationBackground::drawFogEllipse(QPainter *painter)
{
    painter->setPen(Qt::NoPen);

    painter->setBrush(QColor("#D6D6D600)"));
    int radius = qSqrt(width()*width() + height()*height()) / 2;
    painter->drawEllipse(-radius-m_value+10, -radius-m_value+10, radius*2, radius*2);
    painter->drawEllipse(width()-radius+m_value-10, height()-radius+m_value-10, radius*2, radius*2);
}

void AnimationBackground::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反走样，避免锯齿

    painter.setClipRect(0, 0, width(), height());

    // draw backgroud
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, m_backgroundColor[m_weatherState][0]);
    gradient.setColorAt(1, m_backgroundColor[m_weatherState][1]);
    painter.fillRect(0, 0, width(), height(), QBrush(gradient));

    switch (m_weatherState) {
    case CLEAR:
        drawClearEllipse(&painter);
        break;

    case CLOUDY:
    case OVERCAST:
        drawCloudyEllipse(&painter);
        break;

    case FOG:
    case SANDSTORM:
        drawFogEllipse(&painter);
        break;

    default:
        break;
    }
}
