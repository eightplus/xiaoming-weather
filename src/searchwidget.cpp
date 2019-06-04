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

#include "searchwidget.h"
#include "utils.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include "locationworker.h"
#include "searchview.h"
#include "searchmodel.h"
#include "searchdelegate.h"
#include "searchinputedit.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

SearchWidget::SearchWidget(QWidget *parent)
    : QFrame(parent)
{
    this->setMouseTracking(true);
    this->setStyleSheet("QFrame{border:none;background-color:rgba(255, 255, 255, 0.8);}");

    m_searchTimer = new QTimer;
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(500);

    delayTimer = new QTimer(this);
    connect(delayTimer, SIGNAL(timeout()), this, SLOT(slotCloseTipMessage()));

    m_locationWorker = new LocationWorker;

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    setFocusPolicy(Qt::ClickFocus);
    main_layout->setContentsMargins(0, 0, 0, 15);
    main_layout->setSpacing(15);

    m_searchInputEdit = new SearchInputEdit;
    m_searchInputEdit->setFixedSize(280, 32);
//    m_bLayout->addWidget(m_searchInputEdit, 0, Qt::AlignCenter);
    m_searchInputEdit->setFocus();

    QPushButton *backBtn = new QPushButton();
    backBtn->setFixedSize(190, 36);
    backBtn->setText("<----" + tr("Back"));
    connect(backBtn, &QPushButton::clicked, this, [=] () {
        this->resetSearchInputEdit();
        emit this->requestBackToCityWidget();
    });

    m_tipLabel = new QLabel;
    m_tipLabel->setObjectName("tipLabel");
    m_tipLabel->setAlignment(Qt::AlignCenter);
    m_tipLabel->hide();

    m_searchModel = new SearchModel;
    m_searchView = new SearchView;
    m_searchDelegate = new SearchDelegate;
    //设置item的宽度
    if (parent) {
        m_searchView->setFixedWidth(parent->width());
    }
    else {
        m_searchView->setFixedWidth(WIDGET_WIDTH);
    }
    m_searchView->setItemDelegate(m_searchDelegate);
    m_searchView->setModel(m_searchModel);
    m_searchView->setVisible(false);

    m_noResultLabel = new QLabel();
    m_noResultLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setText(tr("No Result"));
    m_noResultLabel->setVisible(false);
    QFont font = m_noResultLabel->font();
    const QFontMetrics fm(font);
    m_noResultLabel->setFixedWidth(fm.width(m_noResultLabel->text()));

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(5, 0, 5, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(m_searchView, 1, Qt::AlignHCenter);
    contentLayout->addWidget(m_noResultLabel, 1, Qt::AlignCenter);


    main_layout->addWidget(backBtn, 0, Qt::AlignHCenter | Qt::AlignTop);
    main_layout->addWidget(m_tipLabel, 0, Qt::AlignHCenter);
    main_layout->addWidget(m_searchInputEdit, 0, Qt::AlignHCenter);
    main_layout->addLayout(contentLayout);
    main_layout->addStretch();
    this->setLayout(main_layout);

    connect(m_searchView, &SearchView::clicked, this, [this](const QModelIndex &index) {
        QVariant data = index.data(Qt::UserRole);//SearchModel::data Qt::UserRole
        QString selectCityId = data.value<QString>();
        qDebug() << "selectCityId:" << selectCityId;

        for (const LocationData &line : m_searchModel->locationList()) {//it must exits.
            qDebug() << "line.id:" << line.id;
            qDebug() << "line.city:" << line.city;
            if (line.id == selectCityId) {
                if (m_preferences->isCitiesCountOverMax()) {
                    this->displayTip(tr("Only 10 cities can be added at most!"), 3000);//最多只能添加10个城市
                    break;
                }
                if (m_preferences->isCityIdExist(line.id)) {
                    this->displayTip(tr("The city already exists!"), 3000);//该城市已存在
                    break;
                }

                CitySettingData info;
                info.active = false;
                info.id = line.id;
                info.name = line.city;
                info.icon = ":/res/weather_icons/darkgrey/100.png";
                //m_cityWidget->addCityItem(info);

//                City city;
//                city.id = line.id;
//                city.name = line.city;

                //增加城市后，更新城市个数
                emit this->requestAddCityInfo(info);

//                m_preferences->addCityInfoToPref(city);
//                //m_preferences->setCurrentCityNameById(id);
//                m_preferences->save();
////                emit this->requestRefreshCityMenu(info.active);




                this->resetSearchInputEdit();
                emit this->requestBackToCityWidget();
                break;
            }
        }
    });

    connect(m_searchTimer, SIGNAL(timeout()), this, SLOT(onSearchTimerOut()));

    connect(m_searchInputEdit, &SearchInputEdit::textChanged, this, [this] {
        m_searchView->setVisible(false);
        m_noResultLabel->setVisible(false);
        m_searchModel->setLocationData(QList<LocationData>());

        if (m_searchTimer->isActive()) {
            m_searchTimer->stop();
        }
        m_searchTimer->start();
    });
}

SearchWidget::~SearchWidget()
{
    m_locationWorker->deleteLater();

    if (m_searchTimer) {
        disconnect(m_searchTimer, SIGNAL(timeout()), this, SLOT(onSearchTimerOut()));
        if(m_searchTimer->isActive()) {
            m_searchTimer->stop();
        }
        delete m_searchTimer;
        m_searchTimer = nullptr;
    }

    if (delayTimer != NULL) {
        disconnect(delayTimer, SIGNAL(timeout()), this, SLOT(slotCloseTipMessage()));
        if(delayTimer->isActive()) {
            delayTimer->stop();
        }
        delete delayTimer;
        delayTimer = NULL;
    }

    delete m_searchInputEdit;
    delete m_tipLabel;
    delete m_noResultLabel;
    delete m_searchModel;
    delete m_searchView;
    delete m_searchDelegate;
}

void SearchWidget::onSearchTimerOut()
{
    const QString inputText = m_searchInputEdit->text().trimmed();
    if (inputText.isEmpty())
        return;

    QList<LocationData> searchResultList;
    searchResultList = m_locationWorker->exactMatchCity(inputText);
    if (searchResultList.isEmpty()) {
        m_noResultLabel->setVisible(true);
    }
    else {
        setSearchResult(searchResultList);
    }
}

void SearchWidget::setSearchResult(const QList<LocationData> data)
{
    QList<LocationData> resultList;
    for (const LocationData &line : data) {
        if (resultList.indexOf(line) == -1) {
            resultList.append(line);
        }
    }

    m_searchModel->setLocationData(resultList);
    m_searchView->setVisible(true);
    m_noResultLabel->setVisible(false);
}

void SearchWidget::resetSearchInputEdit() const
{
    m_searchInputEdit->setText("");
    m_searchInputEdit->clearFocus();
}

void SearchWidget::slotCloseTipMessage()
{
    m_tipLabel->hide();
}

void SearchWidget::displayTip(const QString &msg, int delay)
{
    m_tipLabel->setText(msg);
    m_tipLabel->show();
    if(this->delayTimer->isActive())
        this->delayTimer->stop();
    this->delayTimer->start(delay);
}
