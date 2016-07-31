#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T12:46:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

macx {
    # mac only
}
unix:!macx{
    # linux only

}
win32 {
    # windows only
    QMAKE_CXXFLAGS += -march=i686
}
top_srcdir = $$PWD
top_builddir = $$shadowed($$PWD)

TARGET = dbc_browser
TEMPLATE = app

INCLUDEPATH += libdbc/dbc/inc
INCLUDEPATH += libdbc/3rdParty/include

LIBS += $$top_srcdir\libdbc\libdbc.a
LIBS += $$top_srcdir\libdbc\3rdParty\lib\libboost_regex-mt.a

SOURCES += main.cpp\
    mainwindow.cpp \
    dbwindow.cpp \
    dbcport.cpp \
    gbldata.cpp \
    dialog_network.cpp

HEADERS  += mainwindow.h \
    dbwindow.h \
    dbcport.h \
    gbldata.h \
    dialog_network.h

FORMS    += mainwindow.ui \
    dbwindow.ui \
    dialog_network.ui

RESOURCES += \
    dbcitem.qrc
