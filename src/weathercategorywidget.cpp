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

#include "weathercategorywidget.h"
#include "categorybutton.h"
#include "indexitemwidget.h"
#include "texttip.h"
#include "tipmodule.h"
#include "windtooltip.h"

#include "preferences.h"
#include "global.h"
using namespace Global;

#include <QApplication>
#include <QDebug>
#include <QStackedLayout>
#include <QPainter>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

namespace {

const int BtnWidgetHeight = 28;
const int TextHeight = 20;
const int AlignMent = 18;
const int TimeTextHeight = 56;
const int TemperatureDotSize = 3;
const int CurrentPopItemWidth = 110;
const int PopItemWidth = 100;
const QStringList LIFESTYLE = {"comf", "drsg", "flu", "sport", "trav", "uv", "cw", "air"};
const QStringList LIFESTYLEICON = {":/res/lifestyle/comf_index.png", ":/res/lifestyle/clothe_index.png", ":/res/lifestyle/flu_index.png", ":/res/lifestyle/sport_index.png", ":/res/lifestyle/trav_index.png", ":/res/lifestyle/ultraviolet_rays_index.png", ":/res/lifestyle/cash_wash_index.png", ":/res/lifestyle/air_index.png"};

}

WeatherCategoryWidget::WeatherCategoryWidget(QWidget *parent)
    : QFrame(parent)
    , m_stackedLayout(new QStackedLayout)
    , m_tipModule(new TipModule(TrianglePostion::TopLeft))
    , m_tempMouseEnterPress(false)
    , m_windMouseEnterPress(false)
    , m_popMouseEnterPress(false)
{
//    this->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::red));
//    this->setPalette(palette);

//    this->setFrameShape(QFrame::NoFrame);
//    this->setStyleSheet("QFrame{border-radius: 5px; color:rgb(250, 250, 250); background-color:rgba(63,63,63,20%); border: 0px;}");

    m_tempHoverIndex = -1;
    m_firstTempX = 51;
    m_tempXSpace = 100;

    m_windHoverIndex = -1;
    m_firstWindX = 51;
    m_windXSpace = 100;

    m_popHoverIndex = -1;
    m_firstPopX = 51;

//    this->setMouseTracking(true);
//    this->setAttribute(Qt::WA_Hover, true);

    m_tempX = new QList<int>();//int m_tempX[8] = {51, 151, 251, 351, 451, 551, 651, 751};
    m_windX = new QList<int>();//int m_windX[8] = {51, 151, 251, 351, 451, 551, 651, 751};
    m_popX = new QList<int>();

    m_stackedLayout->setMargin(0);
    m_stackedLayout->setSpacing(0);
    this->setLayout(m_stackedLayout);

    m_lifeIndexList = LIFESTYLE;
    m_lifeIndexIconList = LIFESTYLEICON;
    m_btnTextLists << tr("Temperature") << tr("Wind") << tr("Pop") << tr("Lifestyle");

    m_lifeItems.clear();
    m_lifestyleTips.clear();

    this->initTemperatureWidget();
    this->initWindWidget();
    this->initPopWidget();
    this->initLifeStyleWidget();

    m_btnsWidget = new QFrame(this);
    m_btnsWidget->setFixedHeight(BtnWidgetHeight);
    m_btnsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->setSpacing(0);
    btn_layout->setMargin(0);

    for(int i = 0; i < m_btnTextLists.count(); i++) {
        CategoryButton *btn = new CategoryButton(this);
        btn->setFixedSize(QSize(120, m_btnsWidget->height()));
        btn->setIconAndText(QPixmap(":/res/detail.png"), m_btnTextLists.at(i));
        btn_layout->addWidget(btn);
        connect(btn, SIGNAL(requestChangeButtonStatus(CategoryButton*)), this, SLOT(changeCurrentPage(CategoryButton*)));
        m_btnArray[i] = btn;
    }
    m_btnsWidget->move(0, this->height() - m_btnsWidget->height());
    m_btnArray[0]->setMouseEnterPress(true);
    btn_layout->addStretch();
    m_btnsWidget->setLayout(btn_layout);



    /*m_tempAnimation = new QPropertyAnimation(m_temperatureCurveLabel, "opacity");
    //setTargetObject
    m_tempAnimation->setDuration(200);
    m_tempAnimation->setStartValue(0);
    m_tempAnimation->setKeyValueAt(0.25, 1);
    m_tempAnimation->setKeyValueAt(0.5, 1);
    m_tempAnimation->setKeyValueAt(0.75, 1);
    m_tempAnimation->setEndValue(1);
    connect(m_tempAnimation, &QPropertyAnimation::finished, this, [=] {
        m_stackedLayout->setCurrentIndex(0);
        m_btnsWidget->raise();
//        m_tempAnimation->deleteLater();
    });*/


    /*m_lifeAnimation = new QPropertyAnimation(m_lifestyleWidget, "opacity");
    m_lifeAnimation->setDuration(200);
    m_lifeAnimation->setStartValue(0);
    m_lifeAnimation->setKeyValueAt(0.25, 1);
    m_lifeAnimation->setKeyValueAt(0.5, 1);
    m_lifeAnimation->setKeyValueAt(0.75, 1);
    m_lifeAnimation->setEndValue(1);
    connect(m_lifeAnimation, &QPropertyAnimation::finished, this, [=] {
        m_stackedLayout->setCurrentIndex(3);
        m_btnsWidget->raise();
//        m_lifeAnimation->deleteLater();
    });

    m_animationGroup = new QParallelAnimationGroup;
    m_animationGroup->addAnimation(m_tempAnimation);
    m_animationGroup->addAnimation(m_lifeAnimation);*/
}

