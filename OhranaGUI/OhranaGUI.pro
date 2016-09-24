#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T08:38:37
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OhranaGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qsqlconnectiondialog.cpp \
    logic/als.cpp \
    logic/controller.cpp \
    controllermodel.cpp \
    logic/db.cpp \
    settingsdialog.cpp \
    logic/controllerevent.cpp

HEADERS  += mainwindow.h \
    qsqlconnectiondialog.h \
    bd.h \
    logic/als.h \
    logic/controller.h \
    controllermodel.h \
    main.h \
    logic/db.h \
    settingsdialog.h \
    logic/controllerevent.h

FORMS    += mainwindow.ui \
    qsqlconnectiondialog.ui \
    settingsdialog.ui

CONFIG -= build_all
CONFIG += release

DISTFILES += \
    model.qmodel
