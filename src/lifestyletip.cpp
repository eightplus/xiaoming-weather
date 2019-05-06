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

#include "lifestyletip.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QDebug>

LifestyleTip::LifestyleTip(const QString &txt, QWidget *parent) : QFrame(parent)
  , m_radius(1)
  , m_background(QBrush(QColor(255,255,255,255)))
  , m_borderColor(QColor(224,224,224,130))
  , m_trianglePostion(TopLeft)
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(260, 56);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    m_frame = new QFrame();
    m_frame->setContentsMargins(0, 0, 0, 0);

    m_textLabel = new QLabel;
    m_textLabel->setWordWrap(true);//QLabel自动换行
    m_textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080; font-size:12px;}");
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setText(txt);

    QHBoxLayout *h_layout = new QHBoxLayout(this->m_frame);
    h_layout->setContentsMargins(10, 5, 10, 5);
    h_layout->setSpacing(0);
    h_layout->addWidget(m_textLabel, 0, Qt::AlignVCenter);

    main_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    this->adjustSize();

    hide();
}

LifestyleTip::LifestyleTip(const QString &title, const QString &desc, QWidget *parent) : QFrame(parent)
  , m_radius(1)
  , m_background(QBrush(QColor(255,255,255,255)))
  , m_borderColor(QColor(224,224,224,130))
  , m_trianglePostion(TopMiddle)
{
    this->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(260, 56);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);

    m_frame = new QFrame();
    m_frame->setContentsMargins(0, 0, 0, 0);

    m_textLabel = new QLabel;
    m_textLabel->setWordWrap(true);//QLabel自动换行
    m_textLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080; font-size:12px;}");
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setText(title);

    m_descLabel = new QLabel;
    m_descLabel->setWordWrap(true);//QLabel自动换行
    m_descLabel->setStyleSheet("QLabel{border:none;background-color:transparent;color:#808080; font-size:12px;}");
    m_descLabel->setAlignment(Qt::AlignCenter);
    m_descLabel->setText(desc);

    QVBoxLayout *h_vayout = new QVBoxLayout(this->m_frame);
    h_vayout->setContentsMargins(10, 5, 10, 5);
    h_vayout->setSpacing(5);
    h_vayout->addWidget(m_textLabel, 0, Qt::AlignLeft);
    h_vayout->addWidget(m_descLabel, 0, Qt::AlignLeft);

    main_layout->addWidget(m_frame, 0, Qt::AlignVCenter);

    this->adjustSize();

    hide();
}

LifestyleTip::~LifestyleTip()
{

}

void LifestyleTip::resetTipText(const QString &txt)
{
    m_textLabel->setText(txt);
}

void LifestyleTip::resetTipText(const QString &title, const QString &desc)
{
    m_textLabel->setText(title);
    m_descLabel->setText(desc);
}

/*void LifestyleTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    QMarginsF shadowMargins = QMarginsF(2.0, 2.0, 2.0, 2.0);
    QRectF bgRect = QRectF(rect()).marginsRemoved(shadowMargins);
    QPainterPath bgPath;
    bgPath.addRoundedRect(bgRect, this->m_radius, this->m_radius);
    painter.fillPath(bgPath, background);

    QPainterPath borderPath;
    QRectF borderRect = QRectF(rect());
    int borderRadius = this->m_radius;
    QMarginsF borderMargins(0.5, 0.5, 0.5, 0.5);
    borderRadius += 0.5;
    borderRect = borderRect.marginsAdded(borderMargins).marginsRemoved(shadowMargins);
    borderPath.addRoundedRect(borderRect, borderRadius, borderRadius);

    QPen borderPen(borderColor);
    borderPen.setWidthF(1.0);
    painter.strokePath(borderPath, borderPen);
}*/

void LifestyleTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush background =  this->m_background;
    QColor borderColor = this->m_borderColor;

    const qreal radius = this->m_radius;
    const qreal triangleHeight = 6;
    const qreal triangleWidth = 8;
    const qreal height = this->height() - triangleHeight;

    QPainterPath path;
//    QRect rect = this->rect();
//    int arcR = radius/2;
//    path.moveTo(rect.left() + arcR, rect.top());
//    path.arcTo(rect.left(), rect.top(), radius, radius, 90.0f, 90.0f);
//    path.moveTo(rect.left(), rect.bottom() - arcR);
//    path.arcTo(rect.left(), rect.bottom() - radius, radius, radius, 180.0f, 90.0f);
//    path.moveTo(rect.right() - arcR, rect.bottom());
//    path.arcTo(rect.right() - radius, rect.bottom() - radius, radius, radius, 270.0f, 90.0f);
//    path.moveTo(rect.right(), rect.top() + arcR);
//    path.arcTo(rect.right() - radius, rect.top(), radius, radius, 0.0f, 90.0f);

    if (m_trianglePostion == TopLeft) {
        int triangleVertexPosX = 30;//根据IndexItemWidget中图标的位置x
        path.moveTo(radius, triangleHeight);

        path.lineTo(radius + triangleVertexPosX - triangleWidth, triangleHeight);
        path.lineTo(radius + triangleVertexPosX, 0);
        path.lineTo(radius + triangleVertexPosX + triangleWidth, triangleHeight);

        path.lineTo(this->width() - radius, triangleHeight);
        path.arcTo(QRectF(QPointF(this->width(), triangleHeight), QPointF(this->width() - 2 * radius, 2 * radius + triangleHeight)), 90.0, 90.0);//以90度为起点，逆时针画90度

        path.lineTo(this->width(), height - triangleHeight - radius);
        path.arcTo(QRectF(QPointF(this->width(), height), QPointF(this->width() - 2 * radius, height - 2 * radius)), 180.0, -90.0);//以180度为起点，顺时针画90度

        path.arcTo(QRectF(QPointF(0, height - 2 * radius), QPointF(2 * radius, height)), 270.0, -90.0);//以270度为起点，顺时针画90度
        path.lineTo(0.0, radius + triangleHeight);

        path.arcTo(QRectF(QPointF(0, triangleHeight), QPointF(2 * radius, 2 * radius + triangleHeight)), 180.0, -90.0);//以180度为起点，顺时针画90度
        path.lineTo(radius, triangleHeight);
    }
    else {//TopMiddle
        path.moveTo(radius, triangleHeight);

        path.lineTo(radius, triangleHeight);
        path.lineTo(radius + triangleWidth / 2, 0);
        path.lineTo(radius + triangleWidth, triangleHeight);

        path.lineTo(this->width() - radius, triangleHeight);
        path.arcTo(QRectF(QPointF(this->width(), triangleHeight), QPointF(this->width() - 2 * radius, 2 * radius + triangleHeight)), 90.0, 90.0);//以90度为起点，逆时针画90度

        path.lineTo(this->width(), height - triangleHeight - radius);
        path.arcTo(QRectF(QPointF(this->width(), height), QPointF(this->width() - 2 * radius, height - 2 * radius)), 180.0, -90.0);//以180度为起点，顺时针画90度

        path.arcTo(QRectF(QPointF(0, height - 2 * radius), QPointF(2 * radius, height)), 270.0, -90.0);//以270度为起点，顺时针画90度
        path.lineTo(0.0, radius + triangleHeight);

        path.arcTo(QRectF(QPointF(0, triangleHeight), QPointF(2 * radius, 2 * radius + triangleHeight)), 180.0, -90.0);//以180度为起点，顺时针画90度
        path.lineTo(radius, triangleHeight);
    }

    painter.fillPath(path, background);

    QPen pen(borderColor);
    pen.setWidth(1);
    painter.strokePath(path, pen);
}
