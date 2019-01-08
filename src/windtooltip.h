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

#ifndef WIND_TOOLTIP_H
#define WIND_TOOLTIP_H

#include <QLabel>

class WindTooltip : public QLabel
{

    Q_OBJECT
    enum TrianglePostion {TopMiddle, BottomMiddle};

public:
    explicit WindTooltip(QWidget *parent = 0);
    ~WindTooltip();

    void resetTipText(const QString &title, const QString &desc);
    void setTrianglePosition(TrianglePostion position);

public slots:
    void popupTip(const QPoint &point);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_text;
    QString m_desc;
    TrianglePostion m_trianglePostion;
    int m_borderRadius;
};

#endif // WIND_TOOLTIP_H
