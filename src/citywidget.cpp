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
        qDebug() << "default===" << id << index.data(CityModel::NameRole).toString();
        //TODO: refresh m_cityDataList which in CityModel
        QList<CitySettingData> m_cityList = m_cityModel->getCityListData();
        QList<CitySettingData>::iterator iter = m_cityList.begin();
        for (; iter != m_cityList.end(); iter++) {
            if (iter->id == id)
                iter->setActive(false);
            else
                iter->setActive(true);
        }
    });
    connect(m_cityDelegate, &CityDelegate::removeCityButtonClicked, this, [=] {
        const QModelIndex &index = m_cityView->currentHoverIndex();
        qDebug() << "remove===" << index.data(CityModel::IdRole).toString() << index.data(CityModel::NameRole).toString();
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

    //test
    this->onCityLIstDataChanged();
}

CityWidget::~CityWidget()
{

}

void CityWidget::onCityLIstDataChanged()
{
    this->m_cityModel->removeRows(0, this->m_cityModel->rowCount(QModelIndex()));

    //TODO test data
    QList<CitySettingData> cityDataList;

    CitySettingData data1;
    data1.active = true;
    data1.id = "12345";
    data1.name = "长沙";
    data1.temperature = "33";
    data1.icon = "101";
    cityDataList.append(data1);

    CitySettingData data2;
    data2.active = false;
    data2.id = "45235";
    data2.name = "北京";
    data2.temperature = "33";
    data2.icon = "101";
    cityDataList.append(data2);

    CitySettingData data3;
    data3.active = false;
    data3.id = "33335";
    data3.name = "上海";
    data3.temperature = "33";
    data3.icon = "101";
    cityDataList.append(data3);

    CitySettingData data4;
    data4.active = false;
    data4.id = "34908";
    data4.name = "岳阳";
    data4.temperature = "33";
    data4.icon = "101";
    cityDataList.append(data4);

    CitySettingData data5;
    data5.active = false;
    data5.id = "34901";
    data5.name = "深圳";
    data5.temperature = "33";
    data5.icon = "101";
    cityDataList.append(data5);

    CitySettingData data6;
    data6.active = false;
    data6.id = "34902";
    data6.name = "广州";
    data6.temperature = "33";
    data6.icon = "101";
    cityDataList.append(data6);

    CitySettingData data7;
    data7.active = false;
    data7.id = "34903";
    data7.name = "东莞";
    data7.temperature = "33";
    data7.icon = "101";
    cityDataList.append(data7);

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
