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
**   @file   mgraphicsschene.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Graphics schene sclass that allowed users iteractions like
**           dot pasting
**
**************************************************************************/

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "mgraphicsscene.h"

//
MGraphicsScene::MGraphicsScene( QObject *parent ) : QGraphicsScene(parent)
{
    _scale         = 1.0;
    _is_grid_shown = false;
    _grid_step     = 20.0;

    // Дефолтные параметры для 'Документы (3x4)'
    _format.setSize(QSizeF(30, 40));
    _format.setTopPateDistance(6);
    _format.setNoseChinDistance(9);

    _sceneRect = sceneRect();

    connect(this, &QGraphicsScene::sceneRectChanged, this, [this](const QRectF &rect) {
        if (!_points.empty()) {
            //auto xOffset = rect.x() - _sceneRect.x();
            auto xScale = rect.width() / _sceneRect.width();
            //auto yOffset = rect.y() - _sceneRect.y();
            auto yScale = rect.height() / _sceneRect.height();
            for (auto &point : _points) {
                point.setX(point.x() * xScale);
                point.setY(point.y() * yScale);
            }
            update();
        }
        _sceneRect = rect;
    });
}

void MGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);

    if (mouseEvent->button() == Qt::LeftButton)
    {
        QPointF point = mouseEvent->scenePos();

        if (_points.count() >= 3)
        {
            _points.clear();
        }

        _points << point;
        update();
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        removeLastPoint();
    }
}

void MGraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Escape)
    {
        removeLastPoint();
    }
}

void MGraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QColor grid_color(100, 100, 100, 127);
    painter->setPen(grid_color);

    painter->setRenderHint(QPainter::Antialiasing);

    // рамка
    painter->drawRect(sceneRect());

    // сетка
    if (_is_grid_shown)
    {
        qreal x         = sceneRect().x();
        qreal y         = sceneRect().y();
        qreal w         = sceneRect().width();
        qreal h         = sceneRect().height();
        qreal grid_step = _grid_step * _scale;
        qreal i;

        // вертикальная сетка
        for (i = grid_step; i < w; i += grid_step)
        {
            painter->drawLine(x + i, y, x + i, y + h);
        }

        // горизонтальная сетка
        for (i = grid_step; i < h; i += grid_step)
        {
            painter->drawLine(x, y + i, x + w, y + i);
        }
    }

    // Опорные точки
    QList<QColor> dot_colors;
    dot_colors << QColor(Qt::red) << QColor(Qt::blue) << QColor(Qt::green);
    for (int i = 0; i < _points.count(); i++)
    {
        painter->setBrush(QBrush(dot_colors.at(i)));
        painter->drawEllipse(_points.at(i), 2.5, 2.5);
    }

    // Обрамляющий прямоугольник
    if (_points.count() == 3)
    {
        painter->setPen(QColor(Qt::red));
        painter->setBrush(QBrush());

        QRectF rect = getPhotoRect(sceneRect().size(),
                                   sceneRect().size(),
                                   _format,
                                   _points);

        painter->drawRect(rect);
    }

    QGraphicsScene::drawForeground(painter, rect);
}

void MGraphicsScene::removeLastPoint()
{
    if (_points.count() > 0)
    {
        _points.removeLast();
    }
    update();
}

void MGraphicsScene::showGrid(bool is_grid_shown)
{
    _is_grid_shown = is_grid_shown;
    update();
}

void MGraphicsScene::setScaleFactor(qreal scale)
{
    _scale = scale;
}

qreal MGraphicsScene::getScaleFactor()
{
    return _scale;
}

void MGraphicsScene::setGridStep(qreal step)
{
    _grid_step = step;
}

qreal MGraphicsScene::getGridStep()
{
    return _grid_step;
}

void MGraphicsScene::setPhotoParams(PhotoFormat format)
{
    _format = format;
    update();
}

bool MGraphicsScene::isComplete()
{
    return (_points.count() == 3);
}

QList<QPointF> MGraphicsScene::getBasePints()
{
    return _points;
}

