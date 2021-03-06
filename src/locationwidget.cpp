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

#include "locationwidget.h"
#include "citywidget.h"
#include "searchwidget.h"
#include "utils.h"
#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QDebug>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

namespace {
const int BACK_BUTTON_HEIGHT = 24;
}

inline void animationFromRightToLeft(QWidget *leftWidget, QWidget *rightWidget, int delay)
{
    /*
     * 页面切换的动画效果为从右往左滑
     * 显示rightWidget，隐藏leftWidget
     * 主界面为900×600,标题栏高度为39,所以天气显示界面为900×561
     * leftWidget从QRect(0,0 280x535)处移动到QRect(-280,0 280x535)
     * rightWidget从QRect(280,0 280x535)处移动到QRect(0,0 280x535)
     */
    rightWidget->show();
    leftWidget->show();
    rightWidget->resize(leftWidget->size());
    QRect leftStartRect = QRect(0, 0, leftWidget->width(), leftWidget->height());
    QRect leftEndRect = QRect(-leftWidget->width(), 0, leftWidget->width(), leftWidget->height());
//    qDebug() << "animationFromRightToLeft: leftStartRect" << leftStartRect << ",leftEndRect" << leftEndRect;
    QPropertyAnimation *leftAnimation = new QPropertyAnimation(leftWidget, "geometry");
    leftAnimation->setDuration(delay);
    leftAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    leftAnimation->setStartValue(leftStartRect);
    leftAnimation->setEndValue(leftEndRect);
    leftAnimation->start();
    leftAnimation->connect(leftAnimation, &QPropertyAnimation::finished, leftAnimation, &QPropertyAnimation::deleteLater);

    QRect rightStartRect = QRect(leftWidget->width(), 0, rightWidget->width(), rightWidget->height());
    QRect rightEndRect = QRect(0, 0, rightWidget->width(), rightWidget->height());
//    qDebug() << "animationFromRightToLeft: rightStartRect" << rightStartRect << ",rightEndRect" << rightEndRect;
    QPropertyAnimation *rightAnimation = new QPropertyAnimation(rightWidget, "geometry");
    rightAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    rightAnimation->setDuration(delay);
    rightAnimation->setStartValue(rightStartRect);
    rightAnimation->setEndValue(rightEndRect);
    rightAnimation->start();
    rightAnimation->connect(rightAnimation, &QPropertyAnimation::finished, rightAnimation, &QPropertyAnimation::deleteLater);
    rightAnimation->connect(rightAnimation, &QPropertyAnimation::finished, leftWidget, &QWidget::hide);
}

inline void animationFromLeftToRight(QWidget *leftWidget, QWidget *rightWidget, int delay)
{
    /*
     * 页面切换的动画效果为从左往右滑
     * 显示rightWidget，隐藏leftWidget
     * 主界面为900×600,标题栏高度为39,所以天气显示界面为900×561
     * leftWidget从QRect(0,0 280x535)处移动到QRect(280,0 280x535)
     * rightWidget从QRect(-280,0 280x535)处移动到QRect(0,0 280x535)
     */
    rightWidget->show();
    leftWidget->show();
    rightWidget->resize(leftWidget->size());
    QRect leftStartRect = QRect(0, 0, leftWidget->width(), leftWidget->height());
    QRect leftEndRect = QRect(leftWidget->width(), 0, leftWidget->width(), leftWidget->height());/*leftStart*/
    //qDebug() << "animationFromLeftToRight: leftStartRect" << leftStartRect << ",leftEndRect" << leftEndRect;
    QPropertyAnimation *leftAnimation = new QPropertyAnimation(leftWidget, "geometry");
    leftAnimation->setDuration(delay);
    leftAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    leftAnimation->setStartValue(leftStartRect);
    leftAnimation->setEndValue(leftEndRect);
    leftAnimation->start();
    leftAnimation->connect(leftAnimation, &QPropertyAnimation::finished, leftAnimation, &QPropertyAnimation::deleteLater);

    QRect rightStartRect = QRect(-leftWidget->width(), 0, rightWidget->width(), rightWidget->height());
    QRect rightEndRect = QRect(0, 0, rightWidget->width(), rightWidget->height());
    //qDebug() << "animationFromLeftToRight: rightStartRect" << rightStartRect << ",rightEndRect" << rightEndRect;
    QPropertyAnimation *rightAnimation = new QPropertyAnimation(rightWidget, "geometry");
    rightAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    rightAnimation->setDuration(delay);
    rightAnimation->setStartValue(rightStartRect);
    rightAnimation->setEndValue(rightEndRect);
    rightAnimation->start();
    rightAnimation->connect(rightAnimation, &QPropertyAnimation::finished, rightAnimation, &QPropertyAnimation::deleteLater);
    rightAnimation->connect(rightAnimation, &QPropertyAnimation::finished, leftWidget, &QWidget::hide);
}

