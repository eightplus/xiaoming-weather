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

#include "tipmodule.h"

#include <QWidget>
#include <QTimer>
#include <QEvent>
#include <QDebug>

TipModule::TipModule(TrianglePostion pos, QObject *parent)  : QObject(parent)
    , m_displayTimer(new QTimer)
    , m_trianglePostion(pos)
{
    m_displayTimer->setInterval(300);
    connect(m_displayTimer, SIGNAL(timeout()), this, SLOT(onDisplayTimeOut()));
}

TipModule::~TipModule()
{
    if (m_displayTimer) {
        disconnect(m_displayTimer,SIGNAL(timeout()), this, SLOT(onDisplayTimeOut()));
        if(m_displayTimer->isActive()) {
            m_displayTimer->stop();
        }
        delete m_displayTimer;
        m_displayTimer = nullptr;
    }
}

void TipModule::onDisplayTimeOut()
{
    if (m_parentWidget) {
        QWidget *tip = m_parentWidget->property("TextTipWidget").value<QWidget *>();
        if (!tip) {
            this->m_displayTimer->stop();
            return;
        }

        if (m_tipWidget && m_tipWidget != tip) {
            m_tipWidget->setVisible(false);
        }
        m_tipWidget = tip;
        m_tipWidget->adjustSize();
        m_tipWidget->setVisible(true);
        m_tipWidget->raise();

        QTimer::singleShot(5, this, [this] {
            if (!m_parentWidget) {
                return;
            }
            if (m_trianglePostion == TrianglePostion::TopLeft) {
                QPoint point = m_parentWidget->mapToGlobal(m_parentWidget->rect().bottomLeft());
                m_tipWidget->move(point);
            }
            else if (m_trianglePostion == TrianglePostion::BottomMiddle) {
                QPoint centerPos = m_parentWidget->mapToGlobal(m_parentWidget->rect().center());
                QSize tipSize = m_tipWidget->size();
                centerPos.setX(centerPos.x() - tipSize.width()/2);
                centerPos.setY(centerPos.y() - tipSize.height() - 30);
                centerPos = m_tipWidget->mapFromGlobal(centerPos);
                centerPos = m_tipWidget->mapToParent(centerPos);
                m_tipWidget->move(centerPos);
            }
            else {
                QPoint point = m_parentWidget->mapToGlobal(m_parentWidget->rect().bottomLeft());//m_parentWidget->rect().center()
                m_tipWidget->move(point);
            }
        });
    }
    this->m_displayTimer->stop();
}

bool TipModule::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
    {
        if (this->m_tipWidget && this->m_tipWidget->isVisible()) {
            this->m_tipWidget->setVisible(false);
        }
        QWidget *widget = qobject_cast<QWidget *>(obj);
        this->m_parentWidget = widget;
        if (!widget) {
            break;
        }
        this->m_tipWidget = widget->property("TextTipWidget").value<QWidget *>();
        if (!this->m_tipWidget) {
            break;
        }
        this->m_displayTimer->start();
        break;
    }
    case QEvent::Leave:
        if (this->m_tipWidget) {
            this->m_tipWidget->setVisible(false);
            this->m_displayTimer->stop();
        }
        break;
    case QEvent::MouseButtonPress:
        if (this->m_tipWidget) {
            this->m_tipWidget->setVisible(false);
            this->m_displayTimer->stop();
        }
        break;
    default:
        break;
    }

    return QObject::eventFilter(obj, event);
}
