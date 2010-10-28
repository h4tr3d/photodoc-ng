/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2006-04-21
 * Description : main photograph information container
 *
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

#ifndef PHOTOINFOCONTAINER_H
#define PHOTOINFOCONTAINER_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QDateTime>

// Local includes

class PhotoInfoContainer
{

public:

    PhotoInfoContainer(){};

    bool isEmpty()
    {
        if ( make.isEmpty()            &&
             model.isEmpty()           &&
             lens.isEmpty()            &&
             exposureTime.isEmpty()    &&
             exposureMode.isEmpty()    &&
             exposureProgram.isEmpty() &&
             aperture.isEmpty()        &&
             focalLength.isEmpty()     &&
             focalLength35mm.isEmpty() &&
             sensitivity.isEmpty()     &&
             flash.isEmpty()           &&
             whiteBalance.isEmpty()    &&
             !dateTime.isValid() )
            return true;
        else
            return false;
    };

    QString   make;
    QString   model;
    QString   lens;
    QString   exposureTime;
    QString   exposureMode;
    QString   exposureProgram;
    QString   aperture;
    QString   focalLength;
    QString   focalLength35mm;
    QString   sensitivity;
    QString   flash;
    QString   whiteBalance;

    QDateTime dateTime;
};

QDataStream& operator<<(QDataStream& ds, const PhotoInfoContainer& info);
QDataStream& operator>>(QDataStream& ds, PhotoInfoContainer& info);

#endif /* PHOTOINFOCONTAINER_H */
