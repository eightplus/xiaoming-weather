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

#include "aqitooltip.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

AqiToolTip::AqiToolTip(QWidget *parent) :
    QFrame(parent)
    , m_radius(0)
    , m_background(QBrush(QColor(255,255,255,230)))
    , m_borderColor(QColor(224,224,224,130))
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);

    m_frame = new QFrame();
    m_frame->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *layout = new QVBoxLayout(this->m_frame);
    layout->setContentsMargins(15, 5, 15, 5);
    layout->setSpacing(5);

    m_aqiLabel = new QLabel;
    m_qltyLabel = new QLabel;
    m_mainLabel = new QLabel;
    m_pm25Label = new QLabel;
    m_pm10Label = new QLabel;
    m_no2Label = new QLabel;
    m_so2Label = new QLabel;
    m_coLabel = new QLabel;
    m_o3Label = new QLabel;
    m_aqiLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_qltyLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_mainLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_pm25Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_pm10Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_no2Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_so2Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_coLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");
    m_o3Label->setStyleSheet("QLabel{border:none;background-color:transparent;color:#ffffff;font-size:12px;}");

    layout->addWidget(m_aqiLabel, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(m_qltyLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_mainLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_pm25Label, 0, Qt::AlignLeft);
    layout->addWidget(m_pm10Label, 0, Qt::AlignLeft);
    layout->addWidget(m_no2Label, 0, Qt::AlignLeft);
    layout->addWidget(m_so2Label, 0, Qt::AlignLeft);
    layout->addWidget(m_coLabel, 0, Qt::AlignLeft);
    layout->addWidget(m_o3Label, 0, Qt::AlignLeft);

    m_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    adjustSize();

    this->resetData();
}

AqiToolTip::~AqiToolTip()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

void AqiToolTip::resetData()
{
    m_aqiLabel->setText(QString(tr("aqi:%1")).arg("45"));//空气质量指数
    m_qltyLabel->setText(QString(tr("qlty:%1")).arg("优"));//空气质量
    m_mainLabel->setText(QString(tr("main:%1")).arg("pm10"));//主要污染物
    m_pm25Label->setText(QString(tr("pm25:%1")).arg("10"));
    m_pm10Label->setText(QString(tr("pm10:%1")).arg("25"));
    m_no2Label->setText(QString(tr("no2:%1")).arg("12"));//二氧化氮
    m_so2Label->setText(QString(tr("so2:%1")).arg("13"));//二氧化硫
    m_coLabel->setText(QString(tr("co:%1")).arg("14"));//一氧化碳
    m_o3Label->setText(QString(tr("o3:%1")).arg("5"));//臭氧
}

void AqiToolTip::popupTip(QPoint point)
{
    this->move(QPoint(point.rx(), point.ry()));
    this->setStyleSheet("QFrame{border:none;background-color:rgba(6,54,56,85%);color:rgb(255,255,255);}");
    QFrame::show();

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this->m_frame);
    opacityEffect->setOpacity(1);
    this->m_frame->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *animation = new QPropertyAnimation(opacityEffect, "opacity");
    animation->setDuration(6000);
    animation->setStartValue(0);
    animation->setKeyValueAt(0.25, 1);
    animation->setKeyValueAt(0.5, 1);
    animation->setKeyValueAt(0.75, 1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, [=] {
        animation->deleteLater();
        this->m_frame->setGraphicsEffect(NULL);
        this->hide();
    });
}

//void AqiToolTip::paintEvent(QPaintEvent *event)
//{
//    QPainter painter(this);
//    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

//    QBrush background =  this->m_background;
//    QColor borderColor = this->m_borderColor;

//    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
//    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
//    QPainterPath bgPath;
//    bgPath.addRoundedRect(bgRect, this->m_radius, this->m_radius);
//    painter.fillPath(bgPath, background);

//    QPainterPath borderPath;
//    QRectF borderRect = QRectF(rect());
//    int borderRadius = this->m_radius;
//    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
//    borderRadius += 0.5;
//    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
//    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

//    QPen borderPen(borderColor);
//    borderPen.setWidthF(1.0);
//    painter.strokePath(borderPath, borderPen);
//}
