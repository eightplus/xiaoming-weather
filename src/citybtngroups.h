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

#ifndef CITYBTNGROUPS_H
#define CITYBTNGROUPS_H

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

class CityBtnGroups : public QFrame
{
    Q_OBJECT

public:
    explicit CityBtnGroups(QWidget *parent = 0);
    ~CityBtnGroups();

    void setActive(bool active);

signals:
    void defaultBtnClicked();
    void removeBtnClicked();

private:
    QPushButton *m_defaultBtn = nullptr;
    QPushButton *m_removeBtn = nullptr;
    QHBoxLayout *m_layout = nullptr;
    bool m_isActive = false;
};

#endif // CITYBTNGROUPS_H
