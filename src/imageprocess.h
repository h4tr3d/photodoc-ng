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
**   @brief  Util functions and macrosses for image processing
**           some code taked from Gimp project
**
**************************************************************************/

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <QtCore>

/*
 * The weights to compute true CIE luminance from linear red, green
 * and blue, as defined by the ITU-R Recommendation BT.709, "Basic
 * Parameter Values for the HDTV Standard for the Studio and for
 * International Programme Exchange" (1990). Also suggested in the
 * sRGB colorspace specification by the W3C.
 */

#define RGB_LUMINANCE_RED    (0.2126)
#define RGB_LUMINANCE_GREEN  (0.7152)
#define RGB_LUMINANCE_BLUE   (0.0722)

#define RGB_LUMINANCE(r,g,b) ((r) * RGB_LUMINANCE_RED   + \
                              (g) * RGB_LUMINANCE_GREEN + \
                              (b) * RGB_LUMINANCE_BLUE)


//
// apply brightness and contrast formulas to value
//
qreal brightnessContrast(qreal brightness, qreal contrast, qreal value);

//
// apply brightness and contrast to Image
//
QImage applyBrightnessContrast(QImage &src, qreal brightness, qreal contrast);

//
// transform image to Gray scaled
//
QImage transformToGrayscale(QImage &src);

#endif // IMAGEPROCESS_H
