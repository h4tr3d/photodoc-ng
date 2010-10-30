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
**   @file   utils.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Different utils functions
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

/* MD5 wrappers */
QString    getMd5HexDigest(QByteArray data);
QByteArray getMd5(QByteArray data);

#endif // UTILS_H
