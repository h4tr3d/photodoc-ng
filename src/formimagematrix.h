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
**   @file   formimagematrix.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Photo matrix form dialog
**
**************************************************************************/

#ifndef FORMIMAGEMATRIX_H
#define FORMIMAGEMATRIX_H

#include <QPrinter>
#include <QShortcut>

#include "ui_formimagematrix.h"

#include "settings.h"
#include "matrixview.h"

class FormImageMatrix : public QDialog, private Ui::FormImageMatrix
{
    Q_OBJECT

public:
    explicit FormImageMatrix(QWidget *parent        = 0,
                             QImage   cell          = QImage(),
                             QString  external_name = QString(),
                             QString  document_type = QString());
    ~FormImageMatrix();

protected:
    void changeEvent(QEvent *e);

private slots:
    void printPreview(QPrinter *printer);
    void on_matrixClose_clicked();
    void on_matrixType_currentIndexChanged(int index);
    void on_composePhoto_clicked();
    void on_passToEditor_clicked();
    void on_matrixSave_clicked();
    void on_matrixPrint_clicked();
    void on_matrixPrintPrview_clicked();

private:
    void      loadMatrices();
    void      updateMatrix();
    QPrinter *getPrinter();
    void      drawPrinter(QPrinter *printer);

private:
    Settings       *_settings;
    QGraphicsScene *_scene;

    QString _document_type;

    QString _external_file_name;
    QImage  _single_image;
    QImage  _work_image;
    QPixmap _target_pix;
    QString _saved_image_file_name;
    qreal   _dpi;

    QList<MatrixView> _matrices;

    QPrinter *_printer;

    QList<QShortcut*> _shortcuts;

};

#endif // FORMIMAGEMATRIX_H
