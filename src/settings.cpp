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

#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>

#include <iostream>
#include "settings.h"

Settings::Settings()
{}

Settings *Settings::instance()
{
    static Settings *instance = new Settings();
    return instance;
}

void Settings::load(QString file_name)
{
    if (file_name.isEmpty())
    {
        file_name = getDefaultConfig();
    }
    QSettings cfg(file_name, QSettings::IniFormat);

    QStringList keys = cfg.allKeys();
    for (int i = 0; i < keys.count(); i++)
    {
        QString cfg_key = keys.at(i);
        QString key     = cfg_key;
        key.replace("/", ".");

        setParam(key, cfg.value(cfg_key));
    }

}

void Settings::save(QString file_name)
{
    if (file_name.isEmpty())
    {
        file_name = getDefaultConfig();
    }
    QSettings cfg(file_name, QSettings::IniFormat);

    QStringList keys = _settings.keys();
    for (int i = 0; i < keys.count(); i++)
    {
        QString key     = keys.at(i);
        QString cfg_key = key;
        cfg_key.replace(".", "/");

        cfg.setValue(cfg_key, getParam(key));
    }
}

QString Settings::getDefaultConfig()
{
    QString app_name;
    QString company_name;

    // Подберем имя компании, можем его и не использовать
    if (!_company_name.isEmpty())
    {
        company_name = _company_name;
    }
    else
    {
        company_name = qApp->organizationName();
    }

    // А вот имя приложения мы должны по любому сформировать
    if (!_application_name.isEmpty())
    {
        app_name = _application_name;
    }
    else if (!qApp->applicationName().isEmpty())
    {
        app_name = qApp->applicationName();
    }
    else
    {
        QString file = qApp->arguments().at(0);
        QFileInfo fi(file);
        app_name = fi.baseName();
    }

    QSettings cfg(QSettings::IniFormat, QSettings::UserScope, company_name, app_name);
    return cfg.fileName();
}

void Settings::setAppName(QString name)
{
    _application_name = name;
}

void Settings::setCompanyName(QString name)
{
    _company_name = name;
}

QVariant Settings::getParam(QString key)
{
    //std::cout << "Ask key: " << qPrintable(key) << "\n";
    return _settings.value(key);
}

void Settings::setParam(QString key, QVariant value)
{
    _settings.insert(key, value);
}

