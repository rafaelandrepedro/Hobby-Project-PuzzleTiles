QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    building.cpp \
    canvas.cpp \
    entity.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    customlabel.cpp \
    map.cpp \
    score.cpp \
    shape.cpp \
    texture.cpp \
    tile.cpp \
    util.cpp \
    utils.cpp

HEADERS += \
    building.h \
    canvas.h \
    entity.h \
    game.h \
    mainwindow.h \
    customlabel.h \
    map.h \
    score.h \
    shape.h \
    texture.h \
    tile.h \
    util.h \
    utils.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
