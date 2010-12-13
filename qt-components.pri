INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/src/lib
TEMPLATE = lib ## hack to make qtLibraryTarget work
!build_lib {
    macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
        LIBS += -F$$Q_COMPONENTS_BUILD_TREE/lib -framework QtComponents
    } else {
        LIBS += -L$$Q_COMPONENTS_BUILD_TREE/lib -l$$qtLibraryTarget(QtComponents)
    }
}

unix {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
}

LIBRARYPATH += $$Q_COMPONENTS_BUILD_TREE/lib

QMAKE_INCDIR += $$INCLUDEPATH
QMAKE_LIBDIR += $$LIBRARYPATH

meego {
    DEFINES += Q_COMPONENTS_MEEGO
    CONFIG += meegotouch
}
