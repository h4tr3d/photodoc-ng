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
**   @file   mfiledialog.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Implementation of file dialog with image preview
**
**************************************************************************/

#ifndef MFILEDIALOG_H
#define MFILEDIALOG_H

#include <QLabel>
#include <QFileDialog>

class MFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit MFileDialog(QWidget *parent, Qt::WindowFlags flags);
    explicit MFileDialog(QWidget *parent = 0,
                         const QString &caption = QString(),
                         const QString &directory = QString(),
                         const QString & filter = QString());

    ~MFileDialog();

signals:

public slots:
    void fileSelected(const QString &file);

private:
    void init();

private:
    QLabel *_preview;
};

#endif // MFILEDIALOG_H
