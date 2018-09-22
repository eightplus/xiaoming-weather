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

#include "citywidget.h"

#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

CityWidget::CityWidget(QWidget *parent)
    : QFrame(parent)
{
    this->setMouseTracking(true);
    this->setStyleSheet("QFrame{border:none;background-color:rgba(255, 255, 255, 0.8);}");

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    m_noResultLabel = new QLabel(tr("No City List"));
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setVisible(false);

    m_cityView = new CityView;
    m_cityModel = new CityModel;
    m_cityDelegate = new CityDelegate;
    m_cityView->setModel(m_cityModel);
    m_cityView->setItemDelegate(m_cityDelegate);
    m_cityView->setEditTriggers(QListView::NoEditTriggers);

    main_layout->addWidget(m_cityView);
    main_layout->addWidget(m_noResultLabel);
    main_layout->setSpacing(1);
    main_layout->setMargin(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(main_layout);

    connect(m_cityView, &CityView::entered, this, [=] (const QModelIndex &index) {
        if (!index.isValid())
            return;

        m_cityView->onItemEntered(index);
        m_cityModel->setCurrentItem(index);
    });
    /*connect(m_cityView, &CityView::clicked, this, [=] (const QModelIndex &index) {
        if (!index.isValid())
            return;
    }, Qt::QueuedConnection);*/


    connect(m_cityDelegate, &CityDelegate::defaultCityButtonClicked, this, [=] {
        const QModelIndex &index = m_cityView->currentHoverIndex();
        const QString id = index.data(CityModel::IdRole).toString();
        //qDebug() << "default===" << id << index.data(CityModel::NameRole).toString();
        m_preferences->m_currentCityId = id;
        m_preferences->m_currentCity = index.data(CityModel::NameRole).toString();
        m_cityModel->updateCityListData(id);
    });
    connect(m_cityDelegate, &CityDelegate::removeCityButtonClicked, this, [=] {
        if (m_preferences->citiesCount() == 1) {
            qDebug() << "At least there must be a city!!!";
            return;
        }
        const QModelIndex &index = m_cityView->currentHoverIndex();
        const QString id = index.data(CityModel::IdRole).toString();
        //qDebug() << "remove===" << index.data(CityModel::IdRole).toString() << index.data(CityModel::NameRole).toString();
        m_preferences->removeCityInfoFromPref(id, index.data(CityModel::ActiveRole).toBool());
        m_cityModel->removeItem(id);
        m_cityModel->updateCityListData(m_preferences->m_currentCityId);
        m_cityView->closePersistentEditor(index);
        m_cityModel->showRemoveAnimation(index, m_cityView->width());
    });
    connect(m_cityModel, &CityModel::removeAnimationFinished, this, [=] (const QModelIndex &index) {
        if (m_cityModel->rowCount(QModelIndex()) <= index.row()) {
            return;
        }
        m_cityView->openPersistentEditor(m_cityView->currentHoverIndex());
    });
    connect(m_cityModel, &CityModel::cityListStateChanged, this, [=] (bool isEmpty) {
        if (isEmpty) {
            m_cityView->setVisible(false);
            m_noResultLabel->setVisible(true);
        }
        else {
            m_cityView->setVisible(true);
            m_noResultLabel->setVisible(false);
        }
    });


     /*QPushButton *mbtn = new QPushButton(this);
     mbtn->setText("Test");
     connect(mbtn, &QPushButton::clicked, this , [=] {
         this->m_cityModel->resetCityListData();
     });*/
}

CityWidget::~CityWidget()
{

}

void CityWidget::onCityListDataChanged()
{
    this->m_cityModel->removeRows(0, this->m_cityModel->rowCount(QModelIndex()));

    QList<CitySettingData> cityDataList;
    QList<QString> cityIdList = m_preferences->getCityIdList();
    for (int i=0; i<cityIdList.size(); i++) {
        CitySettingData data;
        data.active = (m_preferences->m_currentCityId == cityIdList.at(i)) ? true : false;
        data.id = cityIdList.at(i);
        data.name = m_preferences->getCitiesList().at(i);
        data.temperature = "33";
        data.icon = "101";
        cityDataList.append(data);
    }

    this->m_cityModel->resetCityListData(cityDataList);
}

void CityWidget::onCityListStateChanged(bool isEmpty)
{
    if (isEmpty) {
        m_cityView->setVisible(false);
        m_noResultLabel->setVisible(true);
    }
    else {
        m_cityView->setVisible(true);
        m_noResultLabel->setVisible(false);
    }
}
