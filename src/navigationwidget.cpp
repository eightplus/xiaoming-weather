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

#include "navigationwidget.h"
#include "citynavigation.h"
#include "imagebutton.h"
#include "tipmodule.h"
#include "texttip.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QTimer>

NavigationWidget::NavigationWidget(QWidget *parent, int cityCount)
    : QWidget(parent)
    , m_cityNavigation(new CityNavigation)
    , m_prevCityBtn(new ImageButton)
    , m_nextCityBtn(new ImageButton)
    , m_wheelTimer(new QTimer(this))
    , m_tipModule(new TipModule)
{
    m_cityNavigation->setObjectName("MainNavigation");
    m_prevCityBtn->setObjectName("PrevCityBtn");
    m_nextCityBtn->setObjectName("NextCityBtn");

    m_prevCityBtn->setVisible(false);
    m_nextCityBtn->setVisible(false);

    QHBoxLayout *m_layout = new QHBoxLayout(this);
    m_layout->setSpacing(18);
    m_layout->addStretch();
    m_layout->addWidget(m_prevCityBtn);
    m_layout->addWidget(m_cityNavigation);
    m_layout->addWidget(m_nextCityBtn);
    m_layout->addStretch();

    m_cityNavigation->setFixedHeight(24);
    m_cityNavigation->setCityCount(cityCount, 0);
    m_cityNavigation->setCurrentCityIndex(0);
    m_cityNavigation->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(m_nextCityBtn, &ImageButton::clicked, this, [=] {
        m_cityNavigation->activeNextNavigationDot();
        emit this->requestNextCity();
    });
    connect(m_prevCityBtn, &ImageButton::clicked, this, [=] {
        m_cityNavigation->activePrevNavigationDot();
        emit this->requestPrevCity();
    });

    m_wheelTimer->setSingleShot(true);
    m_wheelTimer->setInterval(240);

    tip = new TextTip(tr("Please click the left or right button to switch city!"), TrianglePostion::BottomMiddle,this);
    this->setProperty("TextTipWidget", QVariant::fromValue<QWidget *>(tip));
    this->installEventFilter(m_tipModule);
}

NavigationWidget::~NavigationWidget()
{
    delete tip;
}

void NavigationWidget::setCityCount(const int count, const int curIndex)
{
    m_count = count;
    m_cityNavigation->setCityCount(m_count, curIndex);
}

void NavigationWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_prevCityBtn->setVisible(true);
    m_nextCityBtn->setVisible(true);
}

void NavigationWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_prevCityBtn->setVisible(false);
    m_nextCityBtn->setVisible(false);
}

void NavigationWidget::wheelEvent(QWheelEvent *event)
{
    QWidget::wheelEvent(event);

    if (m_wheelTimer->isActive())
        return;

    m_wheelTimer->start();

    if (event->delta() < 0) {
        m_cityNavigation->activeNextNavigationDot();
        emit this->requestNextCity();
    }
    else {
        m_cityNavigation->activePrevNavigationDot();
        emit this->requestPrevCity();
    }
}
