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

#include "citybtngroups.h"

#include <QDebug>

CityBtnGroups::CityBtnGroups(QWidget *parent) :
    QFrame(parent)
{
    this->setFixedHeight(28);
    this->setStyleSheet("QFrame{background-color:transparent;border:none;}");

    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 10, 0);
    m_layout->setSpacing(10);

    m_defaultBtn = new QPushButton(this);
    m_defaultBtn->setFocusPolicy(Qt::NoFocus);
    m_defaultBtn->setStyleSheet("QPushButton{border:none;text-align:center;background:transparent;background-color:transparent;color:#0a9ff5;font-size:12px;}");
    m_defaultBtn->setText(tr("Set as default"));
    m_defaultBtn->setCursor(QCursor(Qt::PointingHandCursor));
    m_defaultBtn->setVisible(false);
    QFont defaultfont = m_defaultBtn->font();
    const QFontMetrics defaultfm(defaultfont);
    m_defaultBtn->setFixedWidth(defaultfm.width(m_defaultBtn->text()) + 10);

    m_removeBtn = new QPushButton(this);
    m_removeBtn->setFixedSize(12, 12);
    m_removeBtn->setFocusPolicy(Qt::NoFocus);
    m_removeBtn->setStyleSheet("QPushButton{background-image:url(':/res/delete_normal.png');border:0px;}QPushButton:hover{background:url(':/res/delete_hover_press.png');}QPushButton:pressed{background:url(':/res/delete_hover_press.png');}");

    m_layout->addStretch();
    m_layout->addWidget(m_defaultBtn, 0, Qt::AlignVCenter);
    m_layout->addWidget(m_removeBtn, 0, Qt::AlignVCenter);

//    adjustSize();

    connect(m_defaultBtn, &QPushButton::clicked, this, &CityBtnGroups::defaultBtnClicked);
    connect(m_removeBtn, &QPushButton::clicked, this, &CityBtnGroups::removeBtnClicked);
}

CityBtnGroups::~CityBtnGroups()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->deleteLater();
        delete child;
    }

    delete m_layout;
}

void CityBtnGroups::setActive(bool active)
{
    this->m_isActive = active;
    if (this->m_isActive) {
        m_defaultBtn->setVisible(false);
    }
    else {
        m_defaultBtn->setVisible(true);
    }
}
