INCLUDEPATH += $$PWD/src/lib
TEMPLATE = lib ## hack to make qtLibraryTarget work
!build_lib {
    macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {
        LIBS += -F$$PWD/lib -framework QtComponents
    } else {
        LIBS += -L$$PWD/lib -l$$qtLibraryTarget(QtComponents)
    }
}

meego {
    DEFINES += Q_COMPONENTS_MEEGO
    CONFIG += meegotouch
}
