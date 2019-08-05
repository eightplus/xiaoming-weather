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

#include "citymodel.h"

#include <QSize>
#include <QDebug>

/*
 * QObject定时器：
 * startTimer()
 * timerEvent(QTimerEvent *event);
 * killTimer()
*/


CityModel::CityModel(QObject *parent) : QAbstractListModel(parent)
  , m_windowWindthOffset(0)
  , m_curOffset(0)
  , m_timerId(0)
{
    m_removeIndex = QModelIndex();
    m_hoverIndex = QModelIndex();

    connect(this, &CityModel::removeAnimationFinished, this, [=] (const QModelIndex &index) {
        //qDebug() << "removeAnimationFinished row=" << index.row();
        emit dataChanged(index, index);

        if (m_cityDataList.size() == 0) {
            emit cityListStateChanged(true);
        }
    });
}

CityModel::~CityModel()
{
    beginResetModel();
    m_cityDataList.clear();
    endResetModel();
}

void CityModel::updateCityListActive(const QString &currentId)
{
    QList<CitySettingData>::iterator iter = m_cityDataList.begin();
    for (; iter != m_cityDataList.end(); iter++) {
        if (iter->id == currentId)
            iter->setActive(true);
        else
            iter->setActive(false);
    }
}

void CityModel::resetCityListData(QList<CitySettingData> cityDataList)
{
    beginResetModel();
    m_cityDataList = cityDataList;
    endResetModel();

    if (m_cityDataList.isEmpty()) {
        emit cityListStateChanged(true);
    }
}

QList<CitySettingData> CityModel::getCityListData() const
{
    return this->m_cityDataList;
}

void CityModel::addItem(const CitySettingData &data)
{
    bool isExist = false;
    foreach (const CitySettingData &tmp, m_cityDataList) {
        if (tmp.id == data.id) {
            isExist = true;
            break;
        }
        else {
            continue;
        }
    }

    if (!isExist) {
        m_cityDataList.append(data);
        const QModelIndex mindex = index(0);
        emit dataChanged(mindex, mindex);
    }
}

void CityModel::removeItem(const QString &id)
{
//    m_cityDataList.remove(id);

    foreach (const CitySettingData &data, m_cityDataList) {
        if (data.id == id) {
            const int idx = m_cityDataList.indexOf(data);

            if (idx == -1)
                return;

            m_cityDataList.removeAt(idx);
            break;
        }
    }
}

int CityModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_cityDataList.size();

//    const int itemCount = m_cityDataList.size();
//    const int rowCount = itemCount % 6;//按每行6个来计算
//    return itemCount + (rowCount ? 6 - rowCount : rowCount);
}

QVariant CityModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (m_cityDataList.size() <= 0 || index.row() > m_cityDataList.size()) {
        return QVariant();
    }

    CitySettingData cityData = m_cityDataList.at(m_cityDataList.size() - 1 - index.row());
    switch (role) {
    case IdRole:
        return cityData.id;
        break;
    case NameRole:
        return cityData.name;
        break;
    case ActiveRole:
        return cityData.active;
    case IconRole:
        return QString(":/res/%1.png").arg(cityData.icon);
        break;
    case TemperatureRole:
        return cityData.temperature;
        break;
    case Qt::SizeHintRole:
        return QSize(0, 56);//高度为56,宽度扩展
        break;
    case RemoveRole: {
        if (m_removeIndex == index) {
            return true;
        }
        else {
            return false;
        }
        break;
    }
    case XOffsetRole:
        return m_curOffset;
        break;
    case HoverRole:
        return m_hoverIndex == index;
        break;
    default:
        break;
    }

    return QVariant();
}

Qt::ItemFlags CityModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index);
}

void CityModel::setCurrentItem(const QModelIndex &index)
{
    const QModelIndex origIndex = m_hoverIndex;
    m_hoverIndex = index;

    emit dataChanged(origIndex, origIndex);
    emit dataChanged(m_hoverIndex, m_hoverIndex);
}

void CityModel::showRemoveAnimation(const QModelIndex &removeIndex, int offset)
{
    m_removeIndex = removeIndex;
    m_windowWindthOffset = offset;

    if (m_timerId == 0) {
        m_timerId = startTimer(10);
    }
}

void CityModel::setCurrentIndexHover(const QModelIndex &index)
{
    m_hoverIndex = index;

    emit dataChanged(m_hoverIndex, m_hoverIndex);
}

void CityModel::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    if (m_curOffset < m_windowWindthOffset) {
        m_curOffset += 15;
        emit dataChanged(m_removeIndex, m_removeIndex);

        return;
    }

    if (m_timerId != 0) {
        killTimer(m_timerId);
    }

    emit removeAnimationFinished(m_removeIndex);

    m_removeIndex = QModelIndex();
    m_timerId = 0;
    m_curOffset = 0;
}
