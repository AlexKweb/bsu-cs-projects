QT += core gui widgets

CONFIG += c++17
CONFIG += warn_on

TARGET = qpainter_example
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui
