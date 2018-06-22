#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T22:09:47
#
#-------------------------------------------------

QT       -= core gui

TARGET = xcaspad
TEMPLATE = lib

DEFINES += XCASPAD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../jni/xcaspad/Equation.cc \
    ../../jni/xcaspad/main.cc \
    ../../jni/xcaspad/ftpngwriter.cpp

HEADERS += \
    ../../jni/xcaspad/Equation.h \
    ../../jni/xcaspad/ftpngwriter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../../libs/armeabi-v7a/ -lpngwriter

INCLUDEPATH += $$PWD/../../jni/pngwriter
DEPENDPATH += $$PWD/../../jni/pngwriter

unix:!macx: LIBS += -L$$PWD/../../libs/armeabi-v7a/ -lgiac

INCLUDEPATH += $$PWD/../../../src/giac
DEPENDPATH += $$PWD/../../../src/giac

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../android-ndk-r10e/platforms/android-21/arch-arm/usr/lib/release/ -lstdc++
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../android-ndk-r10e/platforms/android-21/arch-arm/usr/lib/debug/ -lstdc++
else:unix: LIBS += -L$$PWD/../../../../../../android-ndk-r10e/platforms/android-21/arch-arm/usr/lib/ -lstdc++

INCLUDEPATH += $$PWD/../../../../../../android-ndk-r10e/platforms/android-21/arch-arm/usr/include
DEPENDPATH += $$PWD/../../../../../../android-ndk-r10e/platforms/android-21/arch-arm/usr/include

unix:!macx: LIBS += -L$$PWD/../../../../../../android-ndk-r10e/sources/cxx-stl/gnu-libstdc++/4.9/libs/armeabi-v7a/ -lgnustl_shared

INCLUDEPATH += $$PWD/../../../../../../android-ndk-r10e/sources/cxx-stl/gnu-libstdc++/4.9/include
DEPENDPATH += $$PWD/../../../../../../android-ndk-r10e/sources/cxx-stl/gnu-libstdc++/4.9/libs