WeatherCategoryWidget::~WeatherCategoryWidget()
{
//    delete m_tempAnimation;
//    delete m_lifeAnimation;
//    delete m_animationGroup;
    delete m_tempX;
    delete m_windX;
    delete m_popX;

    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        delete item;
        item = NULL;
    }
    m_lifeItems.clear();

    for(int i=0; i<m_lifestyleTips.count(); i++) {
        TextTip *tip = m_lifestyleTips.at(i);
        delete tip;
        tip = NULL;
    }
    m_lifestyleTips.clear();
    delete m_tipModule;

    QLayoutItem *child;
    while ((child = m_stackedLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
    delete m_stackedLayout;

    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }
    delete m_indexGridLayout;

    for (int i = 0; i < 4; i++) {
        delete m_btnArray[i];
    }
}

void WeatherCategoryWidget::updateTempCurveLabel()
{
    m_temperatureCurveLabel->update();
}

void WeatherCategoryWidget::changeCurrentPage(CategoryButton *label)
{
    for(int i=0; i<4; i++) {
        if(label != m_btnArray[i]) {
            m_btnArray[i]->setMouseEnterPress(false);
        }
        else {
            /*if (i == 0) {
                m_tempAnimation->start();
                //m_animationGroup->start();
            }
            else {
                m_stackedLayout->setCurrentIndex(i);
            }*/
            m_stackedLayout->setCurrentIndex(i);
            m_btnsWidget->raise();
//            for (int i = 0; i < 4; i++) {
//                m_btnArray[i]->raise();
//            }
        }
    }
    m_tempMouseEnterPress = false;
    m_windMouseEnterPress = false;
    m_popMouseEnterPress = false;
    if (m_windTip->isVisible()) {
        m_windTip->setVisible(false);
    }
}

void WeatherCategoryWidget::calculateTemperatureHoverIndex(int hoverX)
{
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0)
        return;

    if (m_tempX->size() < count) {
        m_tempHoverIndex = -1;
        return;
    }

    m_tempHoverIndex = -1;
    for (int i=0;i<count;i++) {
        if (hoverX <= m_tempX->at(i) + m_tempXSpace/2) {
            m_tempHoverIndex = i;
            break;
        }
    }

    /*if (hoverX <= m_tempX->at(0) + m_tempXSpace/2) {
        m_tempHoverIndex = 0;
    }
    else if (hoverX <= m_tempX->at(1) + m_tempXSpace/2) {
        m_tempHoverIndex = 1;
    }
    else if (hoverX <= m_tempX->at(2) + m_tempXSpace/2) {
        m_tempHoverIndex = 2;
    }
    else if (hoverX <= m_tempX->at(3) + m_tempXSpace/2) {
        m_tempHoverIndex = 3;
    }
    else if (hoverX <= m_tempX->at(4) + m_tempXSpace/2) {
        m_tempHoverIndex = 4;
    }
    else if (hoverX <= m_tempX->at(5) + m_tempXSpace/2) {
        m_tempHoverIndex = 5;
    }
    else if (hoverX <= m_tempX->at(6) + m_tempXSpace/2) {
        m_tempHoverIndex = 6;
    }
    else if (hoverX <= m_tempX->at(7) + m_tempXSpace/2) {
        m_tempHoverIndex = 7;
    }
    else {
        m_tempHoverIndex = -1;
    }*/
}

int WeatherCategoryWidget::calculateWindHoverIndex(int hoverX)
{
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0) {
        m_windHoverIndex = -1;
        return 0;
    }

    if (m_windX->size() < count) {
        m_windHoverIndex = -1;
        return 0;
    }

    if (hoverX < m_firstWindX) {
        m_windHoverIndex = -1;
        return 0;
    }

    for (int i=0;i<count;i++) {
        if (hoverX <= m_windX->at(i) + m_windXSpace/2) {
            if (m_windHoverIndex == i) {
                return 0;
            }
            else {
                m_windHoverIndex = i;
            }
            return m_windX->at(i);
        }
    }

    m_windHoverIndex = -1;
    return 0;

