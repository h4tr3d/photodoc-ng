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
**   @file   mainwindow.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Main Window dialog
**
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsPixmapItem>
#include <QShortcut>

#include "ui_mainwindow.h"
#include "mgraphicsscene.h"
#include "photoformat.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    void on_helpBrowser_clicked();

    void on_saveFile_clicked();

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

    // Set shortcuts
    void setShortcuts();

private:
    QImage               _original_image;
    QImage               _small_image;
    MGraphicsScene      *_scene;
    QGraphicsPixmapItem *_image_item;

    QList<PhotoFormat> _formats;
    QList<QShortcut*>  _shortcuts;

    QString            _external_file_name;

    // Image transforms
    bool              _need_update;
    qreal             _brightness;
    qreal             _contrast;
    qreal             _rotate;
    bool              _is_grayscale;
};

#endif // MAINWINDOW_H
