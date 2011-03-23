!mobility:!no_mobility {
    # bool load(feature, ignore_error)
    load(mobilityconfig, true):CONFIG += mobility
}

CONFIG += qt-components

unix:!symbian {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
}
