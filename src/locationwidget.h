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

#ifndef LOCATION_WIDGET_H
#define LOCATION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class QVBoxLayout;
class QStackedWidget;
class CityWidget;
class SearchWidget;

class LocationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LocationWidget(QWidget *parent = 0);
    ~ LocationWidget();

    void initWidgets();

signals:
    void backBtnClicked();

private:
    QVBoxLayout *m_layout = nullptr;
    CityWidget *m_cityWidget = nullptr;
    SearchWidget *m_searchWidget = nullptr;
    QPushButton *m_backBtn = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;
};

#endif // LOCATION_WIDGET_H
