QT += widgets network core

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    windowforcommunication.cpp

HEADERS += \
    mainwindow.h \
    myLib.h \
    myLib_global.h \
    windowforcommunication.h

FORMS += \
    mainwindow.ui \
    windowforcommunication.ui

# 打印实际路径，确认是否正确
message("LIBS path: $$PWD/build/Desktop_Qt_6_10_3_MinGW_64_bit-Debug")

INCLUDEPATH += $$PWD/../publicheader
INCLUDEPATH += $$PWD/../Logger
LIBS += -L$$PWD/ -l5KNX01
LIBS += -L$$PWD/ -lLogger


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RC_ICONS = icon.ico
