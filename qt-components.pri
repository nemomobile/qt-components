!mobility:!no_mobility {
    # bool load(feature, ignore_error)
    load(mobilityconfig, true):CONFIG += mobility
}

CONFIG += qt-components depend_includepath

unix:!symbian {
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
