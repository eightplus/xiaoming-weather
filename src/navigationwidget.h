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

#ifndef NAVIGATION_WIDGET_H
#define NAVIGATION_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class QTimer;
class CityNavigation;
class ImageButton;
//class TopTextTip;
class TextTip;
class TipModule;

class NavigationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationWidget(QWidget *parent = 0, int cityCount = 0);
    ~ NavigationWidget();

    void setCityCount(const int count, const int curIndex);
    int getCityCount() const { return m_count; }

signals:
    void requestPrevCity();
    void requestNextCity();

private:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    CityNavigation *m_cityNavigation = nullptr;
    ImageButton *m_prevCityBtn = nullptr;
    ImageButton *m_nextCityBtn = nullptr;
    int m_count;
    QTimer *m_wheelTimer;
    /*TopTextTip*/TextTip *tip;
    TipModule *m_tipModule = nullptr;
};

#endif // NAVIGATION_WIDGET_H
