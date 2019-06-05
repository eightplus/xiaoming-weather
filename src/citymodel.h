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

#ifndef CITYMODEL_H
#define CITYMODEL_H

#include <QAbstractListModel>

#include "data.h"

class CityModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum CityDataRole {
        IdRole = Qt::UserRole + 1,
        NameRole = Qt::UserRole + 2,
        ActiveRole = Qt::UserRole + 3,
        IconRole = Qt::UserRole + 4,
        TemperatureRole = Qt::UserRole + 5,
        RemoveRole = Qt::UserRole + 6,
        XOffsetRole = Qt::UserRole + 7,
        HoverRole = Qt::UserRole + 8
    };

    CityModel(QObject *parent = 0);
    ~CityModel();

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateCityListActive(const QString &currentId);
    void resetCityListData(QList<CitySettingData> cityDataList);
    QList<CitySettingData> getCityListData() const;
    void addItem(const CitySettingData &data);
    void removeItem(const QString &id);

public slots:
    void showRemoveAnimation(const QModelIndex &removeIndex, int maxXOffset);
    void setCurrentIndexHover(const QModelIndex &index);
    void setCurrentItem(const QModelIndex &index);

signals:
    void removeAnimationFinished(const QModelIndex &index);
    void cityListStateChanged(bool isClear);

protected:
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    QList<CitySettingData> m_cityDataList;
    QModelIndex m_removeIndex;
    QModelIndex m_hoverIndex;//m_currentIndex
    int m_curOffset;
    int m_windowWindthOffset;
    int m_timerId;
};

#endif // CITYMODEL_H
