#-------------------------------------------------
#
# Project created by QtCreator 2012-11-29T14:13:57
#
#-------------------------------------------------

QT       += core gui
QT       += network

TARGET = FinalProjectGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamewindow.cpp \
    mclient.cpp \
    mserver.cpp \
    mthread.cpp \
    mqtcpserver.cpp \
    CardLib/CardStack.cpp \
    player.cpp \
    qlabelclick.cpp \
    resultstruct.cpp

HEADERS  += mainwindow.h \
    gamewindow.h \
    mvector.h \
    mclient.h \
    mserver.h \
    namedata.h \
    tnode.h \
    btree.h \
    mthread.h \
    mqtcpserver.h \
    CardLib/CardStack.h \
    CardLib/Card.h \
    player.h \
    Stack.h \
    qlabelclick.h \
    resultstruct.h \
    Heap.h \
    Node.h

FORMS    += mainwindow.ui \
    gamewindow.ui \
    mclient.ui \
    mserver.ui

OTHER_FILES +=

RESOURCES += \
    cImage.qrc
