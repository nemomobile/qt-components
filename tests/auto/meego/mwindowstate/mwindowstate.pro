include (../../../tests.pri)

TEMPLATE = app
SOURCES += \
    tst_mwindowstate.cpp \
    ../../../../src/meego/mwindowstate.cpp

HEADERS += \
    ../../../../src/meego/mwindowstate.h \
    ../../../../src/meego/mwindowstate_p.h

INCLUDEPATH += ../../../../src/meego/

include (../../auto.pri)
