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

#include "categorybutton.h"

#include <QPainter>
#include <QDebug>

CategoryButton::CategoryButton(QWidget *parent) :
    QWidget(parent)
    , m_icon(new QLabel(this))
    , m_text(new QLabel(this))
{
    this->setStyleSheet("QWidget{background:transparent;border:0px;color:white;font-size:12px;}");
    this->setMouseEnterPress(false);

    m_hLayout = new QHBoxLayout;
    m_hLayout->setSpacing(5);
    m_hLayout->setContentsMargins(QMargins(5, 0, 5, 0));
    m_hLayout->addWidget(m_icon, 0, Qt::AlignVCenter);
    m_hLayout->addWidget(m_text, 0, Qt::AlignVCenter);
    m_hLayout->addStretch();

    this->setLayout(m_hLayout);
}

CategoryButton::~CategoryButton()
{
    QLayoutItem *child;
    while ((child = m_hLayout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    delete m_hLayout;
}

void CategoryButton::setIconAndText(const QPixmap &pixmap, const QString &txt)
{
    m_icon->setPixmap(pixmap.scaled(QSize(22, 22), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_text->setText(txt);
}

void CategoryButton::setMouseEnterPress(bool flag)
{
    m_mouseEnterPress = flag;

    this->update();
}

void CategoryButton::enterEvent(QEvent *event)
{
    if (!this->getMouseEnterPress()) {
        this->setMouseEnterPress(true);
    }
    emit requestChangeButtonStatus(this);

    this->setCursor(Qt::PointingHandCursor);

    event->accept();
}

void CategoryButton::leaveEvent(QEvent *event)
{
//    this->setMouseEnterPress(false);
    event->accept();
}

void CategoryButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setMouseEnterPress(true);

        emit requestChangeButtonStatus(this);
    }
}

void CategoryButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if (this->getMouseEnterPress()) {
        QPen pen(Qt::NoBrush, 1);
        painter.setPen(pen);
        QLinearGradient lineargradient(this->rect().topLeft(), this->rect().bottomLeft());
        lineargradient.setColorAt(0, QColor(255, 255, 255, 40));

        QBrush brush(lineargradient);
        painter.setBrush(brush);
        painter.drawRoundedRect(this->rect(), 1, 1);
    }

    QWidget::paintEvent(event);
}
