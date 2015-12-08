#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T22:34:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 37-tbb-simd
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++11
LIBS += -ltbb

SOURCES += main.cpp
