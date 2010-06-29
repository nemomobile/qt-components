HEADERS += chrome/quickwindow.h

meegotouch {
    SOURCES += chrome/quickwindow_meego.cpp
    DEFINES += Q_WS_MEEGO
} else {
    SOURCES += chrome/quickwindow_desktop.cpp
}

