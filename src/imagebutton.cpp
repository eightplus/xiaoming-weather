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

#include "imagebutton.h"

#include <QMouseEvent>
#include <QEvent>
#include <QPixmap>

ImageButton::ImageButton(QWidget *parent)
    : QLabel(parent)
{
    updateIconImage();
}

ImageButton::~ImageButton()
{
}

void ImageButton::enterEvent(QEvent *event)
{
    setCursor(Qt::PointingHandCursor);

    setBtnState(Hover);

    event->accept();
}

void ImageButton::leaveEvent(QEvent *event)
{
    setBtnState(Normal);

    event->accept();
}

void ImageButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    setBtnState(Press);

    event->accept();
}

void ImageButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (!rect().contains(event->pos()))
        return;

    setBtnState(Hover);

    event->accept();

    if (event->button() == Qt::LeftButton)
        emit clicked();
}

void ImageButton::updateIconImage()
{
    switch (m_state) {
    case Hover:
        if (!m_hoverPic.isEmpty()) {
            setPixmap(QPixmap(m_hoverPic));
        }
        break;
    case Press:
        if (!m_pressPic.isEmpty())
            setPixmap(QPixmap(m_pressPic));
        break;
    default:
        if (!m_normalPic.isEmpty())
            setPixmap(QPixmap(m_normalPic));
        break;
    }

    setAlignment(Qt::AlignCenter);
}

void ImageButton::setBtnState(ImageButton::BtnState state)
{
    if (m_state == state)
        return;

    m_state = state;

    updateIconImage();
}

void ImageButton::setNormalPic(const QString &pixmap)
{
    m_normalPic = pixmap;

    updateIconImage();
}

void ImageButton::setHoverPic(const QString &pixmap)
{
    m_hoverPic = pixmap;

    updateIconImage();
}

void ImageButton::setPressPic(const QString &pixmap)
{
    m_pressPic = pixmap;

    updateIconImage();
}
