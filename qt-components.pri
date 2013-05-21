!mobility:!no_mobility {
    # bool load(feature, ignore_error)
    load(mobilityconfig, true):CONFIG += mobility
}

INCLUDEPATH += $$PWD/src/components

CONFIG += qt-components depend_includepath qt-components_build

unix {
    CONFIG(release, debug|release) {
        OBJECTS_DIR = .release
        MOC_DIR = .release
        RCC_DIR = .release
    } else {
        OBJECTS_DIR = .debug
        MOC_DIR = .debug
        RCC_DIR = .debug
    }
}

meego {
    exists( $$[QMAKE_MKSPECS]/features/meegotouch_defines.prf ) {
        load(meegotouch_defines)
        THEME_DIR = $$M_THEME_DIR
    } else {
        THEME_DIR = /usr/share/themes
    }

    # Debian and derivatives pass --no-undefined to the linker, which
    # means that each library must explicitly link to all dependencies
    # than assuming that the application or another library will bring
    # in the necessary symbols at run time.
    contains(QMAKE_LFLAGS, "-Wl,--no-undefined"):LIBS += $${QMAKE_LIBS_X11}

    equals(QT_MAJOR_VERSION, 5): include(src/compat/compat.pri)
}

# Transition flag for aligning screen api's width/height/displayWidth/displayHeight
# Once application have been updated, remove this define and flagings from screen api implementation and tests
DEFINES += USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
