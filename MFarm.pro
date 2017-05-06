#-------------------------------------------------
#
# Project created by QtCreator 2012-09-04T13:11:29
#
#-------------------------------------------------

DEFINES += NCREPORT_IMPORT
LIBS += "D:/QtProjects/NCReport/lib/libncreportd2.a"
INCLUDEPATH += "D:/QtProjects/NCReport/include"

QT       += core gui
QT       += sql

TARGET = MFarm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    bulls.cpp \
    cows.cpp \
    bullcard.cpp \
    cowcard.cpp \
    newcow.cpp \
    newbull.cpp \
    dbconnect.cpp \
    photodialog.cpp \
    bullsfilter.cpp \
    cowsfilter.cpp \
    newdb.cpp \
    dblist.cpp \
    adddb.cpp \
    reportlist.cpp \
    aboutfarm.cpp \
    bullbonitation.cpp \
    cowbonitation.cpp \
    measurement.cpp \
    insemination.cpp \
    calving.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    bulls.h \
    cows.h \
    bullcard.h \
    cowcard.h \
    newcow.h \
    newbull.h \
    dbconnect.h \
    photodialog.h \
    bullsfilter.h \
    cowsfilter.h \
    newdb.h \
    dblist.h \
    adddb.h \
    reportlist.h \
    aboutfarm.h \
    bullbonitation.h \
    cowbonitation.h \
    measurement.h \
    insemination.h \
    calving.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    bulls.ui \
    cows.ui \
    bullcard.ui \
    cowcard.ui \
    newcow.ui \
    newbull.ui \
    photodialog.ui \
    bullsfilter.ui \
    cowsfilter.ui \
    newdb.ui \
    dblist.ui \
    adddb.ui \
    reportlist.ui \
    aboutfarm.ui \
    bullbonitation.ui \
    cowbonitation.ui \
    measurement.ui \
    insemination.ui \
    calving.ui

RESOURCES += \
    res/res.qrc

TRANSLATIONS +=\
    res/russian.ts



