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

#ifndef AQI_TOOLTIP_H
#define AQI_TOOLTIP_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "data.h"

class AqiToolTip : public QFrame
{
    Q_OBJECT
public:
    explicit AqiToolTip(QWidget *parent = 0);
    ~AqiToolTip();

    void resetData();
    void popupTip(QPoint point);

protected:
//    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_radius;
    QBrush m_background;
    QColor m_borderColor;

    QFrame *m_frame = nullptr;
    QLabel *m_aqiLabel = nullptr;
    QLabel *m_qltyLabel = nullptr;
    QLabel *m_mainLabel = nullptr;
    QLabel *m_pm25Label = nullptr;
    QLabel *m_pm10Label = nullptr;
    QLabel *m_no2Label = nullptr;
    QLabel *m_so2Label = nullptr;
    QLabel *m_coLabel = nullptr;
    QLabel *m_o3Label = nullptr;

    QVBoxLayout *m_layout = nullptr;
};

#endif // AQI_TOOLTIP_H
