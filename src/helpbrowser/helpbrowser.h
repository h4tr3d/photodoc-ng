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
**   @file   helpbrowser.h
**   @date   2010-10-31
**   @author hatred
**   @brief  Simple HTML help browser
**
**************************************************************************/

#ifndef HELPBROWSER_H
#define HELPBROWSER_H
//
#include <Qt>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QDialog>
#include <QApplication>
//
class HelpBrowser : public QDialog
{
Q_OBJECT
private slots:
    void updateCaption();

public:
    HelpBrowser(const QString& path,
                const QString& page,
                QWidget *parent = 0);
    ~HelpBrowser();

    static void showPage(const QString &page);

private:
    QVBoxLayout  *vblMain;
    QHBoxLayout  *hblButtons;
    QPushButton  *pbBack;
    QPushButton  *pbHome;
    QPushButton  *pbForward;
    QPushButton  *pbClose;
    QTextBrowser *txtBrowser;

};
#endif
