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
**   @file   photoformat.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Class for storing info about photo format
**
**************************************************************************/

#ifndef PHOTOFORMAT_H
#define PHOTOFORMAT_H

#include <QtCore>

/**
 Photo format class, that desctibe format:
  - name (for user)
  - size in mm
  - distaces from top of photo to pate and from bridge of nose to chin in mm
 */
class PhotoFormat
{
public:
    PhotoFormat(QString name               = QString(),
                QSizeF  size               = QSize(),
                qreal   top_pate_distance  = 0,
                qreal   nose_chin_distance = 0);

    void setFormanName(QString name);
    void setSize(QSizeF size);
    void setTopPateDistance(qreal distance);
    void setNoseChinDistance(qreal distance);

    QString  getFormatName() const;
    QSizeF   getSize() const;
    qreal    getTopPateDistance() const;
    qreal    getNoseChinDistance() const;

private:
    QString _format_name;
    QSizeF  _size;
    qreal   _top_pate_distance;
    qreal   _bridge_of_nose_chin_distace;

};

// Для использования в настройках
Q_DECLARE_METATYPE(PhotoFormat);

QDataStream &operator<<(QDataStream &out, const PhotoFormat &obj);
QDataStream &operator>>(QDataStream &in, PhotoFormat &obj);


#endif // PHOTOFORMAT_H
