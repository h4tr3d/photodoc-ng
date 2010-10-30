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
**   @file   settingsdialog.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Settings dialog
**
**************************************************************************/

#include <QFileDialog>

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    _settings = Settings::instance();
    loadSettings();
}

void SettingsDialog::changeEvent(QEvent *e)
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

void SettingsDialog::on_settingsButtons_clicked(QAbstractButton* button)
{
    switch (settingsButtons->standardButton(button))
    {
        case QDialogButtonBox::Save:
            saveSettings();
            _settings->save();
            break;

        case QDialogButtonBox::Ok:
            saveSettings();
            accept();
            break;

        case QDialogButtonBox::Cancel:
            reject();
            break;

        default:        /* prevent wrnings regard Unhandled emum items */
            break;
    }
}

void SettingsDialog::saveSettings()
{
    int i;

    //
    // Common page
    //
    _settings->setParam("image.editor", externalEditorName->text());
    _settings->setParam("image.dpi",    imageDpi->value());
    _settings->setParam("image.format", imageFormat->currentText());

    //
    // Format page
    //
    QList<QVariant> formats;
    for (i = 0; i < _formats.count(); i++)
    {
        formats << qVariantFromValue(_formats.at(i));
    }
    _settings->setParam("core.formats", formats);

    //
    // Matrices page
    //
    QList<QVariant> matrices;
    for (i = 0; i < _matrices.count(); i++)
    {
        matrices << qVariantFromValue(_matrices.at(i));
    }
    _settings->setParam("core.matrix", matrices);
}

void SettingsDialog::loadSettings()
{
    int i;

    //
    // Common page
    //
    QString editor = _settings->getParam("image.editor").toString();
    if (!editor.isEmpty())
    {
        externalEditorName->setText(editor);
    }

    bool ok;
    qreal dpi = _settings->getParam("image.dpi").toDouble(&ok);
    if (ok == true)
    {
        imageDpi->setValue(dpi);
    }

    QString image_format = _settings->getParam("image.format").toString();
    i = imageFormat->findText(image_format);
    if (i != -1)
    {
        imageFormat->setCurrentIndex(i);
    }

    //
    // Formats page
    //
    QList<QVariant> formats = _settings->getParam("core.formats").toList();
    _formats.clear();
    for (i = 0; i < formats.count(); i++)
    {
        _formats << qVariantValue<PhotoFormat>(formats.at(i));
        formatsList->addItem(_formats.last().getFormatName());
    }

    //
    // Matrices page
    //
    QList<QVariant> matrices = _settings->getParam("core.matrix").toList();
    _matrices.clear();
    for (i = 0; i < matrices.count(); i++)
    {
        _matrices << qVariantValue<MatrixView>(matrices.at(i));
        matrixList->addItem(_matrices.last().getName());
    }

}

void SettingsDialog::updateFormatEditor(int index)
{
    if (index < 0 || index >= _formats.count())
    {
        return;
    }

    PhotoFormat format = _formats.at(index);

    formatName->setText(format.getFormatName());
    formatWidth->setValue(format.getSize().width());
    formatHeight->setValue(format.getSize().height());
    formatTopToPateDistance->setValue(format.getTopPateDistance());
    formatNoseToChinDistance->setValue(format.getNoseChinDistance());
}

PhotoFormat SettingsDialog::fillFormat()
{
    PhotoFormat format;

    format.setFormanName(formatName->text());
    format.setSize(QSizeF(formatWidth->value(), formatHeight->value()));
    format.setTopPateDistance(formatTopToPateDistance->value());
    format.setNoseChinDistance(formatNoseToChinDistance->value());

    return format;
}

void SettingsDialog::updateMatrixEditor(int index)
{
    if (index < 0 || index >= _matrices.count())
    {
        return;
    }

    MatrixView matrix = _matrices.at(index);

    matrixName->setText(matrix.getName());
    matrixPaperWidth->setValue(matrix.getPageSize().width());
    matrixPaperHeight->setValue(matrix.getPageSize().height());
    matrixBannerText->setText(matrix.getBanner());
    matrixBannerX->setValue(matrix.getBannerPos().x());
    matrixBannerY->setValue(matrix.getBannerPos().y());
    matrixBannerFontSize->setValue(matrix.getBannerFontSize());

    _matrix_pos = matrix.getBasePoints();
    matrixPosList->clear();
    for (int i = 0; i < _matrix_pos.count(); i++)
    {
        QString text = QString("X%1 %2, Y%1 %3")
                        .arg(QString::fromUtf8("→"))
                        .arg(_matrix_pos.at(i).x())
                        .arg(_matrix_pos.at(i).y());
        matrixPosList->addItem(text);
    }
}

