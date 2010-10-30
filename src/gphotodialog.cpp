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
**   @file   gphotodialog.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  GPhoto download dialog
**
**************************************************************************/

#include <QImage>
#include <QMessageBox>
#include <QDir>

#include <iostream>

#include "gphotodialog.h"
#include "utils.h"

GphotoDialog::GphotoDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    _camera       = 0;
    _is_connected = false;

    refreshCameraList();
}

GphotoDialog::~GphotoDialog()
{
    if (_camera != NULL)
    {
        delete _camera;
    }
}

void GphotoDialog::changeEvent(QEvent *e)
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

void GphotoDialog::refreshCameraList()
{
    QString print;

    _models.clear();
    _ports.clear();
    cameraList->clear();

    GPCamera::autoDetect(_models, _ports);
    for (int i = 0; i < _models.count(); i++)
    {
        print = _models.at(i) + " : " + _ports.at(i);
        cameraList->addItem(print);
    }

    itemGet->setEnabled(false);
    itemDel->setEnabled(false);
}

void GphotoDialog::errorDialog()
{
    QMessageBox::critical(this, tr("Camera error"), tr("Unsupported operation"));
}

QString GphotoDialog::getDowloadedFileName()
{
    return _downloaded_file_name;
}

/**************************************************************************************************/
/* SLOTS -----------------------------------------------------------------------------------------*/
/**************************************************************************************************/

void GphotoDialog::on_cameraList_currentIndexChanged(int index)
{
    if (index < 0)
    {
        return;
    }

    QString model = _models.at(index);
    QString port  = _ports.at(index);

    if (_camera != NULL)
    {
        delete _camera;
    }

    _camera = new GPCamera(model, model, port, "/");
    if (_camera == NULL)
    {
        return;
    }

    _is_connected = _camera->doConnect();
    if (!_is_connected)
    {
        return;
    }

    QStringList dirs;
    _camera->getAllFolders("/", dirs);

    cameraDirs->clear();
    cameraDirs->addItems(dirs);
}

void GphotoDialog::on_cameraRefresh_clicked()
{
    refreshCameraList();
}

void GphotoDialog::on_cameraDirs_currentRowChanged(int current_row)
{
    if (current_row < 0)
    {
        return;
    }

    QStringList items;
    QString     dir = cameraDirs->item(current_row)->text();
    _camera->getItemsList(dir, items);

    itemGet->setEnabled(false);
    itemDel->setEnabled(false);

    cameraItems->clear();
    cameraItems->addItems(items);
}

void GphotoDialog::on_cameraItems_currentRowChanged(int current_row)
{
    if (current_row < 0)
    {
        return;
    }

    QString     dir     = cameraDirs->currentItem()->text();
    QString     file    = cameraItems->item(current_row)->text();
    QImage      preview;

    if (_camera->getThumbnail(dir, file, preview))
    {
        QSize size = previewArea->size();
        preview = preview.scaled(size, Qt::KeepAspectRatio);
        previewArea->setPixmap(QPixmap::fromImage(preview));
    }

    itemGet->setEnabled(true);
    itemDel->setEnabled(true);
}

void GphotoDialog::on_itemGet_clicked()
{
    QString     dir     = cameraDirs->currentItem()->text();
    QString     file    = cameraItems->currentItem()->text();
    //int         row     = cameraDirs->currentRow();

    QString temp_file = QString("%1/%2-%3-%4")
                            .arg(QDir::tempPath())
                            .arg(qApp->applicationName())
                            .arg(getUserName())
                            .arg(file);

    if (_camera->downloadItem(dir, file, temp_file))
    {
        std::cout << "Downloaded: " << qPrintable(temp_file) << "\n";
        _downloaded_file_name = temp_file;
        accept();
    }
    else
    {
        errorDialog();
    }

}

void GphotoDialog::on_itemDel_clicked()
{
    QString     dir     = cameraDirs->currentItem()->text();
    QString     file    = cameraItems->currentItem()->text();
    int         row     = cameraDirs->currentRow();

    int button = QMessageBox::question(this,
                                       tr("Delete file?"),
                                       tr("Delete file '%1' from camera?").arg(file),
                                       QMessageBox::Yes|QMessageBox::No,
                                       QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        if (_camera->deleteItem(dir, file))
        {
            on_cameraDirs_currentRowChanged(row);
        }
        else
        {
            errorDialog();
        }
    }

}
