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

#include "locationwidget.h"

#include <QDebug>

LocationWidget::LocationWidget(QWidget *parent)
    : QWidget(parent)
{
//    this->setFixedSize(parent->size());
//    qDebug() << parent->size();

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(1);

    m_label = new QLabel;
    m_label->setText(tr("Location Page"));
//    m_label->setAlignment(Qt::AlignHCenter/* | Qt::AlignTop*/);
//    m_label->setAlignment(Qt::AlignCenter/* | Qt::AlignTop*/);
    m_label->setStyleSheet("QLabel{border-radius: 0px; color:rgb(250, 250, 250); background-color:argb(60, 60, 60, 130);}");

    m_backBtn = new QPushButton;
    m_backBtn->setFixedHeight(24);
    m_backBtn->setFocusPolicy(Qt::NoFocus);
    m_backBtn->setStyleSheet("QPushButton{border:none;text-align:center;font-size:14px;color:#ffffff;background:transparent;}");
    m_backBtn->setText(tr("Back"));
    QFont font = m_backBtn->font();
    const QFontMetrics fm(font);
    m_backBtn->setFixedWidth(fm.width(m_backBtn->text()) + 10);

    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_backBtn, 0, Qt::AlignHCenter);

    connect(m_backBtn, &QPushButton::clicked, this, &LocationWidget::backBtnClicked);
}

LocationWidget::~LocationWidget()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }
}

