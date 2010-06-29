INCLUDEPATH += $$PWD/src $$PWD/src/models $$PWD/src/styles $$PWD/src/chrome
!build_lib:LIBS += -L$$PWD/lib -lQtComponents
QMAKE_RPATHDIR = $$OUT_PWD/lib

# hack for now so Window keeps working`
CONFIG += mx

LIBRARYPATH += $$PWD/lib

QMAKE_INCDIR += $$INCLUDEPATH
QMAKE_LIBDIR += $$LIBRARYPATH

mx {
    DEFINES += Q_COMPONENTS_MX
    INCLUDEPATH += $$PWD/src/styles/Mx
}

meego {
    DEFINES += Q_COMPONENTS_MEEGO
    CONFIG += meegotouchcore
}
