#-------------------------------------------------
#
# Project created by QtCreator 2017-03-24T00:07:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix:QT += x11extras

TARGET = VulkanInfoViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ToString.cpp

HEADERS  += mainwindow.h \
    ToString.h

FORMS    += mainwindow.ui

unix:INCLUDEPATH += "$$(VULKAN_SDK)/include"
unix:LIBS += -L"$$(VULKAN_SDK)/lib"
unix:LIBS += -lvulkan

win32:INCLUDEPATH += "$$(VULKAN_SDK)/Include"
win32:LIBS += -L"$$(VULKAN_SDK)/Lib"
win32:LIBS += -lvulkan-1

message("VULKAN_SDK:" $$(VULKAN_SDK))

DISTFILES += \
    icons/Vulkan-2016-Transparent-bg.png

RESOURCES += \
    VulkanInfoViewer.qrc
