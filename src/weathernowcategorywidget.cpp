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

#include "weathernowcategorywidget.h"

#include <QDebug>
#include <QStackedLayout>

WeatherNowCategoryWidget::WeatherNowCategoryWidget(QWidget *parent)
    : QFrame(parent)
{
//    m_categoryLayout(new QStackedLayout),
//    m_categoryLayout->setMargin(0);
//    m_categoryLayout->setSpacing(0);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(20);

    m_label = new QLabel;
//    m_label->setFixedWidth(300);
    m_label->setFixedSize(900, 160);
    m_label->setText(tr("Weather Now Category Page"));
    m_label->setAlignment(Qt::AlignCenter/* | Qt::AlignTop*/);
    m_label->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:argb(60, 60, 60, 130);}");

    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
}

WeatherNowCategoryWidget::~WeatherNowCategoryWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void WeatherNowCategoryWidget::addCategory(QWidget * const w)
{
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    const int idx = m_categoryLayout->addWidget(w);
    m_categoryLayout->setCurrentIndex(idx);
}
