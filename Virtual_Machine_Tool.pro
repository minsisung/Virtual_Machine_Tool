#-------------------------------------------------
#
# Project created by QtCreator 2019-09-16T00:02:56
#
#-------------------------------------------------

QT       += core gui opengl
LIBS += -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Virtual_Machine_Tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    control_panel.cpp \
        creategemoetry.cpp \
    joint.cpp \
    link.cpp \
    machineTool.cpp \
        main.cpp \
        mainwindow.cpp \
        myopenglwidget.cpp \
        readgcode.cpp \
    tinyxml2.cpp

HEADERS += \
        control_panel.h \
        creategemoetry.h \
        joint.h \
        link.h \
        machineTool.h \
        mainwindow.h \
        myopenglwidget.h \
        readgcode.h \
        shaders.h \
        stl_reader.h \
        tinyxml2.h \
        vector3.h \
        vectorRGBA.h

FORMS += \
        control_panel.ui \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