MatrixView SettingsDialog::fillMatrix()
{
    MatrixView matrix;

    matrix.setName(matrixName->text());
    matrix.setPaperSize(QSizeF(matrixPaperWidth->value(), matrixPaperHeight->value()));
    matrix.setBasePoints(_matrix_pos);
    matrix.setBanner(matrixBannerText->text());
    matrix.setBannerPos(QPointF(matrixBannerX->value(), matrixBannerY->value()));
    matrix.setBannerFontSize(matrixBannerFontSize->value());

    return matrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SLOTS
//
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::on_externalEditorSelect_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Select external graphics editor"));
    if (file_name.isEmpty())
    {
        return;
    }

    externalEditorName->setText(file_name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Formats
////////////////////////////////////////////////////////////////////////////////////////////////////

void SettingsDialog::on_formatsList_currentRowChanged(int current_row)
{
    if (current_row == -1)
    {
        formatSave->setEnabled(false);
        formatDel->setEnabled(false);
        return;
    }

    formatSave->setEnabled(true);
    formatDel->setEnabled(true);
    updateFormatEditor(current_row);
}

void SettingsDialog::on_formatName_textChanged(QString text)
{
    if (text.isEmpty())
    {
        formatAdd->setEnabled(false);
        formatSave->setEnabled(false);
    }
    else
    {
        formatAdd->setEnabled(true);
        if (formatsList->currentRow() != -1)
        {
            formatSave->setEnabled(true);
        }
    }
}

void SettingsDialog::on_formatDel_clicked()
{
    int index = formatsList->currentRow();
    if (index < 0 || index >= _formats.count())
    {
        return;
    }

    _formats.removeAt(index);
    QListWidgetItem *item = formatsList->takeItem(index);
    delete item;
}

void SettingsDialog::on_formatSave_clicked()
{
    int index = formatsList->currentRow();
    if (index < 0 || index >= _formats.count())
    {
        return;
    }

    PhotoFormat format = fillFormat();
    _formats.replace(index, format);
    formatsList->currentItem()->setText(format.getFormatName());
}

void SettingsDialog::on_formatAdd_clicked()
{
    PhotoFormat format = fillFormat();

    _formats.append(format);
    formatsList->addItem(format.getFormatName());
    formatsList->setCurrentRow(formatsList->count() - 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrices
////////////////////////////////////////////////////////////////////////////////////////////////////


void SettingsDialog::on_matrixList_currentRowChanged(int current_row)
{
    if (current_row == -1)
    {
        matrixSave->setEnabled(false);
        matrixDel->setEnabled(false);
        return;
    }

    matrixSave->setEnabled(true);
    matrixDel->setEnabled(true);
    updateMatrixEditor(current_row);
}

void SettingsDialog::on_matrixName_textChanged(QString text)
{
    if (text.isEmpty())
    {
        matrixAdd->setEnabled(false);
        matrixSave->setEnabled(false);
    }
    else
    {
        matrixAdd->setEnabled(true);
        if (matrixList->currentRow() != -1)
        {
            matrixSave->setEnabled(true);
        }
    }
}

void SettingsDialog::on_matrixDel_clicked()
{
    int index = matrixList->currentRow();
    if (index < 0 || index >= _matrices.count())
    {
        return;
    }

    _matrices.removeAt(index);
    QListWidgetItem *item = matrixList->takeItem(index);
    delete item;
}

void SettingsDialog::on_matrixSave_clicked()
{
    int index = matrixList->currentRow();
    if (index < 0 || index >= _matrices.count())
    {
        return;
    }

    MatrixView matrix = fillMatrix();
    _matrices.replace(index, matrix);
    matrixList->currentItem()->setText(matrix.getName());
}

void SettingsDialog::on_matrixAdd_clicked()
{
    MatrixView matrix = fillMatrix();

    _matrices.append(matrix);
    matrixList->addItem(matrix.getName());
    matrixList->setCurrentRow(matrixList->count() - 1);
}

void SettingsDialog::on_matrixPosDel_clicked()
{
    int index = matrixPosList->currentRow();
    if (index < 0 || index >= _matrix_pos.count())
    {
        return;
    }

    _matrix_pos.removeAt(index);
    QListWidgetItem *item = matrixPosList->takeItem(index);
    delete item;
}

void SettingsDialog::on_matrixPosAdd_clicked()
{
    _matrix_pos.append(QPointF(matrixPosX->value(), matrixPosY->value()));
    QString text = QString("X%1 %2, Y%1 %3")
                        .arg(QString::fromUtf8("→"))
                        .arg(_matrix_pos.last().x())
                        .arg(_matrix_pos.last().y());
    matrixPosList->addItem(text);
    matrixPosList->setCurrentRow(matrixPosList->count() - 1);
}

