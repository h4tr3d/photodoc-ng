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
**   @file   matrixview.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Class that stored information about matrix view:
**           page size, name, banner text and so on
**
**************************************************************************/

#include "matrixview.h"

// Для сериализации
QDataStream & operator <<(QDataStream &out, const MatrixView &obj)
{
    out << obj.getName()
        << obj.getPageSize()
        << obj.getBanner()
        << obj.getBannerPos()
        << obj.getBannerFontSize()
        << obj.getBasePoints();
    return out;
}

QDataStream & operator >>(QDataStream &in, MatrixView &obj)
{
    QString        name;
    QString        banner;
    QSizeF         size;
    QList<QPointF> points;
    QPointF        banner_pos;
    int            font_size;

    in >> name >> size >> banner >> banner_pos >> font_size >> points;

    obj.setName(name);
    obj.setPaperSize(size);
    obj.setBanner(banner);
    obj.setBasePoints(points);
    obj.setBannerPos(banner_pos);
    obj.setBannerFontSize(font_size);

    return in;
}

///
MatrixView::MatrixView(QString        name,
                       QSizeF         paper_size,
                       QList<QPointF> points,
                       QString        banner,
                       QPointF        banner_pos,
                       int            banner_size)
{
    _name             = name;
    _paper_size       = paper_size;
    _banner           = banner;
    _base_points      = points;
    _banner_pos       = banner_pos;
    _banner_font_size = banner_size;
}

void MatrixView::setBasePoints(QList<QPointF> points)
{
    _base_points = points;
}

void MatrixView::addBasePoint(QPointF point)
{
    _base_points.append(point);
}

void MatrixView::addBasePoints(QList<QPointF> points)
{
    _base_points.append(points);
}

void MatrixView::cleatBasePoints()
{
    _base_points.clear();
}

void MatrixView::setPaperSize(QSizeF size)
{
    _paper_size = size;
}

void MatrixView::setName(QString name)
{
    _name = name;
}

void MatrixView::setBanner(QString banner)
{
    _banner = banner;
}

void MatrixView::setBannerPos(QPointF pos)
{
    _banner_pos = pos;
}

void MatrixView::setBannerFontSize(int size)
{
    _banner_font_size = size;
}

QList<QPointF> MatrixView::getBasePoints() const
{
    return _base_points;
}

QSizeF MatrixView::getPageSize() const
{
    return _paper_size;
}

QString MatrixView::getName() const
{
    return _name;
}

QString MatrixView::getBanner() const
{
    return _banner;
}

QPointF MatrixView::getBannerPos() const
{
    return _banner_pos;
}

int MatrixView::getBannerFontSize() const
{
    return _banner_font_size;
}

