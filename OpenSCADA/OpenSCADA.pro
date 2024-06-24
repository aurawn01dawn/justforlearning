QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    absscenewidgetitem.cpp \
    customdataitem.cpp \
    customfunction.cpp \
    customscene.cpp \
    datamanager.cpp \
    datawidget.cpp \
    designertoolbar.cpp \
    designerview.cpp \
    designerwidget.cpp \
    funcconfigwidget.cpp \
    functionselection.cpp \
    imageselectwidget.cpp \
    leftdownwidget.cpp \
    leftupwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    midwidget.cpp \
    rightwidget.cpp \
    saveloadmanager.cpp \
    sceneselecttoolbar.cpp \
    scenesmanager.cpp \
    showwidget.cpp \
    showwidgetview.cpp

HEADERS += \
    config.h \
    absscenewidgetitem.h \
    customdataitem.h \
    customfunction.h \
    customscene.h \
    datamanager.h \
    datawidget.h \
    designertoolbar.h \
    designerview.h \
    designerwidget.h \
    funcconfigwidget.h \
    functionselection.h \
    imageselectwidget.h \
    leftdownwidget.h \
    leftupwidget.h \
    mainwindow.h \
    midwidget.h \
    rightwidget.h \
    saveloadmanager.h \
    sceneselecttoolbar.h \
    scenesmanager.h \
    showwidget.h \
    showwidgetview.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
