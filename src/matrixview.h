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
**   @file   matrixview.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Class that stored information about matrix view:
**           page size, name, banner text and so on
**
**************************************************************************/

#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QtCore>

class MatrixView
{
public:
    MatrixView(QString        name        = QString(),
               QSizeF         paper_size  = QSizeF(),
               QList<QPointF> points      = QList<QPointF>(),
               QString        banner      = QString(),
               QPointF        banner_pos  = QPointF(),
               int            banner_size = 0);

    void setBasePoints(QList<QPointF> points);
    void addBasePoint(QPointF point);
    void addBasePoints(QList<QPointF> points);
    void cleatBasePoints();

    void setPaperSize(QSizeF size);
    void setName(QString name);
    void setBanner(QString banner);
    void setBannerPos(QPointF pos);
    void setBannerFontSize(int size);

    QList<QPointF> getBasePoints() const;
    QSizeF         getPageSize() const;
    QString        getName() const;
    QString        getBanner() const;
    QPointF        getBannerPos() const;
    int            getBannerFontSize() const;

private:
    QList<QPointF> _base_points;      // Точки вершин фотографий, мм
    QSizeF         _paper_size;       // Размер листа, мм
    QString        _name;             // Название набора
    QString        _banner;           // Банерный текст
    QPointF        _banner_pos;       // Позиция банера
    int            _banner_font_size; // Размер шрифта банера в мм
};

// Для использования в настройках
Q_DECLARE_METATYPE(MatrixView);

QDataStream &operator<<(QDataStream &out, const MatrixView &obj);
QDataStream &operator>>(QDataStream &in, MatrixView &obj);

#endif // MATRIXVIEW_H
