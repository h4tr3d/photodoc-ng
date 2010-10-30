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

#include <QtGui/QApplication>

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

    // Portable solution
    QTranslator app_translator;
    app_translator.load("photodoc-ng_" + QLocale::system().name());
    a.installTranslator(&app_translator);

    // Register custom types
    qRegisterMetaType<PhotoFormat>("PhotoFormat");
    qRegisterMetaTypeStreamOperators<PhotoFormat>("PhotoFormat");
    qRegisterMetaType<MatrixView>("MatrixView");
    qRegisterMetaTypeStreamOperators<MatrixView>("MatrixView");

    // Init settings istance, load settings
    Settings *sets = Settings::instance();
    sets->load();

    // begin STUB
    // Форматы
    QList<QVariant> formats = sets->getParam("core.formats").toList();
    if (formats.count() <= 0)
    {
        formats << qVariantFromValue(PhotoFormat(QString::fromUtf8("Документы (3x4)"), QSizeF(30, 40), 6, 9))
                << qVariantFromValue(PhotoFormat(QString::fromUtf8("Паспорт (3.5x4.5)"), QSizeF(37, 47), 5, 12))
                << qVariantFromValue(PhotoFormat(QString::fromUtf8("Личное дело (9x12)"), QSizeF(90, 120), 10, 22));
        sets->setParam("core.formats", formats);
    }


    // Матрицы
    QList<QVariant> matrix = sets->getParam("core.matrix").toList();
    if (matrix.count() <= 0)
    {
        matrix << qVariantFromValue(MatrixView(QString::fromUtf8("6шт. Документы 3x4 (бумага 10х15)"),
                                               QSizeF(100, 150),
                                               QList<QPointF>() << QPointF(15, 10)
                                                                << QPointF(55, 10)
                                                                << QPointF(15, 50)
                                                                << QPointF(55, 50)
                                                                << QPointF(15, 90)
                                                                << QPointF(55, 90),
                                               QString::fromUtf8("Сделано {DATE}, {TIME}"),
                                               QPointF(5, 140),
                                               3));
        matrix << qVariantFromValue(MatrixView(QString::fromUtf8("4шт. Паспорт (бумага 10х15)"),
                                               QSizeF(100, 150),
                                               QList<QPointF>() << QPointF(55, 15)
                                                                << QPointF(10, 15)
                                                                << QPointF(10, 70)
                                                                << QPointF(55, 70),
                                               QString::fromUtf8("Сделано {DATE}, {TIME}"),
                                               QPointF(5, 14),
                                               3));
        sets->setParam("core.matrix", matrix);
    }
    // end STUB

    MainWindow w;
    w.show();
    int result = a.exec();

    sets->save();

    return result;
}
