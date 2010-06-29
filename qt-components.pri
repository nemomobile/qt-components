INCLUDEPATH += $$PWD/src $$PWD/src/models $$PWD/src/styles $$PWD/src/chrome
LIBS += -L$$PWD/lib -lqtcomponents
QMAKE_RPATHDIR = $$OUT_PWD/lib

LIBRARYPATH += $$PWD/lib

QMAKE_INCDIR += $$INCLUDEPATH
QMAKE_LIBDIR += $$LIBRARYPATH