/*
    if (hoverX <= m_windX->at(0) + m_windXSpace/2) {
        if (m_windHoverIndex == 0) {
            return 0;
        }
        else {
            m_windHoverIndex = 0;
        }
        return m_windX->at(0);
    }
    else if (hoverX <= m_windX->at(1) + m_windXSpace/2) {
        if (m_windHoverIndex == 1) {// no changes
            return 0;
        }
        else {// have changes
            m_windHoverIndex = 1;
        }
        return m_windX->at(1);
    }
    else if (hoverX <= m_windX->at(2) + m_windXSpace/2) {
        if (m_windHoverIndex == 2) {
            return 0;
        }
        else {
            m_windHoverIndex = 2;
        }
        return m_windX->at(2);
    }
    else if (hoverX <= m_windX->at(3) + m_windXSpace/2) {
        if (m_windHoverIndex == 3) {
            return 0;
        }
        else {
            m_windHoverIndex = 3;
        }
        return m_windX->at(3);
    }
    else if (hoverX <= m_windX->at(4) + m_windXSpace/2) {
        if (m_windHoverIndex == 4) {
            return 0;
        }
        else {
            m_windHoverIndex = 4;
        }
        return m_windX->at(4);
    }
    else if (hoverX <= m_windX->at(5) + m_windXSpace/2) {
        if (m_windHoverIndex == 5) {
            return 0;
        }
        else {
            m_windHoverIndex = 5;
        }
        return m_windX->at(5);
    }
    else if (hoverX <= m_windX->at(6) + m_windXSpace/2) {
        if (m_windHoverIndex == 6) {
            return 0;
        }
        else {
            m_windHoverIndex = 6;
        }
        return m_windX->at(6);
    }
    else if (hoverX <= m_windX->at(7) + m_windXSpace/2) {
        if (m_windHoverIndex == 7) {
            return 0;
        }
        else {
            m_windHoverIndex = 7;
        }
        return m_windX->at(7);
    }
    else {
        m_windHoverIndex = -1;
        return 0;
    }*/
}

void WeatherCategoryWidget::calculatePopHoverIndex(int hoverX)
{
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0) {
        m_popHoverIndex = -1;
        return;
    }

    if (m_popX->size() < 8) {
        m_popHoverIndex = -1;
        return;
    }

    if (hoverX < m_firstPopX) {
        m_popHoverIndex = -1;
        return;
    }

    m_popHoverIndex = -1;
    for (int i=0;i<count;i++) {
        if (hoverX <= m_popX->at(i) + PopItemWidth) {
            m_popHoverIndex = i;
            break;
        }
    }

    /*else if (hoverX <= m_popX->at(0) + PopItemWidth) {
        m_popHoverIndex = 0;
    }
    else if (hoverX <= m_popX->at(1) + PopItemWidth) {
        m_popHoverIndex = 1;
    }
    else if (hoverX <= m_popX->at(2) + PopItemWidth) {
        m_popHoverIndex = 2;
    }
    else if (hoverX <= m_popX->at(3) + PopItemWidth) {
        m_popHoverIndex = 3;
    }
    else if (hoverX <= m_popX->at(4) + PopItemWidth) {
        m_popHoverIndex = 4;
    }
    else if (hoverX <= m_popX->at(5) + PopItemWidth) {
        m_popHoverIndex = 5;
    }
    else if (hoverX <= m_popX->at(6) + PopItemWidth) {
        m_popHoverIndex = 6;
    }
    else if (hoverX <= m_popX->at(7) + PopItemWidth) {
        m_popHoverIndex = 7;
    }
    else {
        m_popHoverIndex = -1;
    }*/
}

