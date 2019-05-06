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

#ifndef TEXTTIP_H
#define TEXTTIP_H

#include <QFrame>

class QLabel;

class LifestyleTip : public QFrame
{
    Q_OBJECT

    enum TrianglePostion {TopLeft, TopMiddle, TopRight, BottomLeft, BottomMiddle, BottomRight};

public:
    explicit LifestyleTip(const QString &txt, QWidget *parent = 0);
    explicit LifestyleTip(const QString &title, const QString &desc, QWidget *parent = 0);
    ~LifestyleTip();

    void resetTipText(const QString &txt);
    void resetTipText(const QString &title, const QString &desc);

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    TrianglePostion m_trianglePostion;
    int m_radius;
    QBrush m_background;
    QColor m_borderColor;
    QLabel *m_textLabel = nullptr;
    QLabel *m_descLabel = nullptr;
    QFrame *m_frame = nullptr;

};

#endif // TEXTTIP_H
