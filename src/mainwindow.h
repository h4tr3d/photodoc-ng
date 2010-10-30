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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsPixmapItem>

#include "ui_mainwindow.h"
#include "mgraphicsscene.h"
#include "photoformat.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_openFile_clicked();
    void on_photoFormats_activated(int index);
    void on_settingsDialog_clicked();
    void on_composePhoto_clicked();
    void on_passToEditor_clicked();
    void on_brightness_valueChanged(int value);
    void on_contrast_valueChanged(int value);
    void on_brightness_sliderPressed();
    void on_brightness_sliderReleased();
    void on_contrast_sliderPressed();
    void on_contrast_sliderReleased();
    void on_setBW_clicked(bool checked);
    void on_resetChanges_clicked();
    void on_rotateLeft_clicked();
    void on_rotateRight_clicked();

    void on_captureFile_clicked();

private:
    void loadFile(QString file_name = QString());

    // читаем форматы из конфигурационного файла
    void loadFormats();

    // обновляем информацию о форматах в окне
    void updateFormats();

    // Формируем картинку для одной ячейки в матрице
    QImage formCellImage();

    // Update image trasnformation
    void updateImageTransformations();

    // Reset all transformations
    void resetTransformations();

    // Apply transformations
    QImage applyTransformations(QImage &src);

    // Geometry transformation in different function
    QImage applyGeometryTransformations(QImage &src);

private:
    QImage               _original_image;
    QImage               _small_image;
    MGraphicsScene      *_scene;
    QGraphicsPixmapItem *_image_item;

    QList<PhotoFormat> _formats;

    QString            _external_file_name;

    // Image transforms
    bool              _need_update;
    qreal             _brightness;
    qreal             _contrast;

    bool              _is_grayscale;

    qreal             _rotate;
};

#endif // MAINWINDOW_H
