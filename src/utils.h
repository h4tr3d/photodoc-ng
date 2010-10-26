/**************************************************************************
**   Copyright (C) 2010 by hatred
**   hatred@inbox.ru
**   http://hatred.homelinux.net
**
**   This file is a part of "%ProjectName%" application
**
**   This program is free software; you can redistribute it and/or modify
**   it under the terms of the version 2 of GNU General Public License as
**   published by the Free Software Foundation.
**
**   For more information see LICENSE and LICENSE.ru files
**
**   @file   %FileName%
**   @date   %DATE%
**   @author hatred
**   @brief
**
**************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include "photoformat.h"

/**
 Function calculate rect for target photo after users iteractions
 @param real_size - size (width and height) of original image, that loaded into PhotoDoc
 @param view_size - size of view, in common cases is 640x480
 @param format    - photo format sructure, @see PhotoFormat
 @param points    - 3 base points in view coordinates
 */
QRectF getPhotoRect(QSizeF real_size, QSizeF view_size, PhotoFormat format, QList<QPointF> points);

/* Platform independ user name */
const char *getUserName();


#endif // UTILS_H
