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

#include "toolbutton.h"
#include <QDebug>

ToolButton::ToolButton(QWidget *parent) :
    QPushButton(parent)
    , m_mousePressed(false)
{

}

void ToolButton::loadPixmap(QString pic_name)
{
    pixmap = QPixmap(pic_name);
    btn_width = pixmap.width()/3;
    btn_height = pixmap.height();

    this->setFixedSize(btn_width, btn_height);
}

void ToolButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    setBtnState(Hover);
    event->accept();
}

void ToolButton::leaveEvent(QEvent *event)
{
    setBtnState(Normal);
    event->accept();
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    m_mousePressed = true;
    setBtnState(Press);
    event->accept();
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (!rect().contains(event->pos()))
            return;

    if (m_mousePressed) {//if (event->button() == Qt::LeftButton)
        m_mousePressed = false;
        setBtnState(Hover);
        emit clicked();
    }
    event->accept();
}

void ToolButton::setBtnState(ToolButton::BtnState state)
{
    if (m_btnState == state)
        return;

    m_btnState = state;

    update();
}

void ToolButton::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(this->rect(), pixmap.copy(btn_width * m_btnState, 0, btn_width, btn_height));
    painter.end();
}
