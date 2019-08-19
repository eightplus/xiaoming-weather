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

#ifndef ANIMATION_BACKGROUND_H
#define ANIMATION_BACKGROUND_H

#include <QWidget>
#include <QSequentialAnimationGroup>

#include "utils.h"

class QLabel;

class AnimationBackground : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue)

public:
    AnimationBackground(QWidget *parent = 0);

    void setWeatherCode(int code);
    void setValue(int value);
    int value() const {return m_value;}

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    void drawClearEllipse(QPainter* painter);
    void drawCloudyEllipse(QPainter* painter);
    void drawFogEllipse(QPainter* painter);

private:
    int m_value;
    WeatherState m_weatherState;
    QSequentialAnimationGroup *m_sequentialAnimationGroup = nullptr;
};

#endif // ANIMATION_BACKGROUND_H
