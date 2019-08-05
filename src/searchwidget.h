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

#ifndef SEARCH_WIDGET_H
#define SEARCH_WIDGET_H

#include <QFrame>
#include <QLabel>
#include <QTimer>

#include "data.h"

class SearchModel;
class SearchView;
class SearchDelegate;
class SearchInputEdit;
class LocationWorker;


class SearchWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();

    void resetSearchInputEdit() const;
    void setSearchResult(const QList<LocationData> data);
    void displayTip(const QString &msg, int delay);

signals:
    void requestBackToCityWidget();
    void requestAddCityInfo(const CitySettingData &data);

public slots:
    void onSearchTimerOut();
    void slotCloseTipMessage();

private:
//    QFrame *m_searchFrame = nullptr;
//    QWidget *m_displayWidget = nullptr;
    QLabel *m_noResultLabel = nullptr;
    SearchModel *m_searchModel = nullptr;
    SearchView *m_searchView = nullptr;
    SearchDelegate *m_searchDelegate = nullptr;

    SearchInputEdit *m_searchInputEdit = nullptr;
    QLabel *m_tipLabel = nullptr;
    QTimer *m_searchTimer = nullptr;
    QTimer *delayTimer = nullptr;
    LocationWorker *m_locationWorker = nullptr;
};

#endif // SEARCH_WIDGET_H
