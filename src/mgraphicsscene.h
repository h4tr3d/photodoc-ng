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

private:

private:
    bool  _is_grid_shown;
    qreal _scale;
    qreal _grid_step;

    // опорные точки
    QList<QPointF> _points;

    // параметры рамки фотографии
    PhotoFormat    _format;
};
#endif
