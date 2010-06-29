HEADERS += chrome/qdeclarativewindow.h

meego {
    SOURCES += chrome/qdeclarativewindow_meego.cpp
} else {
    SOURCES += chrome/qdeclarativewindow_desktop.cpp
}

