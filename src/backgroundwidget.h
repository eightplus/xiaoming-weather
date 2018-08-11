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

#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include <QWidget>

class BackgroundWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(QString backPixmap READ backPixmap WRITE setBackPixmap DESIGNABLE true SCRIPTABLE true)

public:
    explicit BackgroundWidget(QWidget *parent = 0);

    void setForNight(bool isNight);

    void setColor(const QColor &color);
    const QColor &color() { return m_color; }

    const QString &backPixmap() const { return m_backPixmap; }
    void setBackPixmap(const QString &pix);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QColor m_color;
    QString m_backPixmap;

    bool m_isNight;
    qreal m_opacity;
    QPixmap m_weatherBgPixmap;
};

#endif // BACKGROUNDWIDGET_H
