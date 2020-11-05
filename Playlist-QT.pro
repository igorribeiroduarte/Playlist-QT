QT       += core gui network networkauth multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createplaylist.cpp \
    databaseutil.cpp \
    homepage.cpp \
    main.cpp \
    player.cpp \
    playlistmodel.cpp \
    search.cpp \
    spotify.cpp \
    trackmodel.cpp

HEADERS += \
    createplaylist.h \
    databaseutil.h \
    homepage.h \
    player.h \
    playlistmodel.h \
    search.h \
    spotify.h \
    trackmodel.h

FORMS += \
    createplaylist.ui \
    homepage.ui \
    search.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
