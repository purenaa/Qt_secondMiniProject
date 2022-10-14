QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    main.cpp \
    clientmanagerform.cpp \
    mainwindow.cpp \
    order.cpp \
    ordermanagerform.cpp \
    product.cpp \
    productmanagerform.cpp

HEADERS += \
    client.h \
    clientmanagerform.h \
    mainwindow.h \
    order.h \
    ordermanagerform.h \
    product.h \
    productmanagerform.h

FORMS += \
    clientmanagerform.ui \
    mainwindow.ui \
    ordermanagerform.ui \
    productmanagerform.ui

TRANSLATIONS += \
    ShopApp_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
