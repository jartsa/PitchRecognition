#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T11:05:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = windowChangeTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog0.cpp \
    dialog1.cpp \
    Gist.cpp \
    kiss_fft.c \
    core/CoreFrequencyDomainFeatures.cpp \
    core/CoreTimeDomainFeatures.cpp \
    fft/WindowFunctions.cpp \
    onset-detection-functions/OnsetDetectionFunction.cpp \
    pitch/Yin.cpp \
    qcustomplot.cpp \
    songthread.cpp \
    timethread.cpp \
    freq2pitch.cpp \
    run.cpp

HEADERS  += mainwindow.h \
    dialog0.h \
    dialog1.h \
    _kiss_fft_guts.h \
    Gist.h \
    kiss_fft.h \
    core/CoreFrequencyDomainFeatures.h \
    core/CoreTimeDomainFeatures.h \
    fft/WindowFunctions.h \
    onset-detection-functions/OnsetDetectionFunction.h \
    pitch/Yin.h \
    qcustomplot.h \
    songthread.h \
    stdafx.h \
    timerthread.h \
    freq2pitch.h \
    wav2pitch.h

FORMS    += mainwindow.ui \
    dialog0.ui \
    dialog1.ui

RESOURCES += \
    images.qrc