LocationWidget::LocationWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(parent->width(), parent->height() - TITLE_HEIGHT);//高度上减去标题栏的高度
    initWidgets();
}

LocationWidget::~LocationWidget()
{
//    QLayoutItem *child;
//    while ((child = m_settingLayout->takeAt(0)) != 0) {
//        if (child->widget())
//            child->widget()->deleteLater();
//        delete child;
//    }

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void LocationWidget::initWidgets()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_stackedWidget = new QStackedWidget;
    m_cityWidget = new CityWidget;
    m_cityWidget->setContentsMargins(0, 0, 0, 0);
    m_cityWidget->setFixedSize(this->width(), this->height()/* - BACK_BUTTON_HEIGHT*/);
    m_cityWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->addWidget(m_cityWidget);
    m_stackedWidget->setCurrentWidget(m_cityWidget);

    m_searchWidget = new SearchWidget;
    m_searchWidget->setContentsMargins(0, 0, 0, 0);
    m_searchWidget->setFixedSize(this->width(), this->height()/* - BACK_BUTTON_HEIGHT*/);
    m_searchWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->addWidget(m_searchWidget);

    /*
    m_backBtn = new QPushButton;
    m_backBtn->setFixedSize(120, BACK_BUTTON_HEIGHT);
    //m_backBtn->setFixedHeight(BACK_BUTTON_HEIGHT);
    m_backBtn->setFocusPolicy(Qt::NoFocus);
//    m_backBtn->setStyleSheet("QPushButton{border:none;text-align:center;font-size:14px;color:#ffffff;background:transparent;}");
    m_backBtn->setStyleSheet("QPushButton{border-radius:4px;background-color:rgba(0,0,0,0.2);color:rgb(255,255,255);}");
    m_backBtn->setText(tr("Back"));
//    QFont font = m_backBtn->font();
//    const QFontMetrics fm(font);
    //m_backBtn->setFixedWidth(fm.width(m_backBtn->text()) + 10);
*/

    m_layout->addWidget(m_stackedWidget, 0, Qt::AlignHCenter);
//    m_layout->addWidget(m_backBtn, 0, Qt::AlignHCenter);
    m_layout->addStretch(0);

//    connect(m_backBtn, &QPushButton::clicked, this, &LocationWidget::backBtnClicked);
    connect(m_cityWidget, &CityWidget::requestAddCity, this, [=] () {
        emit this->requestChangeBackBtnState(BackBtnState::SearchState);
        m_stackedWidget->setCurrentWidget(m_searchWidget);
        animationFromLeftToRight(m_cityWidget, m_searchWidget, 500);
    });
    connect(m_searchWidget, &SearchWidget::requestBackToCityWidget, this, &LocationWidget::onResponseBackToCityWidget);
    //增加城市后，更新城市个数
    connect(m_searchWidget, &SearchWidget::requestAddCityInfo, this, [=] (const CitySettingData &data) {
        /*增加新城市后，将新城市信息加入配置信息中，并设置为默认城市；
          城市列表界面上增加新的一行，并将该行设置为默认选中
          更新滑动导航条圆点的个数，让选择的圆点为第一个，并让新城市显示为第一个圆点对应的页面；
          提交更新天气数据的请求
        */

        City city;
        city.id = data.id;
        city.name = data.name;

        m_preferences->addCityInfoToPref(city);
        m_preferences->setCurrentCityNameById(city.id);
        m_preferences->save();

        m_cityWidget->addCityItem(data);

        emit this->requestUpdateCityCounts(m_preferences->citiesCount(), 0);

        emit this->requestRefreshWeatherById(city.id);
    });


    //删除城市后，更新城市个数
    connect(m_cityWidget, &CityWidget::requestUpdateCountsAndWeather, this, [=] (const QString &id) {
        emit this->requestUpdateCityCounts(m_preferences->citiesCount(), 0);
        emit this->requestRefreshWeatherById(id);
    });

    /*QStackedLayout *contentLayout = new QStackedLayout(this);
    contentLayout->addWidget(m_cityWidget);
    contentLayout->addWidget(m_searchWidget);
    m_cityWidget->setVisible(true);*/
}

void LocationWidget::updateCityListActive(const QString &id)
{
    m_cityWidget->updateCityListActive(id);
}

void LocationWidget::doResetSearchInputEdit()
{
    m_searchWidget->resetSearchInputEdit();
}

void LocationWidget::onResponseBackToCityWidget()
{
    emit this->requestChangeBackBtnState(BackBtnState::LocateState);
    m_stackedWidget->setCurrentWidget(m_cityWidget);
    animationFromRightToLeft(m_searchWidget, m_cityWidget, 500);
}
