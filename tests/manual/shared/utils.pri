INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/utils.h
SOURCES += $$PWD/utils.cpp

symbian:symbian_internal {
    LIBS += -laknnotify // For CAknSmallIndicator
}
