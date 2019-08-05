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

#include "windtooltip.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

WindTooltip::WindTooltip(QWidget* parent) : QLabel(parent)
  , m_trianglePostion(TopMiddle)
  , m_borderRadius(2)
{
    this->setAlignment(Qt::AlignLeft);
}

WindTooltip::~WindTooltip()
{

}

void WindTooltip::resetTipText(const QString &title, const QString &desc)
{
    this->m_text = title;
    this->m_desc = desc;
}

void WindTooltip::setTrianglePosition(TrianglePostion position)
{
    m_trianglePostion = position;
}

void WindTooltip::popupTip(const QPoint &point)
{
    //TopMiddle
    if (point.x() - this->width() / 2 < 0) {
        this->move(0, point.y());
    }
    else {
        this->move(point.x() - this->width() / 2, point.y());
    }
    //BottomMiddle
    //this->move(point.x() - this->width() / 2, point.y() - this->height());
    this->show();

    QGraphicsOpacityEffect *m_opacity = new QGraphicsOpacityEffect(this);
    m_opacity->setOpacity(1);
    this->setGraphicsEffect(m_opacity);
    QPropertyAnimation *animation = new QPropertyAnimation(m_opacity, "opacity");
    animation->setDuration(5000);
    animation->setStartValue(0);
    animation->setKeyValueAt(0.25, 1);
    animation->setKeyValueAt(0.5, 1);
    animation->setKeyValueAt(0.75, 1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, [=] {
        animation->deleteLater();
        this->setGraphicsEffect(NULL);
        this->hide();
    });
}

void WindTooltip::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);

    const int triangleHeight = 6;

    QPainterPath path;
    if (m_trianglePostion == TopMiddle) {
        // draw triangle
        path.moveTo(this->width()/2-triangleHeight, triangleHeight);

        path.lineTo(this->width()/2-triangleHeight, triangleHeight);
        path.lineTo(this->width()/2, 0);
        path.lineTo(this->width()/2 + triangleHeight, triangleHeight);

        path.lineTo(this->width() - m_borderRadius, triangleHeight);
        path.arcTo(QRectF(QPointF(this->width(), triangleHeight), QPointF(this->width() - 2 * m_borderRadius, 2 * m_borderRadius + triangleHeight)), 90.0, 90.0);//以90度为起点，逆时针画90度

        path.lineTo(this->width(), this->height() - triangleHeight - m_borderRadius);
        path.arcTo(QRectF(QPointF(this->width(), this->height()), QPointF(this->width() - 2 * m_borderRadius, this->height() - 2 * m_borderRadius)), 180.0, -90.0);//以180度为起点，顺时针画90度

        path.arcTo(QRectF(QPointF(0, this->height() - 2 * m_borderRadius), QPointF(2 * m_borderRadius, this->height())), 270.0, -90.0);//以270度为起点，顺时针画90度
        path.lineTo(0.0, m_borderRadius + triangleHeight);

        path.arcTo(QRectF(QPointF(0, triangleHeight), QPointF(2 * m_borderRadius, 2 * m_borderRadius + triangleHeight)), 180.0, -90.0);//以180度为起点，顺时针画90度
        path.lineTo(m_borderRadius, triangleHeight);

        path.closeSubpath();
        painter.fillPath(path, QBrush(Qt::white));
    }
    else {
        path.moveTo(this->width(), m_borderRadius);
        path.arcTo(this->width() - m_borderRadius*2, 0, m_borderRadius*2, m_borderRadius*2, 0.0, 90.0);
        path.lineTo(m_borderRadius, 0);

        path.arcTo(0, 0, m_borderRadius*2, m_borderRadius*2, 90.0, 90.0);
        path.lineTo(0, this->height() - m_borderRadius - triangleHeight);

        path.arcTo(0, this->height() - m_borderRadius*2 - triangleHeight, m_borderRadius*2, m_borderRadius*2, 180.0, 90.0);
        path.lineTo(this->width()/2 - triangleHeight, this->height() - triangleHeight);

        // draw triangle
        path.lineTo(this->width()/2, this->height());
        path.lineTo(this->width()/2 + triangleHeight, this->height() - triangleHeight);
        path.lineTo(this->width() - m_borderRadius, this->height() - triangleHeight);
        path.arcTo(this->width() - m_borderRadius*2,
                            this->height() - m_borderRadius*2 - triangleHeight,
                            m_borderRadius*2, m_borderRadius*2, 270.0, 90.0);
        path.closeSubpath();
        painter.fillPath(path, QBrush(Qt::white));
    }


    QPointF titleCenter = this->rect().center();
    titleCenter.setX(5);
    titleCenter.setY(titleCenter.y()/2);

    QFont font;
    font.setPixelSize(12);
    const QFontMetrics fm(font);//QFontMetrics fm = painter.fontMetrics();
    const int titleLength = fm.width(this->m_text);
    painter.setPen(QPen(Qt::black));
    painter.setFont(font);
    QRect titleRect = QRect(titleCenter.x(), titleCenter.y(), titleLength, 22);
    painter.drawText(titleRect, Qt::AlignLeft, this->m_text);

    QPointF descCenter = this->rect().center();
    descCenter.setX(5);
    descCenter.setY(descCenter.y()*3/2);
    const int descLength = fm.width(this->m_desc);
    painter.drawText(QRect(descCenter.x(), descCenter.y(), descLength, 22), Qt::AlignLeft, this->m_desc);
}
