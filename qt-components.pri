INCLUDEPATH += $$PWD/src/lib $$PWD/src/module $$PWD/src/module/models
!build_lib:LIBS += -L$$PWD/lib -lQtComponents
QMAKE_RPATHDIR = $$OUT_PWD/lib

unix {
	OBJECTS_DIR = .obj
	MOC_DIR = .moc
}

LIBRARYPATH += $$PWD/lib

QMAKE_INCDIR += $$INCLUDEPATH
QMAKE_LIBDIR += $$LIBRARYPATH

meego {
    DEFINES += Q_COMPONENTS_MEEGO
    CONFIG += meegotouch
}
