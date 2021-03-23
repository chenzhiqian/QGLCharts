QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    source/qglcombinechart.cpp \
    source/qglhistogramchart.cpp \
    source/qglhistogramset.cpp \
    source/qgllinechart.cpp \
    source/qglbaseaxis.cpp \
    source/qglaxis.cpp \
    source/qgldataset.cpp \
    source/qgllineset.cpp \
    source/qglbasechart.cpp \
    source/qglscatterchart.cpp \
    source/qglscatterset.cpp

HEADERS += \
    source/qglcombinechart.h \
    source/qglhistogramchart.h \
    source/qglhistogramset.h \
    source/qgllinechart.h \
    source/qglbaseaxis.h \
    source/qglaxis.h \
    source/qgldataset.h \
    source/qgllineset.h \
    source/qglbasechart.h \
    source/qglscatterchart.h \
    source/qglscatterset.h

QT += opengl

win32:LIBS += -lOpengl32\
-lglu32
unix:LIBS += -lglut -lGLU

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
