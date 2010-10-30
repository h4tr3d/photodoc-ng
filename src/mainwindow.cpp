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

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSplitter>

#include <iostream>

#include "mainwindow.h"
#include "settings.h"
#include "settingsdialog.h"
#include "formimagematrix.h"
#include "utils.h"
#include "imageprocess.h"
#include "mfiledialog.h"
#include "gphotodialog.h"
#include "helpbrowser/helpbrowser.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);

    _scene = new MGraphicsScene(this);
    _scene->setSceneRect(0, 0, 640, 480);
    view->setScene(_scene);
    _image_item = 0;

    loadFormats();

    // Update angle select
    QStringList angles;
    angles << tr("Without angle")
           << tr("Right")
           << tr("Left")
           << tr("Direct Right")
           << tr("Direct Left")
           << tr("Shading Ellipse");
    angleFormats->addItems(angles);

    resetTransformations();

    loadFile(":/interface/images/logo.png");

    QString title = QString("%1 %2")
                        .arg(qApp->applicationName())
                        .arg(qApp->applicationVersion());
    setWindowTitle(title);

    status_bar->showMessage(tr("Load photo, select three dots: on pate (red), on bridge of nose (green) and on chin (blue) and turn Compose button"));
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::loadFile(QString file_name)
{
    if (file_name.isEmpty())
    {
        MFileDialog dlg(this,
                        tr("Open Image"),
                        "",
                        tr("All Images (*.jpg *.jpeg *.png *.tiff *.bmp)"));

        if (dlg.exec() && dlg.selectedFiles().count() == 1)
        {
            file_name = dlg.selectedFiles().at(0);
        }

        if (file_name.isEmpty())
        {
            return;
        }
    }

    QImage tmp;
    if (!tmp.load(file_name))
    {
        return;
    }

    _original_image = tmp;
    _small_image    = _original_image.scaled(640, 480, Qt::KeepAspectRatio);
    _scene->clear();
    _image_item = _scene->addPixmap(QPixmap::fromImage(_small_image));
    _external_file_name = "";

    resetTransformations();
}

void MainWindow::loadFormats()
{
    QList<QVariant> list = Settings::instance()->getParam("core.formats").toList();
    int count = list.count();

    _formats.clear();
    for (int i = 0; i < count; i++)
    {
        PhotoFormat format = qVariantValue<PhotoFormat>(list.at(i));
        _formats << format;
    }

    updateFormats();
}

void MainWindow::updateFormats()
{
    photoFormats->clear();
    for (int i = 0; i < _formats.count(); i++)
    {
        PhotoFormat format = _formats.at(i);
        QString name = format.getFormatName();
        photoFormats->addItem(name);
    }
}

// TODO: Вынести эту логику в отдельную функцию, оторванную от класса
QImage MainWindow::formCellImage()
{
    QImage img;
    QList<QPointF> dots = _scene->getBasePints();
    if (dots.count() != 3)
    {
        return img;
    }

    int index = photoFormats->currentIndex();
    PhotoFormat format = _formats.at(index);

    QImage original_image = applyGeometryTransformations(_original_image);
    QImage small_image    = applyGeometryTransformations(_small_image);
    small_image           = small_image.scaled(640, 480, Qt::KeepAspectRatio);

    QRectF rect_ = getPhotoRect(original_image.size(),
                                small_image.size(),
                                format,
                                dots);
    QRect rect((int)rect_.x(), (int)rect_.y(), (int)rect_.width(), (int)rect_.height());

    bool ok;
    qreal dpi   = Settings::instance()->getParam("image.dpi").toDouble(&ok);
    if (!ok)
    {
        dpi = 600;
    }

    qreal scale = dpi / 25.4; // DPI to DotsPerMilliMeter

    img = original_image;
    //img = applyTransformations(_original_image);
    //img = _original_image.copy(rect);

    img = img.copy(rect);
    QSizeF tmp_size = format.getSize() * scale;
    QSize  size((int)tmp_size.width(), (int)tmp_size.height());
    img = img.scaled(size);

    img = applyTransformations(img); // Apply transformations

    index = angleFormats->currentIndex(); // Angle on photo
    QPainter painter(&img);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);

    QPolygonF polygon;
    switch (index)
    {
        case 1: // правый
            painter.drawEllipse(QPointF(img.size().width(), img.size().height()),
                                14 * scale,
                                14 * scale);
            break;
        case 2: // левый
            painter.drawEllipse(QPointF(0, img.size().height()),
                                14 * scale,
                                14 * scale);
            break;
        case 3: // прямой правый
            polygon << QPointF(img.size().width(), img.size().height());
            polygon << QPoint(img.size().width(), img.size().height() - 14 * scale);
            polygon << QPoint(img.size().width() - 14 * scale, img.size().height());
            polygon << polygon.at(0);
            painter.drawPolygon(polygon);
            break;
        case 4: // прямой левый
            polygon << QPointF(0, img.size().height());
            polygon << QPoint(0, img.size().height() - 14 * scale);
            polygon << QPoint(14 * scale, img.size().height());
            polygon << polygon.at(0);
            painter.drawPolygon(polygon);
            break;
        case 5: // овал с растушевкой
            QPixmap ellipse;
            ellipse.load(":/process/images/photo/ellipse.png");
            ellipse = ellipse.scaled(size);

            painter.drawPixmap(0, 0, ellipse);
            break;
    }

    return img;
}

