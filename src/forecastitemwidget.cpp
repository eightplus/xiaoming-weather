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

#include "forecastitemwidget.h"
//#include "tooltip.h"

#include <QDebug>
#include <QEvent>
#include <QCursor>
#include <QPainter>

inline QString covertDateToWeek(QString dateStr)
{
    //星期使用基姆拉尔森计算公式
    if (dateStr.contains(QChar('-'))) {
        QStringList dateList= dateStr.split(QChar('-'));
        if (dateList.length() == 3) {
            bool ok;
            int year = dateList.at(0).toInt(&ok, 10);
            int month = dateList.at(1).toInt(&ok, 10);
            int day = dateList.at(2).toInt(&ok, 10);

            int week = (day + 2*month + 3*(month+1)/5 + year + year/4 - year/100 + year/400)%7;
            switch (week) {
            case 0:
                return QString(QObject::tr("Monday"));//星期一
                break;
            case 1:
                return QString(QObject::tr("Tuesday"));//星期二
                break;
            case 2:
                return QString(QObject::tr("Wednesday"));//星期三
                break;
            case 3:
                return QString(QObject::tr("Thursday"));//星期四
                break;
            case 4:
                return QString(QObject::tr("Friday"));//星期五
                break;
            case 5:
                return QString(QObject::tr("Saturday"));//星期六
                break;
            case 6:
                return QString(QObject::tr("Sunday"));//星期日
                break;
            default:
                break;
            }
        }
    }

    return "--";
}

ForecastItemWidget::ForecastItemWidget(QWidget *parent) :
    QWidget(parent)
    , m_mouseHover(false)
    , m_radius(1)
    , m_borderColor(QColor(224,224,224,130))
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

//    m_toolTip = new ToolTip();

    this->setDefaultData();
}

void ForecastItemWidget::resetForecastData(const ForecastWeather &data, int index)
{
    if (index == 0) {
        m_weekLabel->setText(tr("Today"));
    }
    else {
        m_weekLabel->setText(covertDateToWeek(data.forcast_date));
    }

    m_dateLabel->setText(data.forcast_date);
    m_weatherLabel->setText(data.cond_txt_d);

    //darkgrey or lightgrey
    QPixmap pixmap;
    pixmap = QPixmap(":/res/101.png");
//    if (m_isDayOrNight) {
//        pixmap = QPixmap(QString(":/res/weather_icons/darkgrey/%1.png").arg(data.cond_code_d));
//    }
//    else {
//        pixmap = QPixmap(QString(":/res/weather_icons/lightgrey/%1.png").arg(data.cond_code_d));
//    }
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    m_tempLabel->setText(QString("%1°C~%2°C").arg(data.tmp_min).arg(data.tmp_max));

//    m_toolTip->resetData(data, m_weekLabel->text());
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
//    m_weekLabel->setText("星期五");
}

void ForecastItemWidget::setDefaultData()
{
    m_weekLabel->setText("-");
    m_dateLabel->setText("-");
    m_weatherLabel->setText("-");
    QPixmap pixmap = QPixmap(":/res/101.png");
    pixmap = pixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_iconLabel->setPixmap(pixmap);
    m_tempLabel->setText("-°C");
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
//    if (event->type() == QEvent::ToolTip) {
//        m_toolTip->popupTip(QCursor::pos());
//    }
//    else if (event->type() == QEvent::Leave) {
//        m_toolTip->hide();
//    }

    return QWidget::event(event);
}

void ForecastItemWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    this->m_mouseHover = true;

    update();
}

void ForecastItemWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    this->m_mouseHover = false;

    update();
}

void ForecastItemWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    // draw background
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, m_mouseHover ? 20 : 0));

    // draw border
    if (m_mouseHover) {
        QRect borderRect;
        borderRect.setRect(this->rect().x(), this->rect().y(), this->rect().width() - 1, this->rect().height() - 1);
        QPainterPath outBorderPath;
        outBorderPath.addRoundedRect(borderRect, m_radius, m_radius);
        QPen pen(m_borderColor, 1);
        painter.setPen(pen);
        painter.drawPath(outBorderPath);
    }
}