bool WeatherCategoryWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_temperatureCurveLabel) {
        if (event->type() == QEvent::Paint) {
            paintTemperatureCurve();
        }
        else if (event->type() == QEvent::Enter) {
            QPoint windowPos = mapFromGlobal(QCursor::pos());//将显示器坐标转换成窗口坐标
            int hoverX = windowPos.x();
//            if (hoverX > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            this->calculateTemperatureHoverIndex(hoverX);
            m_tempMouseEnterPress = true;
            m_temperatureCurveLabel->update();
        }
        else if (event->type() == QEvent::Leave) {//QEvent::FocusOut
            m_tempHoverIndex = -1;
            m_tempMouseEnterPress = false;
            m_temperatureCurveLabel->update();
        }
        else if (event->type() == QEvent::HoverMove/* || event->type() == QEvent::HoverLeave*/) {
            //qDebug()<< "hoverMove" << ((QHoverEvent*)event)->pos();
            QPoint windowPos = mapFromGlobal(QCursor::pos());//将显示器坐标转换成窗口坐标
            int hoverX = windowPos.x();
//            if (hoverX > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            this->calculateTemperatureHoverIndex(hoverX);
            m_tempMouseEnterPress = true;
            m_temperatureCurveLabel->update();
        }
    }
    else if (obj == m_windCurveLabel) {
        if (event->type() == QEvent::Paint) {
            paintWindCurve();
        }
        else if (event->type() == QEvent::Enter) {
            m_windMouseEnterPress = true;
            QPoint windowPos = mapFromGlobal(QCursor::pos());
//            qDebug() << "BBBBBB" << windowPos << m_windCurveLabel->height() - m_btnsWidget->rect().height();
//            if (windowPos.x() > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            int middlePosX = calculateWindHoverIndex(windowPos.x());
            if (middlePosX <= 0) {
                if (m_windTip->isVisible()) {
                    m_windTip->setVisible(false);
                }
            }
            else {
                QPoint parentPos = mapToParent(QPoint(middlePosX, m_windCurveLabel->height()- AlignMent*2/* - TimeTextHeight*/));
                //qDebug() << "windowPos=" << windowPos << ",parentPos=" << parentPos;
                m_windTip->resetTipText("aaa112", "bbbbb");
                m_windTip->adjustSize();
                m_windTip->popupTip(parentPos);
            }
        }
        else if (event->type() == QEvent::Leave || event->type() == QEvent::HoverLeave) {
            m_windHoverIndex = -1;
            m_windMouseEnterPress = false;
            if (m_windTip->isVisible()) {
                m_windTip->setVisible(false);
            }
        }
        else if (event->type() == QEvent::HoverMove) {
            m_windMouseEnterPress = true;
            QPoint windowPos = mapFromGlobal(QCursor::pos());
//            if (windowPos.x() > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            int middlePosX = calculateWindHoverIndex(windowPos.x());
            if (middlePosX > 0) {
                QPoint parentPos = mapToParent(QPoint(middlePosX, m_windCurveLabel->height()- AlignMent*2/* - TimeTextHeight*/));
                m_windTip->resetTipText("aaa555", "bbbbb");
                m_windTip->adjustSize();
                m_windTip->popupTip(parentPos);
            }
        }
    }
    else if (obj == m_popLabel) {
        if (event->type() == QEvent::Paint) {
            paintPopWidget();
        }
        else if (event->type() == QEvent::Enter) {
            QPoint windowPos = mapFromGlobal(QCursor::pos());//将显示器坐标转换成窗口坐标
            int hoverX = windowPos.x();
//            if (hoverX > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            this->calculatePopHoverIndex(hoverX);
            m_popMouseEnterPress = true;
            m_popLabel->update();
        }
        else if (event->type() == QEvent::Leave) {
            m_popHoverIndex = -1;
            m_popMouseEnterPress = false;
            m_popLabel->update();
        }
        else if (event->type() == QEvent::HoverMove) {
            QPoint windowPos = mapFromGlobal(QCursor::pos());//将显示器坐标转换成窗口坐标
            int hoverX = windowPos.x();
//            if (hoverX > m_windCurveLabel->height() - m_btnsWidget->rect().height()) {//remove m_btnsWidget zone
//                return false;
//            }
            this->calculatePopHoverIndex(hoverX);
            m_popMouseEnterPress = true;
            m_popLabel->update();
        }
    }

    return QWidget::eventFilter(obj,event);
}

//24小时温度
void WeatherCategoryWidget::paintTemperatureCurve()
{
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0) {
        return;
    }

    m_tempX->clear();
    int temp[count];
//    Forecast forecast[8];
    for (int i=0;i<count;i++) {
        m_tempX->append(m_firstTempX + i*m_tempXSpace);
        temp[i] = m_preferences->m_hourlyWeather.at(i).tmp.toInt();
    }

    QPainter painter(m_temperatureCurveLabel);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int tempTotal = 0;
//    int temperatureArray[8] = {};
    int temperatureArray[count] = {};
    int minValue = 0;
    int maxValue = 0;
    int maxPos = 0;

    for (int i = 0; i < 8; i++) {
//        temperatureArray[i] = forecast[i].high.toInt();
        temperatureArray[i] = temp[i];
        tempTotal += temperatureArray[i];

        if (i==0) {
            minValue = temperatureArray[i];
            maxValue = temperatureArray[i];
        }

        if (temperatureArray[i] > maxValue) {
            maxValue = temperatureArray[i];
            maxPos = i;
        }
        if (temperatureArray[i] < minValue) {
            minValue = temperatureArray[i];
        }
    }

    int tempAverage = (int)(tempTotal / count); //当天各小时段温度的平均值
    int tempDiffMaxMin = abs(maxValue - minValue);//最高温和最低温的差
    int tempMiddle = maxValue - (int)(tempDiffMaxMin / 2);//最高温和最低温的中间值
    int basePosY = m_temperatureCurveLabel->height()/2 - TimeTextHeight/2;
    int unitHeight = (m_temperatureCurveLabel->height() - AlignMent*2 - TimeTextHeight)/tempDiffMaxMin;//每一摄氏度占据的高度,让显示曲线顶部和底部均空出AlignMent个像素

    int pointHY[count] = {0};
    for (int i = 0; i < count; i++) {
        if (temperatureArray[i] >= tempMiddle) {
            pointHY[i] = basePosY - abs(temperatureArray[i] - tempMiddle) * unitHeight;
        }
        else {
            pointHY[i] = basePosY + abs(tempMiddle - temperatureArray[i]) * unitHeight;
        }
    }

    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.save();
    pen.setColor(QColor("#fff222"));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor("#fff222"));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);

