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

#include "citywidget.h"
#include "utils.h"
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
    setFocusPolicy(Qt::ClickFocus);
    main_layout->setMargin(0);
    main_layout->setContentsMargins(0, 0, 0, 50);
    main_layout->setSpacing(0);

    m_noResultLabel = new QLabel(tr("No City List"));
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setVisible(false);

    m_cityView = new CityView;
    m_cityModel = new CityModel;
    m_cityDelegate = new CityDelegate;
    m_cityView->setModel(m_cityModel);
    m_cityView->setItemDelegate(m_cityDelegate);
    m_cityView->setEditTriggers(QListView::NoEditTriggers);
    //设置item的宽度
    if (parent) {
        m_cityView->setFixedWidth(parent->width());
    }
    else {
        m_cityView->setFixedWidth(WIDGET_WIDTH);
    }

    QPushButton *btAdd = new QPushButton();
    btAdd->setFixedSize(190, 36);
    btAdd->setText("+ " + tr("Add City"));
    connect(btAdd, &QPushButton::clicked, this, [=] () {
        emit this->requestAddCity();
    });

    QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp.setVerticalStretch(100);
    m_cityView->setSizePolicy(sp);

    main_layout->addWidget(m_cityView, 0, Qt::AlignHCenter);
    main_layout->addWidget(m_noResultLabel);
    main_layout->addWidget(btAdd, 0, Qt::AlignHCenter | Qt::AlignBottom);
    main_layout->addStretch();
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
        /* 设置为默认城市后，更新配置文件中的默认城市id和城市名字；
          城市列表界面上更新选中的默认城市
          让滑动导航条的第一个圆点处于选中状态；
          提交更新天气数据的请求
        */
        const QModelIndex &index = m_cityView->currentHoverIndex();
        const QString id = index.data(CityModel::IdRole).toString();
        //qDebug() << "default===" << id << index.data(CityModel::NameRole).toString();
        m_preferences->m_defaultId = id;
        m_preferences->m_currentId = id;
        m_preferences->m_defaultCity = index.data(CityModel::NameRole).toString();
        qDebug() << "m_preferences->m_defaultCity: " << m_preferences->m_defaultCity;
        m_cityModel->updateCityListActive(id);

        emit this->requestUpdateCountsAndWeather(m_preferences->m_defaultId);
    });
    connect(m_cityDelegate, &CityDelegate::removeCityButtonClicked, this, [=] {
        /*删除城市后，将该城市的信息从配置信息中删除，如果该城市为默认城市，则将城市列表的第一个城市设置为新的默认城市；
          城市列表界面上删除该城市那一行，如果该城市为默认城市，则将城市列表的第一个城市对应的那行设置为默认选中
          更新滑动导航条圆点的个数，让选中的的圆点为新的默认城市的序号；
          提交更新天气数据的请求
        */
        if (m_preferences->citiesCount() == 1) {
            qDebug() << "At least there must be a city!!!";
            return;
        }
        const QModelIndex &index = m_cityView->currentHoverIndex();
        const QString id = index.data(CityModel::IdRole).toString();
        //qDebug() << "remove===" << index.data(CityModel::IdRole).toString() << index.data(CityModel::NameRole).toString();
        m_preferences->removeCityInfoFromPref(id, index.data(CityModel::ActiveRole).toBool());
        m_preferences->save();

        m_cityModel->removeItem(id);
        m_cityModel->updateCityListActive(m_preferences->m_defaultId);
        m_cityView->closePersistentEditor(index);
        m_cityModel->showRemoveAnimation(index, m_cityView->width());

        emit this->requestUpdateCountsAndWeather(m_preferences->m_defaultId);
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

    this->onCityListDataChanged();
}

CityWidget::~CityWidget()
{
    delete m_noResultLabel;
    delete m_cityView;
    delete m_cityModel;
    delete m_cityDelegate;
}

void CityWidget::onCityListDataChanged()
{
    this->m_cityModel->removeRows(0, this->m_cityModel->rowCount(QModelIndex()));

    QList<CitySettingData> cityDataList;
    QList<QString> cityIdList = m_preferences->getCityIdList();
    for (int i=0; i<cityIdList.size(); i++) {
        CitySettingData data;
        data.active = (m_preferences->m_defaultId == cityIdList.at(i)) ? true : false;
        data.id = cityIdList.at(i);
        data.name = m_preferences->getCitiesList().at(i);
        data.temperature = "33";
        data.icon = "101";
        cityDataList.append(data);
    }

    this->m_cityModel->resetCityListData(cityDataList);
}

void CityWidget::addCityItem(const CitySettingData &data)
{
    m_cityModel->addItem(data);
    this->updateCityListActive(m_preferences->m_defaultId);
}

void CityWidget::updateCityListActive(const QString &id)
{
    m_cityModel->updateCityListActive(id);
}
