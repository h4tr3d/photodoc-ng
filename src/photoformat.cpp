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

#include "photoformat.h"

PhotoFormat::PhotoFormat(QString name,
                         QSizeF  size,
                         qreal   top_pate_distance,
                         qreal   nose_chin_distance)
{
    _format_name                 = name;
    _size                        = size;
    _top_pate_distance           = top_pate_distance;
    _bridge_of_nose_chin_distace = nose_chin_distance;
}

void PhotoFormat::setFormanName(QString name)
{
    _format_name = name;
}

void PhotoFormat::setSize(QSizeF size)
{
    _size = size;
}

void PhotoFormat::setTopPateDistance(qreal distance)
{
    _top_pate_distance = distance;
}

void PhotoFormat::setNoseChinDistance(qreal distance)
{
    _bridge_of_nose_chin_distace = distance;
}

QString PhotoFormat::getFormatName() const
{
    return _format_name;
}

QSizeF PhotoFormat::getSize() const
{
    return _size;
}

qreal PhotoFormat::getTopPateDistance() const
{
    return _top_pate_distance;
}

qreal PhotoFormat::getNoseChinDistance() const
{
    return _bridge_of_nose_chin_distace;
}

// Для настроек, сеарилизация класса
QDataStream & operator <<(QDataStream &out, const PhotoFormat &obj)
{
    QString name = obj.getFormatName();
    QSizeF  size = obj.getSize();
    qreal   dst1 = obj.getTopPateDistance();
    qreal   dst2 = obj.getNoseChinDistance();

    out << name << size << dst1 << dst2;
    return out;
}

QDataStream & operator >>(QDataStream &in, PhotoFormat &obj)
{
    QString name;
    QSizeF  size;
    qreal   dst1;
    qreal   dst2;

    in >> name >> size >> dst1 >> dst2;

    obj.setFormanName(name);
    obj.setSize(size);
    obj.setTopPateDistance(dst1);
    obj.setNoseChinDistance(dst2);

    return in;
}
