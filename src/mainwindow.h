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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TitleBar;
class WeatherWidget;
class LocationWidget;
class QVBoxLayout;
class BackgroundWidget;
class WeatherWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void moveCenter();

    void showSettingDialog();
    void createSettingDialog();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPoint m_dragPosition;
    bool m_mousePressed;
    QWidget *m_centerWidget = nullptr;
    TitleBar *m_titleBar = nullptr;
    WeatherWidget *m_weatherWidget = nullptr;
    LocationWidget *m_locationWidget = nullptr;
    QVBoxLayout *m_layout = nullptr;
    BackgroundWidget *m_backgroundWidget = nullptr;
    bool m_isNight;
    WeatherWorker *m_weatherWorker = nullptr;
    QTimer *m_autoRefreshTimer = nullptr;
};

#endif // MAINWINDOW_H
