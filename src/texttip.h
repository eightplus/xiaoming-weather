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

#ifndef _TEXTTIP_H
#define _TEXTTIP_H

#include <QFrame>

#include "utils.h"

class QLabel;

class TextTip : public QFrame
{
    Q_OBJECT

public:
    explicit TextTip(const QString &txt, TrianglePostion pos, QWidget *parent = 0);
    explicit TextTip(const QString &title, const QString &desc, TrianglePostion pos, QWidget *parent = 0);
    ~TextTip();

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

#endif // _TEXTTIP_H