QImage MainWindow::applyTransformations(QImage &src)
{
    QImage img = src;

    // Brightness and contrast
    if (_brightness != 0.0 || _contrast != 0.0)
    {
        img = applyBrightnessContrast(img, _brightness, _contrast);
    }

    // BW transform
    if (_is_grayscale)
    {
        img = transformToGrayscale(img);
    }

    return img;
}

QImage MainWindow::applyGeometryTransformations(QImage &src)
{
    QImage img = src;

    // Rotations
    if (_rotate != 0.0)
    {
        QMatrix transform;
        QSize   size = img.size();
        img = img.transformed(transform.rotate(_rotate), Qt::SmoothTransformation);
    }

    return img;
}


void MainWindow::updateImageTransformations()
{
    if (_image_item == 0)
    {
        return;
    }

    QImage img;

    img = applyGeometryTransformations(_original_image);
    img = img.scaled(640, 480, Qt::KeepAspectRatio);
    img = applyTransformations(img);

    _image_item->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::resetTransformations()
{
    _brightness = 0.0;
    _contrast   = 0.0;
    brightness->setValue(0);
    contrast->setValue(0);
    on_brightness_valueChanged(0);
    on_contrast_valueChanged(0);
    _need_update = true;

    _is_grayscale = false;
    setBW->setChecked(_is_grayscale);

    _rotate = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Slots for signals
////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::on_openFile_clicked()
{
    loadFile();
}

void MainWindow::on_photoFormats_activated(int index)
{
    PhotoFormat format = _formats.at(index);
    _scene->setPhotoParams(format);
}

void MainWindow::on_settingsDialog_clicked()
{
    SettingsDialog dialog(this);

    dialog.exec();

    // Update format settings
    loadFormats();
}

void MainWindow::on_composePhoto_clicked()
{
    QImage img = formCellImage();
    if (img.isNull())
    {
        return;
    }

    int index = photoFormats->currentIndex();
    PhotoFormat format = _formats.at(index);

    FormImageMatrix dialog(this, img, _external_file_name, format.getFormatName());
    dialog.exec();
}

void MainWindow::on_passToEditor_clicked()
{
    QImage img = formCellImage();
    if (img.isNull())
    {
        return;
    }

    QString editor_name = Settings::instance()->getParam("image.editor").toString();
    if (editor_name.isEmpty())
    {
        QMessageBox::information(this, tr("Editor error"), tr("Editor commad does not specified in configuration"));
        return;
    }

    QString temp_file = QString("%1/%2-%3")
                                .arg(QDir::tempPath())
                                .arg(qApp->applicationName())
                                .arg(getUserName());
    QString format = Settings::instance()->getParam("image.format").toString();
    temp_file += "." + format;

    img.save(temp_file);
    _external_file_name = temp_file;

    // Запуск
    QStringList args;
    args << temp_file;
    QProcess::startDetached(editor_name, args);
}

void MainWindow::on_brightness_valueChanged(int value)
{
    brightnessValue->setText(QString("%1").arg(value));
    _brightness = (qreal)value / 30.0; // move to [-1..1] range
    if (_need_update)
        updateImageTransformations();
}

void MainWindow::on_contrast_valueChanged(int value)
{
    contrastValue->setText(QString("%1").arg(value));
    _contrast = (qreal)value / 30.0; // move to [-1..1] range
    if (_need_update)
        updateImageTransformations();
}

void MainWindow::on_brightness_sliderPressed()
{
    _need_update = false;
}

void MainWindow::on_brightness_sliderReleased()
{
    _need_update = true;
    updateImageTransformations();
}

void MainWindow::on_contrast_sliderPressed()
{
    _need_update = false;
}

void MainWindow::on_contrast_sliderReleased()
{
    _need_update = true;
    updateImageTransformations();
}

void MainWindow::on_setBW_clicked(bool checked)
{
    _is_grayscale = checked;
    updateImageTransformations();
}

void MainWindow::on_resetChanges_clicked()
{
    resetTransformations();
    updateImageTransformations();
}

void MainWindow::on_rotateLeft_clicked()
{
    if (_rotate == 0.0)
        _rotate = 360.0;
    _rotate -= 90.0;
    updateImageTransformations();
}

void MainWindow::on_rotateRight_clicked()
{
    _rotate += 90.0;
    if (_rotate >= 360.0)
        _rotate = 0.0;
    updateImageTransformations();
}


void MainWindow::on_captureFile_clicked()
{
    GphotoDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        QString file_name = dlg.getDowloadedFileName();
        loadFile(file_name);
    }
}

void MainWindow::on_helpBrowser_clicked()
{
    // TODO: create getName() function for get localized or standart name
    QString locale_name = QLocale::system().name();
    QString readme      = "help";
    QString sys_dir     = PREFIX"/share/photodoc";
    QString doc_path    = sys_dir + "/" + readme + "_" + locale_name + ".html";

    if (!QFile::exists(doc_path))
    {
        doc_path = readme + "_" + locale_name + ".html";
        if (!QFile::exists(doc_path))
        {
            doc_path = sys_dir + "/" + readme + ".html";
            if (!QFile::exists(doc_path))
            {
                doc_path = readme + ".html";
            }
        }
    }

    HelpBrowser::showPage(doc_path);
}