//    qDebug() << m_tempX->size();
//    foreach (int *point_X, m_tempX) {
//    }
//    for (int point_X : *m_tempX) {
//        qDebug() << "point_X=" << point_X;
//    }

    // 绘制第一个圆点
    if (m_tempMouseEnterPress && m_tempHoverIndex == 0) {
        painter.drawEllipse(QPoint(m_tempX->at(0), pointHY[0]), TemperatureDotSize+5, TemperatureDotSize+5);
    }
    else {
        painter.drawEllipse(QPoint(m_tempX->at(0), pointHY[0]), TemperatureDotSize, TemperatureDotSize);
    }
    //绘制其他圆点和圆点之间的连线
    for (int i = 0; i < count-1; i++) {
        if (m_tempMouseEnterPress) {
            if (m_tempHoverIndex == i+1) {
                painter.drawEllipse(QPoint(m_tempX->at(i+1), pointHY[i+1]), TemperatureDotSize+5, TemperatureDotSize+5);
            }
            else {
                painter.drawEllipse(QPoint(m_tempX->at(i+1), pointHY[i+1]), TemperatureDotSize, TemperatureDotSize);

            }
        }
        else {
            painter.drawEllipse(QPoint(m_tempX->at(i+1), pointHY[i+1]), TemperatureDotSize, TemperatureDotSize);
        }
        //painter.drawEllipse(QPoint(m_tempX[i+1], pointHY[i+1]), TemperatureDotSize, TemperatureDotSize);
        painter.drawLine(m_tempX->at(i), pointHY[i], m_tempX->at(i+1), pointHY[i+1]);
    }

    // 绘制每个圆点对应的温度值和时间点
    for (int i = 0; i < count; i++) {
        QFont font = painter.font();
        font.setPixelSize(12);
        QFontMetrics fm(font);
        painter.setFont(font);
        painter.setPen(QPen(QColor("#808080")));

        QString tempStr = QString::number(temperatureArray[i]) + "°C";
        if (pointHY[i] >= basePosY) {
            QRect valueRect(m_tempX->at(i) - fm.width(tempStr)/2, pointHY[i] - AlignMent - 5, fm.width(tempStr), TextHeight);
            painter.drawText(valueRect, Qt::AlignCenter, tempStr);
        }
        else {
            QRect valueRect(m_tempX->at(i) - fm.width(tempStr)/2, pointHY[i] + 5, fm.width(tempStr), TextHeight);
            painter.drawText(valueRect, Qt::AlignCenter, tempStr);
        }

        QString timeStr = m_preferences->m_hourlyWeather.at(i).time_hour;
        if (timeStr.contains(" ")) {
            QStringList timeList = timeStr.split(" ");
            QRect dateRect(m_tempX->at(i) - fm.width(timeList.at(0))/2, height()- AlignMent - TimeTextHeight, fm.width(timeList.at(0)), TimeTextHeight/2);
            painter.drawText(dateRect, Qt::AlignCenter, timeList.at(0));

            QRect timeRect(m_tempX->at(i) - fm.width(timeList.at(1))/2, height()- AlignMent - TimeTextHeight + 20, fm.width(timeList.at(1)), TimeTextHeight/2);
            painter.drawText(timeRect, Qt::AlignCenter, timeList.at(1));
        }
        else {
            QRect timeRect(m_tempX->at(i) - fm.width(timeStr)/2, height()- AlignMent - TimeTextHeight, fm.width(timeStr), TimeTextHeight);
            painter.drawText(timeRect, Qt::AlignCenter, timeStr);
        }
    }

    //平均温度线
    /*QPen penAve;
    penAve.setColor(Qt::red);
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);
    painter.setPen(penAve);
    int aveY = 0;
    if (tempAverage >= tempMiddle) {
        aveY = basePosY - abs(tempAverage - tempMiddle) * unitHeight;
    }
    else {
        aveY = basePosY + abs(tempMiddle - tempAverage) * unitHeight;
    }
    painter.drawLine(51, aveY, 750, aveY+1);
    QFont fontAve = painter.font();
    fontAve.setPixelSize(12);
    QFontMetrics fmAve(fontAve);
    painter.setFont(fontAve);
    painter.setPen(QPen(QColor("#808080")));
    QString aveStr = QString::number(tempAverage) + "°C";
    QRect aveRect(750 + 20, aveY - TextHeight/2, fmAve.width(aveStr), TextHeight);
    painter.drawText(aveRect, Qt::AlignCenter, aveStr);*/
    painter.restore();
}

