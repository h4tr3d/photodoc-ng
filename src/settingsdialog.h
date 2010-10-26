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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ui_settingsdialog.h"
#include "settings.h"
#include "photoformat.h"
#include "matrixview.h"

class SettingsDialog : public QDialog, private Ui::SettingsDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_settingsButtons_clicked(QAbstractButton* button);
    void on_formatsList_currentRowChanged(int current_row);
    void on_formatName_textChanged(QString );
    void on_formatDel_clicked();
    void on_formatSave_clicked();
    void on_formatAdd_clicked();
    void on_matrixList_currentRowChanged(int currentRow);
    void on_matrixName_textChanged(QString );
    void on_matrixDel_clicked();
    void on_matrixSave_clicked();
    void on_matrixAdd_clicked();
    void on_matrixPosDel_clicked();
    void on_matrixPosAdd_clicked();

    void on_externalEditorSelect_clicked();

private:
    void        saveSettings();
    void        loadSettings();

    void        updateFormatEditor(int index);
    PhotoFormat fillFormat();

    void        updateMatrixEditor(int index);
    MatrixView  fillMatrix();

private:
    Settings *_settings;

    QList<PhotoFormat> _formats;
    QList<MatrixView>  _matrices;
    QList<QPointF>     _matrix_pos;

};

#endif // SETTINGSDIALOG_H
