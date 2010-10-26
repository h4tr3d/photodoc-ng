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

#include <QImage>

#include <math.h>

#include "imageprocess.h"


/**
 Apply brightness and contrast formulas to color value @param value
 Code taked from Gimp 2.7.1

 @param brightness - brightness value in range [-1..1]
 @param contrast   - contrast value in range [-1..1]
 @param value      - color component value in range [0..1]

 @return new color component value in range [0..1]
 */
qreal brightnessContrast(qreal brightness, qreal contrast, qreal value)
{
    qreal slant;

    /* apply brightness */
    if (brightness < 0.0)
        value = value * (1.0 + brightness);
    else
        value = value + ((1.0 - value) * brightness);

    /* apply contrast */
    slant = tan((contrast + 1) * M_PI_4);
    value = (value - 0.5) * slant + 0.5;

    /* clip value */
    if(value > 1.0)
        value = 1.0;
    if (value < 0.0)
        value = 0.0;

    return value;
}

/**
 Apply brightness and contrast to Image and return new one

 @param src - source image in RGBA format
 @param brighrness - brightness value in range [-1..1]
 @param contrast - contrast value in range [-1..1]

 @return new image
 */
QImage applyBrightnessContrast(QImage &src, qreal brightness, qreal contrast)
{
    QImage result = src;
    uchar *data_ptr    = 0;
    int i = 0;

    int size = result.size().width() * result.size().height();

    data_ptr = result.bits();
    QRgb *rgb_ptr = (QRgb*)data_ptr;
    for (i = 0; i < size; i++)
    {
        int r = qRed(rgb_ptr[i]);
        int g = qGreen(rgb_ptr[i]);
        int b = qBlue(rgb_ptr[i]);

        r = (int)(brightnessContrast(brightness, contrast, (qreal)r / 255.0) * 255.0);
        g = (int)(brightnessContrast(brightness, contrast, (qreal)g / 255.0) * 255.0);
        b = (int)(brightnessContrast(brightness, contrast, (qreal)b / 255.0) * 255.0);

        rgb_ptr[i] = qRgba(r, g, b, qAlpha(rgb_ptr[i]));
    }

    return result;
}

/**
 Transform image to grayscale

 @param src - image in RGBA format

 @return grayscaled image
 */
QImage transformToGrayscale(QImage &src)
{
    QImage result = src;
    uchar *data_ptr    = 0;
    int i = 0;

    int size = result.size().width() * result.size().height();

    data_ptr = result.bits();
    QRgb *rgb_ptr = (QRgb*)data_ptr;
    for (i = 0; i < size; i++)
    {
        int r = qRed(rgb_ptr[i]);
        int g = qGreen(rgb_ptr[i]);
        int b = qBlue(rgb_ptr[i]);

        int gray = (int)RGB_LUMINANCE(r, g, b);

        rgb_ptr[i] = qRgba(gray, gray, gray, qAlpha(rgb_ptr[i]));
    }

    return result;
}
