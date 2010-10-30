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
**   @file   gphotodialog.h
**   @date   2010-10-31
**   @author hatred
**   @brief  GPhoto download dialog
**
**************************************************************************/

#ifndef GPHOTODIALOG_H
#define GPHOTODIALOG_H

#include "ui_gphotodialog.h"
#include "gphoto2/gpcamera.h"

class GphotoDialog : public QDialog, private Ui::GphotoDialog
{
    Q_OBJECT

public:
    explicit GphotoDialog(QWidget *parent = 0);
    ~GphotoDialog();
    QString getDowloadedFileName();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_cameraRefresh_clicked();
    void on_cameraDirs_currentRowChanged(int current_row);
    void on_cameraItems_currentRowChanged(int current_row);
    void on_cameraList_currentIndexChanged(int index);
    void on_itemGet_clicked();
    void on_itemDel_clicked();

private:
    void refreshCameraList();
    void errorDialog();

private:
    GPCamera    *_camera;
    QStringList  _models;
    QStringList  _ports;
    bool         _is_connected;
    QString      _downloaded_file_name;
};

#endif // GPHOTODIALOG_H
