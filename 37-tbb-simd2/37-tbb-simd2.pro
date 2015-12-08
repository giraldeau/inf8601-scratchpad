#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T22:46:04
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_tbbsimdavg
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
LIBS += -ltbb

SOURCES += tst_tbbsimdavg.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
