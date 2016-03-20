QT += core
QT -= gui

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_video \
-lopencv_videoio \
-lopencv_imgcodecs

TARGET = hough
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp

