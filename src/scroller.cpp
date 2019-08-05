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

#include "scroller.h"

#include <QObject>

#if QT_VERSION >= 0x050000
#include <QScroller>
#include <QScrollerProperties>
#endif

void Scroller::setScroller(QObject *w, bool touch)
{
#if QT_VERSION >= 0x050000
        QScroller::grabGesture(w, touch ? QScroller::TouchGesture : QScroller::LeftMouseButtonGesture);

        QVariant overshoot = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
        QScrollerProperties sp = QScroller::scroller(w)->scrollerProperties();
        sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, overshoot);
        sp.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, overshoot);
        QScroller::scroller(w)->setScrollerProperties(sp);
#endif
}
