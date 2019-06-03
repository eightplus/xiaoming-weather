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

#ifndef CITYWIDGET_H
#define CITYWIDGET_H

#include <QFrame>
#include <QLabel>

#include "data.h"
#include "cityview.h"
#include "citymodel.h"
#include "citydelegate.h"

class CityWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CityWidget(QWidget *parent = 0);
    ~CityWidget();

signals:
    void requestAddCity();
    void requestUpdateCount();

public slots:
    void onCityListDataChanged();

private:
    CityView *m_cityView = nullptr;
    CityModel *m_cityModel = nullptr;
    CityDelegate *m_cityDelegate = nullptr;
    QLabel *m_noResultLabel = nullptr;
};

#endif // CITYWIDGET_H
