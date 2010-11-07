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
**   @file   helpbrowser.cpp
**   @date   2010-10-31
**   @author hatred
**   @brief  Simple HTML help browser
**
**************************************************************************/

#include <QFileInfo>

#include "helpbrowser.h"
//
HelpBrowser::HelpBrowser(const QString& path,
                         const QString& page,
                         QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setAttribute(Qt::WA_DeleteOnClose);
    pbBack     = new QPushButton(tr("<<"), this);
    pbHome     = new QPushButton(tr("Home"), this);
    pbForward  = new QPushButton(tr(">>"), this);
    pbClose    = new QPushButton(tr("Close"), this);
    txtBrowser = new QTextBrowser(this);
    
    //pbClose->setAccel(tr("Esc"));
    /*QAction *pbCloseAction = new QAction(pbClose);
    pbCloseAction->setShortcut(QKeySequence("Esc"));
    pbClose->setAction(pbCloseAction);*/ 

    vblMain = new QVBoxLayout(this);
    hblButtons = new QHBoxLayout();
    hblButtons->addWidget(pbBack);
    hblButtons->addWidget(pbHome);
    hblButtons->addWidget(pbForward);
    hblButtons->addStretch(1);
    hblButtons->addWidget(pbClose);
    vblMain->addLayout(hblButtons);
    vblMain->addWidget(txtBrowser);  
        
    connect(pbBack, SIGNAL(clicked()),
            txtBrowser, SLOT(backward()));
    connect(pbHome, SIGNAL(clicked()),
            txtBrowser, SLOT(home()));
    connect(pbForward, SIGNAL(clicked()),
            txtBrowser, SLOT(forward()));
    connect(pbClose, SIGNAL(clicked()),
            this, SLOT(close()));
            
    connect(txtBrowser, SIGNAL(backwardAvailable(bool)),
            pbBack, SLOT(setEnabled(bool)));
    connect(txtBrowser, SIGNAL(forwardAvailable(bool)),
            pbForward, SLOT(setEnabled(bool)));
    connect(txtBrowser, SIGNAL(sourceChanged(const QUrl &)),
            this, SLOT(updateCaption()));

    txtBrowser->setOpenExternalLinks(true);
    txtBrowser->setOpenLinks(true);
    txtBrowser->setSearchPaths(QStringList(path));
    txtBrowser->setSource(QUrl(path + "/" + page));
}
//

HelpBrowser::~HelpBrowser()
{
}

void HelpBrowser::updateCaption()
{
    setWindowTitle(tr("Help: %1").arg(txtBrowser->documentTitle())); 
}

void HelpBrowser::showPage(const QString &page)
{
    QFileInfo fi(page);

    HelpBrowser *browser = new HelpBrowser(fi.absolutePath(),
                                           fi.fileName());
    browser->resize(800, 400);
    browser->show();
}

