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

#include "cityview.h"
#include "citymodel.h"

#include <QScrollBar>

CityView::CityView(QWidget *parent) : QListView(parent)
{
    this->setFrameStyle(QFrame::NoFrame);
    this->setMouseTracking(true);
    this->setUpdatesEnabled(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSpacing(1);
    this->setContentsMargins(0, 0, 0, 0);
    this->verticalScrollBar()->setSingleStep(1);
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{margin:0px 0px 0px 0px;background-color:rgb(255,255,255,100);border:0px;width:10px;}" \
                                             "QScrollBar::sub-line:vertical{subcontrol-origin:margin;border:1px solid red;height:13px;}" \
                                             "QScrollBar::up-arrow:vertical{subcontrol-origin:margin;background-color:blue;height:13px;}" \
                                             "QScrollBar::sub-page:vertical{background-color:#EEEDF0;}" \
                                             "QScrollBar::handle:vertical{background-color:#D1D0D2;width:10px;}" \
                                             "QScrollBar::handle:vertical:hover{background-color:#14ACF5;width:10px;}" \
                                             "QScrollBar::handle:vertical:pressed{background-color:#0B95D7;width:10px;}" \
                                             "QScrollBar::add-page:vertical{background-color:#EEEDF0;}" \
                                             "QScrollBar::down-arrow:vertical{background-color:yellow;}" \
                                             "QScrollBar::add-line:vertical{subcontrol-origin:margin;border:1px solid green;height:18px;}");

    connect(this, &CityView::currentHoverChanged, this, [=] (const QModelIndex &pre, const QModelIndex &cur) {
        if (pre.isValid()) {
            closePersistentEditor(pre);
        }
        openPersistentEditor(cur);
    });
}

const QModelIndex &CityView::currentHoverIndex() const
{
    return m_currentIndex;
}

void CityView::enterEvent(QEvent *event)
{
    if (m_currentIndex.isValid()) {
        openPersistentEditor(m_currentIndex);
    }

    QWidget::enterEvent(event);
}

void CityView::leaveEvent(QEvent *event)
{
    if (m_currentIndex.isValid()) {
        closePersistentEditor(m_currentIndex);
        static_cast<CityModel *>(model())->setCurrentIndexHover(QModelIndex());
    }

    QWidget::leaveEvent(event);
}

void CityView::onItemEntered(const QModelIndex &index)
{
    m_currentIndex = index;
    static_cast<CityModel *>(model())->setCurrentIndexHover(m_currentIndex);
    if (m_previousIndex != m_currentIndex) {
        emit currentHoverChanged(m_previousIndex, m_currentIndex);
        m_previousIndex = m_currentIndex;
    }
}