//风力风向
void WeatherCategoryWidget::paintWindCurve()
{
//    1-2 	1-2级风(微风)
//    3-4 	3-4级风
//    4-5 	4-5级风
//    5-6 	5-6级风
//    6-7 	6-7级风
//    7-8 	7-8级风
//    8-9 	8-9级风
//    9-10 	9-10级风
//    10-11 	10-11级风
//    11-12 	11-12级风
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0)
        return;

    m_windX->clear();
    QStringList windList;
    for (int i=0;i<count;i++) {
        m_windX->append(m_firstWindX + i*m_windXSpace);
        windList.append(m_preferences->m_hourlyWeather.at(i).wind_sc);
    }

    const int levelCount = 11;
    int unitHeight = (m_temperatureCurveLabel->height() - AlignMent*2 - TimeTextHeight)/levelCount;//每个风级占据的高度,让显示曲线顶部和底部均空出AlignMent个像素
    int basePosY = AlignMent;
    m_windCount = windList.count();
    int pointHY[m_windCount] = {0};
    for (int i = 0; i < m_windCount; i++) {
        if (windList.at(i) == "1-2") {
            pointHY[i] = basePosY + 10*unitHeight;
        }
        else if (windList.at(i) == "3-4") {
            pointHY[i] = basePosY + 9*unitHeight;
        }
        else if (windList.at(i) == "4-5") {
            pointHY[i] = basePosY + 8*unitHeight;
        }
        else if (windList.at(i) == "5-6") {
            pointHY[i] = basePosY + 7*unitHeight;
        }
        else if (windList.at(i) == "6-7") {
            pointHY[i] = basePosY + 6*unitHeight;
        }
        else if (windList.at(i) == "7-8") {
            pointHY[i] = basePosY + 5*unitHeight;
        }
        else if (windList.at(i) == "8-9") {
            pointHY[i] = basePosY + 4*unitHeight;
        }
        else if (windList.at(i) == "9-10") {
            pointHY[i] = basePosY + 3*unitHeight;
        }
        else if (windList.at(i) == "10-11") {
            pointHY[i] = basePosY + 2*unitHeight;
        }
        else if (windList.at(i) == "11-12") {
            pointHY[i] = basePosY + 1*unitHeight;
        }
        else {
            pointHY[i] = basePosY + 10*unitHeight;
        }
    }

    const qreal ratio = qApp->devicePixelRatio();

    QPainter painter(m_windCurveLabel);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.save();
    pen.setColor(QColor("#fff222"));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor("#fff222"));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);

    // 绘制第一个箭头
    QPixmap icon = QPixmap(":/res/wind.png");
    QRect triangleRect(m_windX->at(0) - icon.width()/ratio/2, pointHY[0] - icon.height()/ratio/2, icon.width()/ratio, icon.height()/ratio);
    painter.drawPixmap(triangleRect, icon);

    //绘制其他箭头和箭头之间的连线
    for (int i = 0; i < m_windCount - 1; i++) {
        QRect otheraArowRect(m_windX->at(i+1) - int(icon.width()/ratio/2), pointHY[i+1] - icon.height()/ratio/2, icon.width()/ratio, icon.height()/ratio);
        painter.drawPixmap(otheraArowRect, icon);
        painter.drawLine(m_windX->at(i), pointHY[i], m_windX->at(i+1), pointHY[i+1]);
    }

    // 绘制每个箭头对应的时间点
    for (int i = 0; i < m_windCount; i++) {
        QFont font = painter.font();
        font.setPixelSize(12);
        QFontMetrics fm(font);
        painter.setFont(font);
        painter.setPen(QPen(QColor("#808080")));

        QString timeStr = m_preferences->m_hourlyWeather.at(i).time_hour;
        if (timeStr.contains(" ")) {
            QStringList timeList = timeStr.split(" ");
            QRect dateRect(m_windX->at(i) - fm.width(timeList.at(0))/2, height()- AlignMent - TimeTextHeight, fm.width(timeList.at(0)), TimeTextHeight/2);
            painter.drawText(dateRect, Qt::AlignCenter, timeList.at(0));

            QRect timeRect(m_windX->at(i) - fm.width(timeList.at(1))/2, height()- AlignMent - TimeTextHeight + 20, fm.width(timeList.at(1)), TimeTextHeight/2);
            painter.drawText(timeRect, Qt::AlignCenter, timeList.at(1));
        }
        else {
            QRect timeRect(m_windX->at(i) - fm.width(timeStr)/2, height()- AlignMent - TimeTextHeight, fm.width(timeStr), TimeTextHeight);
            painter.drawText(timeRect, Qt::AlignCenter, timeStr);
        }
    }

    painter.restore();
}

