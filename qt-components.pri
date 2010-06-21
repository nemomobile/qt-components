INCLUDEPATH += $$PWD/src $$PWD/src/models $$PWD/src/styles
LIBRARYPATH += $$PWD/lib
QMAKE_RPATHDIR = $$OUT_PWD/lib

QMAKE_INCDIR += $$INCLUDEPATH
QMAKE_LIBDIR += $$LIBRARYPATH
LIBS += -L$$PWD/lib -lqtcomponents

