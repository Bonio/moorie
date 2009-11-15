/*
 *   Copyright (C) 2008-2009 by Patryk Połomski
 *   cykuss@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef THREADLOGS_H
#define THREADLOGS_H
#include <QtCore>
#include <QThread>
#include <QTime>

#include "singleton.h"
#include <Log.h>
#include <iostream>

class threadLogs : public QThread
{
    Q_OBJECT
public:
    threadLogs();
    void run();
    void setLogLevel(int);

    class LogGuiHandle: public LogHandle
    {
    public:
        LogGuiHandle(Log::Level lvl);
        virtual ~LogGuiHandle();
        virtual void log(const char *msg);
    };
Q_SIGNALS:
    void append(const QString & text);

};

#endif // THREADLOGS_H
