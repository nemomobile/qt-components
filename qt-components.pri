unix:!symbian {
    OBJECTS_DIR = .obj
    MOC_DIR = .moc
}

meego {
    DEFINES += Q_COMPONENTS_MEEGO
    CONFIG += meegotouch
}

symbian3 {
    DEFINES += Q_COMPONENTS_SYMBIAN
}
