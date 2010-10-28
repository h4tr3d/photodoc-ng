#-------------------------------------------------
#
# Project created by QtCreator 2010-10-22T02:10:14
#
#-------------------------------------------------

QT       += core gui

TARGET = photodoc
TEMPLATE = app
CONFIG += warn_on link_pkgconfig

PKGCONFIG += exiv2 libgphoto2
DEFINES += HAVE_GPHOTO2

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
    src/mfiledialog.cpp

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
    src/mfiledialog.h

FORMS    += ui/mainwindow.ui \
    src/settingsdialog.ui \
    src/formimagematrix.ui

RESOURCES += \
    rc/main.qrc

TRANSLATIONS += ts/photodoc-ng_ru_RU.ts
