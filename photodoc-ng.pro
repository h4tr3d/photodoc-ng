#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T02:10:14
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets printsupport
} else {
  QT += core gui
}

TARGET = photodoc-ng
TEMPLATE = app
CONFIG += warn_on link_pkgconfig

PKGCONFIG += exiv2
!win32 {
  DEFINES   += HAVE_GPHOTO2
  PKGCONFIG += libgphoto2
}

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/mgraphicsscene.cpp \
    src/utils.cpp \
    src/settings.cpp \
    src/photoformat.cpp \
    src/matrixview.cpp \
    src/settingsdialog.cpp \
    src/formimagematrix.cpp \
    src/imageprocess.cpp \
    src/gphoto2/gpiteminfo.cpp \
    src/gphoto2/gpcamera.cpp \
    src/gphoto2/dkcamera.cpp \
    src/gphoto2/photoinfocontainer.cpp \
    src/md5/md5.c \
    src/mfiledialog.cpp \
    src/gphotodialog.cpp \
    src/helpbrowser/helpbrowser.cpp

HEADERS  += src/mainwindow.h \
    src/mgraphicsscene.h \
    src/utils.h \
    src/settings.h \
    src/photoformat.h \
    src/matrixview.h \
    src/settingsdialog.h \
    src/formimagematrix.h \
    src/imageprocess.h \
    src/gphoto2/gpiteminfo.h \
    src/gphoto2/gpcamera.h \
    src/gphoto2/dkcamera.h \
    src/gphoto2/photoinfocontainer.h \
    src/md5/md5.h \
    src/mfiledialog.h \
    src/gphotodialog.h \
    src/helpbrowser/helpbrowser.h

FORMS    += src/mainwindow.ui \
    src/settingsdialog.ui \
    src/formimagematrix.ui \
    src/gphotodialog.ui

RESOURCES += \
    rc/main.qrc

TRANSLATIONS += ts/photodoc-ng.ts \
    ts/photodoc-ng_ru_RU.ts

## Additional checks
TRANSLATIONS_QM = $$replace(TRANSLATIONS, .ts, .qm)
!build_pass:for(qm, TRANSLATIONS_QM) {
    !exists($$qm) {
        error("Can't found one or more translations files. Run 'lrelease' on project file before 'qmake'")
    }
}

## Installs
unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    DEFINES += PREFIX=\\\"$$PREFIX\\\"

    # dirs
    BINDIR = $$PREFIX/bin
    DATADIR = $$PREFIX/share/$$TARGET
    LOCALEDIR = $$PREFIX/share/$$TARGET/locale

    # files
    target.path   = $${BINDIR}

    locale.files  = $$TRANSLATIONS_QM
    locale.path   = $${LOCALEDIR}

    data.files    = PhotoDocNG.ini \
                    INSTALL \
                    AUTHORS \
                    NEWS \
                    LICENSE \
                    LICENSE.ru \
                    TODO \
                    TODO.ru \
                    THANKS \
                    help.html \
                    help_ru_RU.html
    data.path     = $${DATADIR}

    # install
    INSTALLS += target locale data

    # brifing
    !build_pass:message("Install prefix:" $$PREFIX)
    !build_pass:message("    Binary dir:" $$BINDIR)
    !build_pass:message("      Data dir:" $$DATADIR)
    !build_pass:message("    Locale dir:" $$LOCALEDIR)
}

win32 {
    DEFINES += PREFIX=\\\"\\\"
}
