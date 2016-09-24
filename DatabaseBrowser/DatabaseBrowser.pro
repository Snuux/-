#-------------------------------------------------
#
# Project created by QtCreator 2016-03-10T09:46:07
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatabaseBrowser
TEMPLATE = app


SOURCES += main.cpp \
    browser.cpp \
    qsqlconnectiondialog.cpp \
    bdqueries.cpp \
    oobject.cpp \
    oobjectmodel.cpp \
    oalarmmodel.cpp \
    oevent.cpp \
    oeventmodel.cpp \
    oohranamodel.cpp \
    oeventdelegate.cpp \
    osearchmodel.cpp \
    oohranadelegate.cpp \
    newalarmdialog.cpp \
    osearchdelegate.cpp


HEADERS  += \
    browser.h \
    qsqlconnectiondialog.h \
    bdqueries.h \
    oobject.h \
    oobjectmodel.h \
    oalarmmodel.h \
    oevent.h \
    oeventmodel.h \
    oohranamodel.h \
    oeventdelegate.h \
    osearchmodel.h \
    oohranadelegate.h \
    newalarmdialog.h \
    osearchdelegate.h


FORMS    += \
    qsqlconnectiondialog.ui \
    browserwidget.ui \
    newalarmdialog.ui

CONFIG -= build_all
CONFIG += release

DISTFILES += \
    Model.qmodel
