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

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QFrame>
#include <QWidget>
#include <QMouseEvent>

class QHBoxLayout;
class QMenu;

class TitleBar : public QFrame
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent);
    ~TitleBar();

    void initLeftContent();
    void initMiddleContent();
    void initRightContent();
    void initWidgets();
    void initMenu();

public slots:
    void onSwitch();
    void onAbout();

signals:
    void switchDayOrNight();
    void requestDisplayAboutDialog();
    void requestMinSize();

private:
    QHBoxLayout *m_layout = nullptr;
    QHBoxLayout *m_lLayout = nullptr;
    QHBoxLayout *m_mLayout = nullptr;
    QHBoxLayout *m_rLayout = nullptr;
    QMenu *m_menu = nullptr;
};

#endif // TITLEBAR_H
