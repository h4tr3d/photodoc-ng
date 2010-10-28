/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2004-09-18
 * Description : camera item info container
 *
 * Copyright (C) 2004-2005 by Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Copyright (C) 2006-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef GPITEMINFO_H
#define GPITEMINFO_H

// Qt includes

#include <QList>
#include <QByteArray>
#include <QDateTime>

// Local includes

#include "photoinfocontainer.h"

class QDataStream;

class GPItemInfo
{

public:

    enum DownloadStatus
    {
        DownloadUnknown  = -1,
        DownloadedNo     = 0,
        DownloadedYes    = 1,
        DownloadFailed   = 2,
        DownloadStarted  = 3,
        NewPicture       = 4
    };

public:

    qint64             size;

    int                width;
    int                height;
    int                downloaded;           // See DownloadStatus enum.
    int                readPermissions;
    int                writePermissions;

    QString            name;
    QString            folder;
    QString            mime;

    QDateTime          mtime;

    PhotoInfoContainer photoInfo;
};

QDataStream& operator<<(QDataStream&, const GPItemInfo&);
QDataStream& operator>>(QDataStream&, GPItemInfo&);

typedef QList<GPItemInfo> GPItemInfoList;

#endif /* GPITEMINFO_H */
