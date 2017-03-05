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
**   @file   main.cpp
**   @date   2010-10-30
**   @author hatred
**   @brief  Entry point file
**
**************************************************************************/

#include <QApplication>

#include <iostream>

#include "mainwindow.h"
#include "settings.h"
#include "matrixview.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application name ans version
    qApp->setApplicationName("PhotoDocNG");
    qApp->setApplicationVersion("1.0.5+");
    qApp->setOrganizationName("photodoc-ng");

    // Translation
    QTranslator qt_translator;
    qt_translator.load("qt_" + QLocale::system().name(),
             QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qt_translator);

    QString app_translator_file = "photodoc-ng_" + QLocale::system().name() + ".qm";
    QString app_translator_dir  = PREFIX"/share/photodoc/locale";
    QString app_translator_path = app_translator_dir + "/" + app_translator_file;

    if (!QFile::exists(app_translator_path))
    {
        app_translator_path = app_translator_file;
    }

    // Portable solution
    QTranslator app_translator;
    app_translator.load(app_translator_file);
    a.installTranslator(&app_translator);

    // Register custom types, must be called before settings init
    qRegisterMetaType<PhotoFormat>("PhotoFormat");
    qRegisterMetaTypeStreamOperators<PhotoFormat>("PhotoFormat");
    qRegisterMetaType<MatrixView>("MatrixView");
    qRegisterMetaTypeStreamOperators<MatrixView>("MatrixView");

    // Init users settings istance, load settings
    Settings *sets = Settings::instance();
    sets->load();


    // System wide settings
    QString config_file = qApp->applicationName() + ".ini";
    QString config_dir  = PREFIX"/share/photodoc";
    QString config_path = config_dir + "/" + config_file;
    Settings system_settings;

    // Portable solution
    if (!QFile::exists(config_path))
    {
        config_path = config_file;
    }
    system_settings.load(config_path);

    //
    // Load some default settings
    //
    // Formats
    QList<QVariant> formats = sets->getParam("core.formats").toList();
    if (formats.count() <= 0)
    {
        formats = system_settings.getParam("core.formats").toList();
        sets->setParam("core.formats", formats);
    }

    // Matrix
    QList<QVariant> matrix = sets->getParam("core.matrix").toList();
    if (matrix.count() <= 0)
    {
        matrix = system_settings.getParam("core.matrix").toList();
        sets->setParam("core.matrix", matrix);
    }

    // Create main window and start main loop
    MainWindow w;
    w.show();
    int result = a.exec();

    sets->save();

    return result;
}
