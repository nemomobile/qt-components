
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
!meego:!symbian {
    HEADERS += $$INCLUDEPATH/settingswindow.h
    SOURCES += $$INCLUDEPATH/settingswindow.cpp
}
