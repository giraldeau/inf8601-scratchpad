#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T15:15:29
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 23-omp-atomic
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += main.cpp
