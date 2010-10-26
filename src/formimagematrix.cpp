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

#include <QDateTime>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFileInfo>
#include <QPrintDialog>
#include <QPrintPreviewDialog>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

#include "formimagematrix.h"

FormImageMatrix::FormImageMatrix(QWidget *parent,
                                 QImage   cell,
                                 QString  external_name,
                                 QString  document_type) : QDialog(parent)
{
    setupUi(this);

    _settings = Settings::instance();

    _single_image       = cell;
    _external_file_name = external_name;

    bool ok;
    _dpi = _settings->getParam("image.dpi").toDouble(&ok);
    if (!ok)
    {
        _dpi = 600;
    }

    _scene = new QGraphicsScene(this);
    _scene->setSceneRect(0, 0, 200, 300);
    graphicsView->setScene(_scene);

    loadMatrices();

    _printer = 0;
    _document_type = document_type;
}

void FormImageMatrix::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormImageMatrix::loadMatrices()
{
    QList<QVariant> list = Settings::instance()->getParam("core.matrix").toList();
    int count = list.count();

    _matrices.clear();
    matrixType->clear();
    for (int i = 0; i < count; i++)
    {
        _matrices << qVariantValue<MatrixView>(list.at(i));
        matrixType->addItem(_matrices.last().getName());
    }
}

void FormImageMatrix::updateMatrix()
{
    if (_work_image.isNull())
    {
        return;
    }

    int index             = matrixType->currentIndex();
    MatrixView matrix     = _matrices.at(index);
    QList<QPointF> points = matrix.getBasePoints();

    _scene->clear();

    qreal scale = _dpi / 25.4; // Translate DPI to DotsPerMilliMeter: all our size in mm

    QSizeF size = matrix.getPageSize() * scale;
    _target_pix = QPixmap((int)size.width(), (int)size.height());
    _target_pix.fill(QColor(Qt::white));
    QPainter painter(&_target_pix);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < points.count(); i++)
    {
        painter.drawImage(points.at(i) * scale, _work_image);
    }

    QString text = matrix.getBanner();
    QDateTime date = QDateTime::currentDateTime();
    text.replace("{DATE}", date.toString("yyyy-MM-dd"));
    text.replace("{TIME}", date.toString("hh:mm:ss"));

    painter.setFont(QFont("Arial", matrix.getBannerFontSize() * scale));
    painter.drawText(matrix.getBannerPos() * scale, text);

    QPixmap small = _target_pix.scaled(200, 300, Qt::KeepAspectRatio);
    _scene->addPixmap(small);

    // Exif info
    QString date_str = date.toString("yyyy-MM-dd") + " " + date.toString("hh:mm:ss");
    exifDate->setText(date_str);
    exifType->setText(_document_type);
}

QPrinter *FormImageMatrix::getPrinter()
{
    if (_printer == 0)
    {
        _printer = new QPrinter(QPrinter::HighResolution);
    }

    int index             = matrixType->currentIndex();
    MatrixView matrix     = _matrices.at(index);

    _printer->setPaperSize(matrix.getPageSize(), QPrinter::Millimeter);
    _printer->setResolution((int)_dpi);
    _printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);

    return _printer;
}

void FormImageMatrix::drawPrinter(QPrinter *printer)
{
    QPainter painter;
    painter.begin(printer);
    painter.drawPixmap(0, 0, _target_pix);
    painter.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////

void FormImageMatrix::on_matrixClose_clicked()
{
    close();
}

void FormImageMatrix::on_matrixType_currentIndexChanged(int /*index*/)
{
    updateMatrix();
}

void FormImageMatrix::on_composePhoto_clicked()
{
    if (_single_image.isNull())
    {
        return;
    }

    _work_image = _single_image;
    updateMatrix();
}

void FormImageMatrix::on_passToEditor_clicked()
{
    if (_external_file_name.isEmpty())
    {
        return;
    }

    _work_image.load(_external_file_name);
    updateMatrix();
}

void FormImageMatrix::on_matrixSave_clicked()
{
    if (_target_pix.isNull())
    {
        return;
    }

    QString format = _settings->getParam("image.format").toString();
    if (format.isEmpty())
    {
        format = "jpeg";
    }

    QDateTime date = QDateTime::currentDateTime();
    QString file_name = QString("%1_%2.%3")
                            .arg(date.toString("yyyy-MM-dd"))
                            .arg(date.toString("hh-mm-ss"))
                            .arg(format);

    QFileDialog dlg(this, tr("Save image as"), QString(), "*");
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDefaultSuffix(format);
    dlg.selectFile(file_name);

    if (dlg.exec() && dlg.selectedFiles().count() == 1)
    {
        file_name = dlg.selectedFiles().at(0);
        _target_pix.save(file_name);

        // Write Exif
        Exiv2::ExifData exif_data;
        exif_data["Exif.Image.ImageDescription"] = exifName->text().toUtf8().data();
        exif_data["Exif.Image.DocumentName"]     = exifType->text().toUtf8().data();
        exif_data["Exif.Image.DateTime"]         = exifDate->text().toUtf8().data();
        exif_data["Exif.Image.DateTimeOriginal"] = exifDate->text().toUtf8().data();
        exif_data["Exif.Photo.DateTimeOriginal"] = exifDate->text().toUtf8().data();
        exif_data["Exif.Photo.UserComment"]      = exifComment->toPlainText().toUtf8().data();
        exif_data["Exif.Image.Software"]         = QString(qApp->applicationName() +
                                                           " " +
                                                           qApp->applicationVersion()).toUtf8().data();
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(file_name.toUtf8().data());
        image->setExifData(exif_data);
        image->writeMetadata();

        if (openDir->isChecked())
        {
            QFileInfo fi(file_name);
            QString dir_name = fi.absolutePath();
            QDesktopServices::openUrl(QUrl(dir_name, QUrl::TolerantMode));
        }
    }
}

void FormImageMatrix::on_matrixPrint_clicked()
{
    if (_target_pix.isNull())
    {
        return;
    }

    QPrinter *printer = getPrinter();
    QPrintDialog *dlg = new QPrintDialog(printer, this);
    dlg->setWindowTitle(tr("Print photo matrix"));
    dlg->setOption(QAbstractPrintDialog::PrintShowPageSize, true);
    dlg->setOption(QAbstractPrintDialog::PrintToFile, true);

    if (dlg->exec() == QDialog::Accepted)
    {
        drawPrinter(printer);
    }

    delete dlg;
}

void FormImageMatrix::on_matrixPrintPrview_clicked()
{
    if (_target_pix.isNull())
    {
        return;
    }

    QPrinter *printer = getPrinter();
    QPrintPreviewDialog preview(printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void FormImageMatrix::printPreview(QPrinter *printer)
{
    drawPrinter(printer);
}

