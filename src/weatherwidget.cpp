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

#include "weatherwidget.h"

#include <QDebug>

WeatherWidget::WeatherWidget(QWidget *parent)
    : QFrame(parent)
{
//    this->setFixedSize(900, 600);
//    this->setStyleSheet("QFrame {border-image: url(':/res/background/weather-snow.jpg');color:rgb(255, 255, 255);}");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(20);

    m_label = new QLabel;
    m_label->setFixedWidth(300);
    m_label->setText(tr("Weather Page"));
    m_label->setAlignment(Qt::AlignCenter/* | Qt::AlignTop*/);
    m_label->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:argb(60, 60, 60, 130);}");

    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
}

WeatherWidget::~WeatherWidget()
{

}

