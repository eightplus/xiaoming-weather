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

#ifndef CITYVIEW_H
#define CITYVIEW_H

#include <QListView>

class CityView : public QListView
{
    Q_OBJECT

public:
    CityView(QWidget *parent = 0);

    const QModelIndex &currentHoverIndex() const;
    void onItemEntered(const QModelIndex &index);

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

signals:
    void currentHoverChanged(const QModelIndex &pre, const QModelIndex &cur);

private:
    QModelIndex m_previousIndex;
    QModelIndex m_currentIndex;
};

#endif // CITYVIEW_H
