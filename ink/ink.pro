#-------------------------------------------------
#
# Project created by QtCreator 2019-01-04T23:29:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = theInk
TEMPLATE = app

macx {
    QT += macextras
    ICON = icon.icns
    LIBS += -framework CoreFoundation -framework AppKit
    QMAKE_INFO_PLIST = Info.plist
}

unix:!macx {
    QT += thelib
    TARGET = theink
}

win32 {
    QT += thelib
    INCLUDEPATH += "C:/Program Files/thelibs/include"
    LIBS += -L"C:/Program Files/thelibs/lib" -lthe-libs
    RC_FILE = icons\\icon.rc
}

macx {
    INCLUDEPATH += "/usr/local/include/the-libs"
    LIBS += -L/usr/local/lib -lthe-libs
}

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
        main.cpp \
        mainwindow.cpp \
    documentview.cpp \
    document.cpp \
    documentpage.cpp \
    tabbutton.cpp \
    documentformat.cpp

HEADERS += \
        mainwindow.h \
    documentview.h \
    document.h \
    documentpage.h \
    tabbutton.h \
    documentformat.h

FORMS += \
        mainwindow.ui


# Turn off stripping as this causes the install to fail :(
QMAKE_STRIP = echo

unix:!macx {
    target.path = /usr/bin

    #translations.path = /usr/share/theink/translations
    #translations.files = translations/*

    desktop.path = /usr/share/applications
    desktop.files = theink.desktop

    icon.path = /usr/share/icons/hicolor/scalable/apps/
    icon.files = icons/theink.svg

    INSTALLS += target desktop icon #translations
}

DISTFILES += \
    theink.desktop

RESOURCES += \
    resources.qrc
