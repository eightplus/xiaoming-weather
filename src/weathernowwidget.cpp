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

#include "weathernowwidget.h"
#include "imagebutton.h"
#include "aqitooltip.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

namespace {

    const int itemHeight = 40;
    const int weatherTopOffset = 170;
    const int dateLineHeight = 46;
    const int dateTopOffset = 10;
    const int locationIconSize = 24;

}

WeatherNowWidget::WeatherNowWidget(QWidget *parent)
    : QFrame(parent)
    , m_timeInterval(0)
    , m_refreshTimer(new QTimer(this))
{
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover, true);
    this->setStyleSheet("QFrame{border-radius: 5px; color:rgb(250, 250, 250); background-color:argb(60, 60, 60, 130);}");
    this->installEventFilter(this);

    m_aqiToolTip = new AqiToolTip();

    m_locationBtn = new ImageButton(this);
    m_locationBtn->setFixedSize(locationIconSize, locationIconSize);
    m_locationBtn->setObjectName("LocationBtn");
    m_locationBtn->setToolTip(tr("City Setting"));
    m_locationBtn->setVisible(false);
    connect(m_locationBtn, &ImageButton::clicked, this, &WeatherNowWidget::locationBtnClicked);

    m_refreshTimer->setInterval(60 * 1000);
    m_refreshTimer->setSingleShot(false);

    connect(m_refreshTimer, &QTimer::timeout, this, [=] {
        const int elapsed = m_time.elapsed();
        this->m_timeInterval = elapsed / (1000 * 60);
    });
}

WeatherNowWidget::~WeatherNowWidget()
{
    if (this->m_refreshTimer) {
        if(this->m_refreshTimer->isActive()) {
            this->m_refreshTimer->stop();
        }
        delete this->m_refreshTimer;
        this->m_refreshTimer = nullptr;
    }
}

void WeatherNowWidget::refreshNowWeatherData()
{
    if (m_refreshTimer->isActive())
        m_refreshTimer->stop();
    m_refreshTimer->start();
    m_time.restart();

    this->m_timeInterval = 0;
    m_locationBtn->setVisible(false);

    this->update();
}

void WeatherNowWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    bool show = event->y() < 60 && event->y() > 0;
    if (show) {
        m_locationBtn->move(width() - locationIconSize, dateTopOffset);
    }
    m_locationBtn->setVisible(show);

    this->update();
}

bool WeatherNowWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Enter) {
        //m_aqiToolTip->popupTip(QCursor::pos());
        QPoint mousePos = mapToParent(mapFromGlobal(QCursor::pos()));
        QRect eignoreDateRect(0 ,0, this->width(), dateLineHeight);
        if (!eignoreDateRect.contains(mousePos)) {
            m_aqiToolTip->popupTip(this->mapToGlobal(QPoint(this->rect().width(), this->rect().y())));
        }
        //qDebug() << "mousePos=" << mousePos << eignoreDateRect;
    }
    else if (event->type() == QEvent::Leave || event->type() == QEvent::HoverLeave) {
        if (m_aqiToolTip->isVisible())
            m_aqiToolTip->hide();
    }
    /*else if (event->type() == QEvent::HoverEnter) {
        if (!m_aqiToolTip->isVisible()) {
            QPoint mousePos = mapToParent(mapFromGlobal(QCursor::pos()));
            QRect eignoreDateRect(0 ,0, this->width(), dateLineHeight);
            if (!eignoreDateRect.contains(mousePos)) {
                m_aqiToolTip->popupTip(this->mapToGlobal(QPoint(this->rect().width(), this->rect().y())));
            }
        }
    }*/
    else if (event->type() == QEvent::MouseButtonPress) {
        if (m_aqiToolTip->isVisible())
            m_aqiToolTip->hide();
    }

    return QWidget::eventFilter(obj,event);
}

void WeatherNowWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    QColor color(Qt::white);
    painter.setPen(color);

    const QPointF center = this->rect().center();
    const qreal ratio = qApp->devicePixelRatio();

    QRect dateRect(5 ,0, this->width()- 5*2, dateLineHeight);
    dateRect.moveTop(dateTopOffset);
    painter.drawText(dateRect, Qt::AlignLeft, tr("08月12日 周日 农历七月初二"));

    painter.setRenderHint(QPainter::Antialiasing, true);
    QPixmap icon = QPixmap(":/res/101.png");
    QRect iconRect(center.x() - icon.width()/ratio/2, dateRect.height() + 10, icon.width()/ratio, icon.height()/ratio);
    painter.drawPixmap(iconRect, icon);

    painter.setRenderHint(QPainter::Antialiasing, false);

    QFont m_currentFont = QGuiApplication::font();
    QFont font = painter.font();
    font.setPointSize(m_currentFont.pointSize()*4.0);
    QFontMetrics fm(font);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);

    const int temperature = 37;
    QString tempText = QString("%1").arg(temperature);

    QRect nowTempRect(30, dateRect.bottom(), fm.width(tempText), 60);
    painter.drawText(nowTempRect, Qt::AlignTop | Qt::AlignRight, tempText);
    font.setPointSize(m_currentFont.pointSize()*0.85);
    painter.setFont(font);
    QString unitText = "°C";
    painter.drawText(QRect(nowTempRect.right(), nowTempRect.top(), fm.width(unitText), 20), Qt::AlignTop | Qt::AlignLeft, unitText);

    /*const int rightOffset = 10;
    painter.drawText(QRect(center.x()-fm.width(tempText) + rightOffset, iconRect.bottom(), fm.width(tempText), 50), Qt::AlignTop | Qt::AlignRight, tempText);
    font.setPointSize(m_currentFont.pointSize()*0.85);
    painter.setFont(font);
    QString unitText = "°C";
    painter.drawText(QRect(center.x() + rightOffset, iconRect.bottom() + 5, fm.width(unitText), 20), Qt::AlignTop | Qt::AlignLeft, unitText);
    */

    font.setPointSize(m_currentFont.pointSize()*1.2);
    painter.setFont(font);

    QRect weatherRect(0, 0, width(), itemHeight);
    weatherRect.moveTop(weatherTopOffset);
    painter.drawText(weatherRect, Qt::AlignCenter, "多云");

    QRect tempRect(0, weatherRect.bottom(), width(), itemHeight);
    painter.drawText(tempRect, Qt::AlignCenter, "28 ~ 36℃");

    QRect windRect(0, tempRect.bottom(), width(), itemHeight);
    painter.drawText(windRect, Qt::AlignCenter, "东北风微风");

    QRect aqiRect(width()/2 - 40, windRect.bottom(), 40*2, itemHeight);
    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(aqiRect).marginsRemoved(shadowMargins);
    QPainterPath aqiPath;
    aqiPath.addRoundedRect(bgRect, 5, 5);
    painter.fillPath(aqiPath, QBrush(QColor(0,0,0,120)));
    QPixmap pixmap = QPixmap(":/res/aqi.png");
    QRect aqiIconRect(aqiRect.x() + 5, aqiRect.center().y() -pixmap.height()/2, pixmap.width(), pixmap.height());
    painter.drawPixmap(aqiIconRect, pixmap);
    painter.drawText(aqiRect, Qt::AlignCenter, "70 良");

    if (!m_locationBtn->isVisible()) {
        fm = QFontMetrics(font);
        QString currentCity = "长沙";
        QRect cityRect(width() - fm.width(currentCity) - 10, dateRect.y(), fm.width(currentCity), dateLineHeight);
        painter.drawText(cityRect, Qt::AlignTop | Qt::AlignRight, fm.elidedText(currentCity, Qt::ElideRight, cityRect.width()));
    }

    font.setPointSize(m_currentFont.pointSize() * 0.8);
    fm = QFontMetrics(font);
    QString updateTime = (this->m_timeInterval == 0 ? tr("Just updated") : QString(tr("%1 minutes ago")).arg(this->m_timeInterval));
    QRect statusRect(iconRect.right(), iconRect.center().y(), fm.width(updateTime), iconRect.height()/2);
    color.setAlphaF(0.5);
    painter.setPen(color);
    painter.setFont(font);
    painter.drawText(statusRect, Qt::AlignTop|Qt::AlignRight, updateTime);
}
