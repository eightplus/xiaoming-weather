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

#ifndef LOCATIONWORKER_H
#define LOCATIONWORKER_H

#include <QList>
#include <QObject>
#include <QThread>

#include "data.h"

class LocationWorker;

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(LocationWorker *parent);
    ~WorkerThread() {}

    void run() Q_DECL_OVERRIDE;
};

class LocationWorker : public QObject
{
    Q_OBJECT
public:
    explicit LocationWorker(QObject *parent = 0);
    ~LocationWorker();

    QList<LocationData> exactMatchCity(const QString &inputText) const;

private:
    friend class WorkerThread;
    QList<LocationData> m_locatonList;
    WorkerThread *m_workerThread = nullptr;
};

#endif // LOCATIONWORKER_H