//降水量
void WeatherCategoryWidget::paintPopWidget()
{
    int count = m_preferences->m_hourlyWeather.size();
    if (count == 0)
        return;

    m_popX->clear();

    const qreal ratio = qApp->devicePixelRatio();

    QPainter painter(m_popLabel);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //test count == 0
    /*QRect rect(0, 0, width(), 48);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.drawText(rect, Qt::AlignCenter, tr("N/A"));*/

    //test
    m_popCount = count;
    int xPos = m_firstPopX;

    for(int i= 0; i<m_popCount; i++) {
        m_popX->append(xPos);

        QRect rect(xPos, 0, ((i == m_popHoverIndex) ? CurrentPopItemWidth : PopItemWidth), height()-2*AlignMent);
        if (m_popMouseEnterPress && m_popHoverIndex == i) {
            //background
            painter.fillRect(rect, QColor(0, 0, 0, 20));

            //border
            painter.setOpacity(1);
            QPainterPath borderPath;
            borderPath.addRoundedRect(rect, 0, 0);
            QPen pen(QColor("#0973b4"), 1);//Qt::transparent
            painter.setPen(pen);
            painter.drawPath(borderPath);
        }
        else {
            painter.fillRect(rect, QColor(0, 0, 0, 0));
        }

        QPointF center = rect.center();
//        center.setX(center.x()/2);
//        center.setY(center.y()/2);

        painter.setRenderHint(QPainter::Antialiasing, false);
        QFont curFont = QGuiApplication::font();//painter.font();
        curFont.setPixelSize(14);//font.setPointSize(textSize);
        painter.setFont(curFont);
        painter.setPen(QPen(QColor("#000000")));
        painter.save();

        QFontMetrics fm(curFont);
        QString text = m_preferences->m_hourlyWeather.at(i).time_hour;
        if (text.contains(" ")) {
            QStringList textList = text.split(" ");
            QRect dateRect(center.x()- fm.width(text)/2, /*AlignMent*/5, fm.width(text), 28);
            painter.drawText(dateRect, Qt::AlignCenter, textList.at(0));

            QRect timeRect(center.x()- fm.width(text)/2, /*AlignMent*/5 + 20, fm.width(text), 28);
            painter.drawText(timeRect, Qt::AlignCenter, textList.at(1));
            painter.restore();
        }
        else {
            QRect textRect(center.x()- fm.width(text)/2, AlignMent, fm.width(text), 28);
            painter.drawText(textRect, Qt::AlignCenter, text);
            painter.restore();
        }

        painter.setRenderHint(QPainter::Antialiasing, true);
        QPixmap pixmap = QPixmap(":/res/101.png");
        pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect iconRect(center.x()-pixmap.width()/ratio/2, 50/*textRect.bottom()*/, pixmap.width() / ratio, pixmap.height() / ratio);
        painter.drawPixmap(iconRect, pixmap);

        painter.save();
        QString precipitationStr = m_preferences->m_hourlyWeather.at(i).pop + "%";//"0mm"
        QRect weekArea(center.x()- fm.width(precipitationStr)/2, iconRect.bottom(), fm.width(precipitationStr), 28);
        painter.drawText(weekArea, Qt::AlignCenter, precipitationStr);
        painter.restore();

        xPos += rect.width();
    }
}

void WeatherCategoryWidget::initTemperatureWidget()
{
    m_temperatureCurveLabel = new QLabel;
    m_temperatureCurveLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_temperatureCurveLabel->installEventFilter(this);
    m_temperatureCurveLabel->setMouseTracking(true);
    m_temperatureCurveLabel->setAttribute(Qt::WA_Hover, true);
//    m_temperatureCurveLabel->setStyleSheet("QLabel { background-color : transparent; color : #666666; }");
    m_temperatureCurveLabel->setStyleSheet("QLabel {background-color:rgba(63,63,63,20%); border: 0px; color : #666666; }");

    //test background
//    m_temperatureCurveLabel->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
//    m_temperatureCurveLabel->setPalette(palette);


    const int idx = m_stackedLayout->addWidget(m_temperatureCurveLabel);
    m_stackedLayout->setCurrentIndex(idx);
}

void WeatherCategoryWidget::initWindWidget()
{
    //TODO:跟温度一样绘制曲线，图标处绘制图片wind.png,设置MouseEnter和MouseLeave，鼠标在某个区域范围内（x固定了），显示对应的tooptip
    m_windCurveLabel = new QLabel;
    m_windCurveLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_windCurveLabel->installEventFilter(this);
    m_windCurveLabel->setMouseTracking(true);
    m_windCurveLabel->setAttribute(Qt::WA_Hover, true);
    m_windCurveLabel->setStyleSheet("QLabel {background-color:rgba(63,63,63,20%); border: 0px; color : #666666; }");

    //test background
//    m_windCurveLabel->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
//    m_windCurveLabel->setPalette(palette);

    m_stackedLayout->addWidget(m_windCurveLabel);

    m_windTip = new WindTooltip(this->parentWidget());
    m_windTip->setFixedHeight(60);
    m_windTip->setMinimumWidth(100);
    m_windTip->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_windTip->hide();
}

void WeatherCategoryWidget::initPopWidget()
{
    m_popLabel = new QLabel;
    m_popLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_popLabel->installEventFilter(this);
    m_popLabel->setMouseTracking(true);
    m_popLabel->setAttribute(Qt::WA_Hover, true);
    m_popLabel->setStyleSheet("QLabel {background-color:rgba(63,63,63,20%); border: 0px; color : #666666; }");

    //test background
//    m_popLabel->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
//    m_popLabel->setPalette(palette);

    m_stackedLayout->addWidget(m_popLabel);
}

