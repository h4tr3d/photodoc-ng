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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

class Settings
{
public:
    Settings();

    static Settings *instance();

    void load(QString file_name = QString());
    void save(QString file_name = QString());

    QString getDefaultConfig();
    void    setAppName(QString name);
    void    setCompanyName(QString name);

    QVariant getParam(QString key);
    void     setParam(QString key, QVariant value);

private:
    QMap<QString, QVariant> _settings;
    QString                 _application_name;
    QString                 _company_name;
};

#endif // SETTINGS_H
