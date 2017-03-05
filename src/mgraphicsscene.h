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
**   @file   mgraphicsschene.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Graphics schene sclass that allowed users iteractions like
**           dot pasting
**
**************************************************************************/

#ifndef MGRAPHICSSCENE_H
#define MGRAPHICSSCENE_H
//
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "utils.h"

//
// Наш класс основанный на QGraphicsScene
//
class MGraphicsScene : public QGraphicsScene
{
Q_OBJECT

public:
    MGraphicsScene(QObject *parent = 0);

    /**
     Показывает или скрывает координатную сетку
     @param is_grid_show - true - показывать, false - скрывать
     */
    void  showGrid(bool is_grid_shown);

    /**
     Устанавливает масштабный коэффициент - рассчиывается скольки точкам соответствует заданная
     единица измерения
     @param scale - величина коэффициента
     */
    void  setScaleFactor(qreal scale);

    /**
     Возвращает величину масштабного коэффициента
     @return вещественное число - масштабный коэффициент
     */
    qreal getScaleFactor();

    /**
     Устанавливает шаг сетки на экране, задаётся в условных единицах
     @param step - величина шага
     */
    void  setGridStep(qreal step);

    /**
     Возвращает величину шага сетки
     @return вещественное число - шаг сетки
     */
    qreal getGridStep();

    /**
     Устанавливает параметры фотографии, что бы можно было отображать прямоугольник предпросмотра
     @param - структура типа PhotoFormat, задающая формат фотографии
     */
    void setPhotoParams(PhotoFormat format);

    /**
     Проверка, что все опорные точки поставлены
     @return true - если стоят все три опорные точки, false в ином случае
     */
    bool isComplete();

    /**
     Возвращает список значений опорных точек
     @return список значений опорных точек
     */
    QList<QPointF> getBasePints();

protected:
    void drawForeground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

private:
    void removeLastPoint();

private:
    bool  _is_grid_shown;
    qreal _scale;
    qreal _grid_step;

    // опорные точки
    QList<QPointF> _points;
    QRectF _sceneRect;

    // параметры рамки фотографии
    PhotoFormat    _format;
};
#endif
