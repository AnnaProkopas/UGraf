#-------------------------------------------------
#
# Project created by QtCreator 2017-12-12T00:15:00
#
#-------------------------------------------------

CONFIG   += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = U
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    data.cpp \
    renderarea.cpp \
    main.cpp

HEADERS  += mainwindow.h \
    data.h \
    renderarea.h \
    data.hpp

FORMS    += mainwindow.ui

RESOURCES += \
    basicdrawing.qrc

DISTFILES += \
    cont+01.dat \
    cont+02.dat \
    cont+03.dat

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
