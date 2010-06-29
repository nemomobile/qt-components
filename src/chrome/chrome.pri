HEADERS += chrome/quickwindow.h

meego {
    SOURCES += chrome/quickwindow_meego.cpp
} else {
    SOURCES += chrome/quickwindow_desktop.cpp
}

