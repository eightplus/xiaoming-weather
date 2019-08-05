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

#ifndef ACTIVEBUTTON_H
#define ACTIVEBUTTON_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ActiveButton : public QFrame
{
    Q_OBJECT

public:
    explicit ActiveButton(const QString &text, QWidget *parent = 0);
    ~ActiveButton();

    void setActive(bool active);

signals:
    void btnClicked();

private:
    QPushButton *m_btn = nullptr;
    QLabel *m_label = nullptr;
    QVBoxLayout *m_layout = nullptr;
    bool m_isActive = false;
};

#endif // ACTIVEBUTTON_H
