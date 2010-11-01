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
**   @file   mfiledialog.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Implementation of file dialog with image preview
**
**************************************************************************/

#include <QSplitter>
#include <QPixmap>

#include "mfiledialog.h"

MFileDialog::MFileDialog(QWidget *parent, Qt::WindowFlags flags) : QFileDialog(parent, flags)
{
    init();
}

MFileDialog::MFileDialog(QWidget *parent,
                         const QString &caption,
                         const QString &directory,
                         const QString &filter)
    : QFileDialog(parent, caption, directory, filter)
{
    init();
}

MFileDialog::~MFileDialog()
{
    if (_preview != 0)
    {
        delete _preview;
    }
}

void MFileDialog::init()
{
    _preview = 0;

    // HACK: original file dialog in Qt4 does not have preview functinality
    // Work well on 4.7.1
    QSplitter *splitter = findChild<QSplitter*>("splitter");
    if (splitter == 0)
    {
        return;
    }

    _preview = new QLabel();
    QRect geomerty = _preview->geometry();
    geomerty.setWidth(160);
    _preview->setGeometry(geomerty);

    splitter->addWidget(_preview);

    connect(this, SIGNAL(currentChanged(QString)),
            this, SLOT(fileSelected(QString)));
}

void MFileDialog::fileSelected(const QString &file)
{
    if (_preview == 0)
    {
        return;
    }

    QPixmap pix(file);
    if (pix.isNull())
    {
        _preview->setPixmap(QPixmap());
    }
    else
    {
        QSize size = _preview->size();
        pix = pix.scaled(size, Qt::KeepAspectRatio);
        _preview->setPixmap(pix);
    }
}
