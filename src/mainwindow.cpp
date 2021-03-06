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

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSplitter>
#include <QVariant>
#include <QPaintEvent>

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
    //_scene->setSceneRect(0, 0, 640, 480);
    view->setScene(_scene);
    _image_item  = 0;
    _need_update = true; // must be placed before loadFormats() and filling angles
                         // to prevent use as undefinned in
                         //     on_brightness_valueChanged(int) and
                         //     on_contast_valueChanged(int)

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

    setShortcuts();
}

MainWindow::~MainWindow()
{
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

void MainWindow::paintEvent(QPaintEvent* /*e*/)
{
    QSize newSize = view->viewport()->geometry().size();
    if (newSize != _viewSize) {
        _viewSize = newSize;
        qDebug() << "onPaint:" << view->sceneRect();
        view->setSceneRect(QRectF(0, 0, _viewSize.width(), _viewSize.height()));
        view->updateSceneRect(QRectF(0, 0, _viewSize.width(), _viewSize.height()));
        _scene->setSceneRect(QRectF(0, 0, _viewSize.width(), _viewSize.height()));
        qDebug() << "onPaint:" << view->sceneRect();
        if (_image_item) {
            updateImageTransformations();
        }
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
    _small_image    = _original_image.scaled(_viewSize.width(), _viewSize.height(), Qt::KeepAspectRatio);
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
        PhotoFormat format = list.at(i).value<PhotoFormat>();
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
    if (index < 0 || index >= _formats.count())
    {
        return img;
    }
    PhotoFormat format = _formats.at(index);

    QImage original_image = applyGeometryTransformations(_original_image);
    QImage small_image    = applyGeometryTransformations(_small_image);
    small_image           = small_image.scaled(_viewSize.width(), _viewSize.height(), Qt::KeepAspectRatio);

    if (_image_item && _image_item->offset() != QPointF()) {
        auto const offset = _image_item->offset();
        for (auto& dot : dots) {
            dot.setX(dot.x() - offset.x());
            dot.setY(dot.y() - offset.y());
        }
    }

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
    img = img.scaled(_viewSize.width(), _viewSize.height(), Qt::KeepAspectRatio);
    img = applyTransformations(img);

    _image_item->setPixmap(QPixmap::fromImage(img));

    qreal offsetx = 0.0;
    if (img.width() < _viewSize.width()) {
        offsetx = (_viewSize.width() - img.width()) / 2.0;
    }

    qreal offsety = 0.0;
    if (img.height() < _viewSize.height()) {
        offsety = (_viewSize.height() - img.height()) / 2.0;
    }

    _image_item->setOffset(offsetx, offsety);
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

void MainWindow::setShortcuts()
{
    QShortcut *shortcut;
    _shortcuts.clear(); // ???

    // Quit
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_Q, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(close()));
    _shortcuts << shortcut;

    // Pass to editor
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_E, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(on_passToEditor_clicked()));
    _shortcuts << shortcut;

    // Open
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_O, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(on_openFile_clicked()));
    _shortcuts << shortcut;

    // Save
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_S, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(on_saveFile_clicked()));
    _shortcuts << shortcut;

    // Open from gphoto
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_G, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(on_captureFile_clicked()));
    _shortcuts << shortcut;

    // Compose
    shortcut = new QShortcut(Qt::CTRL + Qt::Key_Space, this);
    connect(shortcut, SIGNAL(activated()),
            this,     SLOT(on_composePhoto_clicked()));
    _shortcuts << shortcut;

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
    if (index < 0 || index >= _formats.count())
    {
        return;
    }

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


void MainWindow::on_saveFile_clicked()
{
    QString     file_name;
    QString     suffix;
    MFileDialog dlg(this,
                    tr("Save Image"),
                    "",
                    tr("All Images (*.jpg *.jpeg *.png *.tiff *.bmp)"));

    suffix = Settings::instance()->getParam("image.format").toString();
    if (suffix.isEmpty())
    {
        suffix = "jpeg";
    }

    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setDefaultSuffix(suffix);

    if (dlg.exec() && dlg.selectedFiles().count() == 1)
    {
        file_name = dlg.selectedFiles().at(0);
    }

    if (file_name.isEmpty())
    {
        return;
    }

    // Apply transformations and save
    QImage img;
    img = applyGeometryTransformations(_original_image);
    img = applyTransformations(img);

    img.save(file_name, 0, 100);
}
