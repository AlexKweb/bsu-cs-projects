QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_testingmyinteger.cpp
HEADERS += ../unit-test-class-integer/MyInteger.h