void WeatherCategoryWidget::initLifeStyleWidget()
{
    m_lifestyleWidget = new QWidget;
    m_lifestyleWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_lifestyleWidget->setMouseTracking(true);
    m_lifestyleWidget->setAttribute(Qt::WA_Hover, true);
    //test background
    /*m_lifestyleWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(Qt::blue));
    m_lifestyleWidget->setPalette(palette);
    //m_lifestyleWidget->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:rgba(0,0,0,0.2);}");
    */
    m_lifestyleWidget->setStyleSheet("QWidget{border-radius: 5px; color:rgb(250, 250, 250); background-color:rgba(63,63,63,20%); border: 0px;}");

    //test background
//    m_lifestyleWidget->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Window, QBrush(Qt::gray));
//    m_lifestyleWidget->setPalette(palette);

    m_indexGridLayout = new QGridLayout(m_lifestyleWidget);
    m_indexGridLayout->setSpacing(1);
    m_indexGridLayout->setContentsMargins(0,0,0,30);

    this->refershLifeIndexGridLayout();

    m_stackedLayout->addWidget(m_lifestyleWidget);
//    showLifeStyleIndex(QString());
}

void WeatherCategoryWidget::refershLifeIndexGridLayout()
{
    while (QLayoutItem *item = m_indexGridLayout->takeAt(0)) {
        item->widget()->deleteLater();
        delete item;
    }

    for(int i=0; i<m_lifestyleTips.count(); i++) {
        TextTip *tip = m_lifestyleTips.at(i);
        delete tip;
        tip = NULL;
    }
    m_lifestyleTips.clear();

    int index = 0;
    const int count = m_lifeIndexList.size();
    for (int i = 0; i != count; ++i, ++index) {
        IndexItemWidget *item = new IndexItemWidget(m_lifeIndexList[i], m_lifeIndexIconList[i]);
        connect(item, SIGNAL(requestShowMsg(QString)), this, SLOT(showLifeStyleIndex(QString)));
        m_indexGridLayout->addWidget(item, index / 3, index % 3);
        TextTip *tip = this->setTipWidget(item, "");
        m_lifestyleTips.append(tip);
        m_lifeItems.append(item);
    }
}

void WeatherCategoryWidget::showLifeStyleIndex(const QString &name)
{

}

void WeatherCategoryWidget::refreshLifestyleData()
{
    int n = 0;
    if (m_lifeItems.count() == 8 && m_lifestyleTips.count() == 8) {
        //舒适度指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.comf_brf, m_preferences->m_lifestyle.comf_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.comf_txt);

        //穿衣指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.drsg_brf, m_preferences->m_lifestyle.drsg_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.drsg_txt);

        //感冒指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.flu_brf, m_preferences->m_lifestyle.flu_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.flu_txt);

        //运动指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.sport_brf, m_preferences->m_lifestyle.sport_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.sport_txt);

        //旅游指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.trav_brf, m_preferences->m_lifestyle.trav_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.trav_txt);

        //紫外线指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.uv_brf, m_preferences->m_lifestyle.uv_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.uv_txt);

        //洗车指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.cw_brf, m_preferences->m_lifestyle.cw_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.cw_txt);

        //空气污染扩散条件指数
        m_lifeItems[n]->refreshLifeStyle(m_preferences->m_lifestyle.air_brf, m_preferences->m_lifestyle.air_txt);
        m_lifestyleTips[n++]->resetTipText(m_preferences->m_lifestyle.air_txt);
    }
}

TextTip *WeatherCategoryWidget::setTipWidget(QWidget *w, const QString &txt)
{
    TextTip *tip = new TextTip(txt, TrianglePostion::TopLeft, this);
    w->setProperty("TextTipWidget", QVariant::fromValue<QWidget *>(tip));
    w->installEventFilter(m_tipModule);

    return tip;
}

void WeatherCategoryWidget::setDayStyleSheets()
{
    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        item->setDayStyleSheets();
    }
}

void WeatherCategoryWidget::setNightStyleSheets()
{
    for(int i=0; i<m_lifeItems.count(); i++) {
        IndexItemWidget *item = m_lifeItems.at(i);
        item->setNightStyleSheets();
    }
}

void WeatherCategoryWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    m_btnsWidget->setFixedWidth(event->size().width());
    m_btnsWidget->move(0, event->size().height() - m_btnsWidget->height());
}

void WeatherCategoryWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);
    m_tempMouseEnterPress = true;
    m_temperatureCurveLabel->update();
}

/*cvoid WeatherCategoryWidget::enterEvent(QEvent *event)
{
    QPoint windowPos = mapFromGlobal(QCursor::pos());//将显示器坐标转换成窗口坐标
    int hoverX = windowPos.x();
    this->calculateTemperatureHoverIndex(hoverX);
    m_tempMouseEnterPress = true;
    m_temperatureCurveLabel->update();

    event->accept();
}

void WeatherCategoryWidget::leaveEvent(QEvent *event)
{
    m_tempMouseEnterPress = false;
    m_tempHoverIndex = -1;
    m_temperatureCurveLabel->update();

    event->accept();
}*/

//void WeatherCategoryWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    QFrame::mouseMoveEvent(event);
//    qDebug() << "event->pos()" << event->pos();
//    QCursor::setPos(rect().center() + pos());
//}
