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

#include "forecastitemwidget.h"
#include "forecasttip.h"

#include <QDebug>
#include <QEvent>
#include <QCursor>
#include <QPainter>
#include <QDateTime>

ForecastItemWidget::ForecastItemWidget(QWidget *parent) :
    QWidget(parent)
    , m_isHover(false)
{
    this->setFixedSize(100, 140);//140:initForecastWidget's height - 2*space = 160- 10*2
    this->setStyleSheet("QWidget{border-radius: 0px;color:rgb(250,250,250);background-color:rgba(0,0,0,0.2)}");
//    setAttribute(Qt::WA_TransparentForMouseEvents);

    m_weekLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_weatherLabel = new QLabel(this);
    m_iconLabel = new QLabel(this);
    m_tempLabel = new QLabel(this);
    m_iconLabel->setFixedSize(48, 48);
    m_iconLabel->setStyleSheet("QLabel{border:none;background-color:transparent;}");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(5);
    m_layout->addWidget(m_weekLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    m_layout->addWidget(m_dateLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_weatherLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_tempLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    m_toolTip = new ForecastTip();

    this->setDayStyleSheets();
    this->setDefaultData();
}

void ForecastItemWidget::resetForecastData(const ForecastWeather &data, int index)
{
    if (index == 0) {
        m_weekLabel->setText(tr("Today"));
    }
    else {
        if (data.forcast_date.isEmpty()) {
            m_weekLabel->setText("--");
        }
        else {
            QDateTime dt = QDateTime::fromString(data.forcast_date,"yyyy-MM-dd");
    //        QDateTime dt;
    //        dt.setTime_t(str.toInt());
            QDate m_date = dt.date();//qDebug() << QDate::currentDate().toString("ddd");
            m_weekLabel->setText(m_date.toString("ddd"));
        }
    }

    m_dateLabel->setText(data.forcast_date);
    m_weatherLabel->setText(data.cond_txt_d);

    //darkgrey or lightgrey
    QPixmap pixmap;
    if (m_isDayOrNight) {
        pixmap = QPixmap(QString(":/res/weather_icons/darkgrey/%1.png").arg(data.cond_code_d));
    }
    else {
        pixmap = QPixmap(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code_d));
    }
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    m_tempLabel->setText(QString("%1°C~%2°C").arg(data.tmp_min).arg(data.tmp_max));

    m_toolTip->resetData(data, m_weekLabel->text());
}

void ForecastItemWidget::setDayStyleSheets()
{
    m_isDayOrNight = true;

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080;font-size:12px;}");
}

void ForecastItemWidget::setNightStyleSheets()
{
    m_isDayOrNight = false;

    m_weekLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_dateLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_weatherLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
    m_tempLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#d9d9d9;font-size:12px;}");
}

void ForecastItemWidget::setTextData()
{
}

void ForecastItemWidget::setDefaultData()
{
    m_weekLabel->setText("今天");
    m_dateLabel->setText("20180919");
    m_weatherLabel->setText("晴");
    QPixmap pixmap = QPixmap(":/res/101.png");
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
//    m_tempLabel->setText("-°C");
    m_tempLabel->setText("27°C");
}

void ForecastItemWidget::setLabelText(const QString &name, const QString &desc)
{
//    QFont ft;
//    QFontMetrics fm(ft);
//    QString elided_text = fm.elidedText(name, Qt::ElideRight, 260);
//    name_label->setText(elided_text);
//    elided_text = fm.elidedText(desc, Qt::ElideRight, 260);
//    description_label->setText(elided_text);
//    if(elided_text.endsWith("…"))
//        description_label->setToolTip(desc);
}

bool ForecastItemWidget::event(QEvent *event)
{
    if (event->type() == QEvent::Enter) {//QEvent::ToolTip
        m_isHover = true;
        m_toolTip->popupTip(QCursor::pos());
    }
    else if (event->type() == QEvent::Leave) {
        m_isHover = false;
        m_toolTip->hide();
    }

    return QWidget::event(event);
}

void ForecastItemWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (m_isHover) {
        QPen pen(Qt::NoBrush, 1);
        painter.setPen(pen);

        QLinearGradient lineargradient(this->rect().topLeft(), this->rect().bottomLeft());
        //lineargradient.setColorAt(0, QColor(255, 255, 255, 40));
        lineargradient.setColorAt(0, QColor(230, 230, 230, 150));
        lineargradient.setColorAt(0.5, QColor(230, 230, 230, 100));
        lineargradient.setColorAt(1, QColor(230, 230, 230, 0));
        QBrush brush(lineargradient);
        painter.setBrush(brush);//painter.setBrush(lineargradient);
        painter.drawRect(rect());//painter.drawRoundedRect(this->rect(), 1, 1);
    }

    QWidget::paintEvent(event);
}
