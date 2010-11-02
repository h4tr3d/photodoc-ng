/**************************************************************************
**   Copyright (C) 2010 by hatred
**   hatred@inbox.ru
**   http://code.google.com/p/photodoc-ng/
**
**   This file is a part of "PhotoDoc NG" application
**
**   This program is free software; you can redistribute it and/or modify
**   it under the terms of the version 2 of GNU General Public License as
**   published by the Free Software Foundation.
**
**   For more information see LICENSE and LICENSE.ru files
**
**   @file   utils.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Different utils functions
**
**************************************************************************/

#if defined(WIN32)
#   include <windows.h>
#   include <lmcons.h>
#else
#   include <pwd.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "utils.h"
#include "md5/md5.h"

//
// Get rect of selection result
//
QRectF getPhotoRect(QSizeF real_size, QSizeF view_size, PhotoFormat format, QList<QPointF> points)
{
    QRectF result;

    if (points.count() != 3)
    {
        // all 3 control dots must be present
        return result;
    }

    // Scale factor, I think, that scaling produced with keep aspect ration
    qreal scale = real_size.width() / view_size.width();

    QPointF p0 = points.at(0) * scale; // Scale coordinates to real size
    QPointF p1 = points.at(1) * scale;
    QPointF p2 = points.at(2) * scale;

    // Photo distances in dots of source image, calculating via proportion
    qreal top_pate_dots = format.getTopPateDistance() /
                             format.getSize().height() * real_size.height();
    qreal bridge_of_nose_chin_dots
                        = format.getNoseChinDistance() / format.getSize().height() * real_size.height();

    // Calculate top Y coordinate on source image, also viea proportion
    qreal rect_y = p0.y() - top_pate_dots * (p2.y() - p1.y()) / bridge_of_nose_chin_dots;

    // Calculate Y coordinate of target imaga
    qreal rect_height = real_size.height() * (p2.y() - p1.y()) / bridge_of_nose_chin_dots;

    // Target image aspect ratio
    qreal aspect_ratio = format.getSize().width() / format.getSize().height();

    // Calculate target image width in pixels
    qreal rect_width = rect_height * aspect_ratio;

    // Calculate middle X coordinate average
    qreal middle_x = (p0.x() + p1.x() + p2.x()) / 3.0;

    // Calculate X coordinate
    qreal rect_x = middle_x - rect_width / 2.0;

    // Store result
    result = QRectF(rect_x, rect_y, rect_width, rect_height);

    return result;
}

/**
 * Platform independ user name detection (currently win32 and *nix)
 * Don't pass return result to free(). Use strdup() for string duplication.
 *
 * @return NULL on error, null-terminated string with user name instead
 */
const char *getUserName()
{
    static char *user_name = NULL;
#if defined(WIN32)
    DWORD size = UNLEN + 1;

    if (user_name == NULL)
    {
        user_name = (char*)calloc(sizeof(char), size);
        if (user_name == NULL)
            return NULL;
    }

    GetUserNameA(user_name, &size);
#else
    struct passwd *passwd = getpwuid(getuid());
    if (passwd == NULL)
        return NULL;

    user_name = passwd->pw_name;
#endif

    return user_name;
}

QString getMd5HexDigest(QByteArray data)
{
    QByteArray hash = getMd5(data);
    QString result;
    for (int i = 0; i < hash.count(); i++)
    {
        result.append(QString("%1").arg((int)hash.at(i), 2, 16));
    }
    return result;
}

QByteArray getMd5(QByteArray data)
{
    uint8_t hash[16];
    memset(hash, 0, sizeof(hash));
    md5hash((uint8_t*)data.data(), data.size(), hash);
    QByteArray result((char*)hash, sizeof(hash));
    return result;
}
