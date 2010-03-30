TARGET = Mx/mxlikeplugin
qmldir.files += Mx/libmxlikeplugin.so #TODO: This is only the lib name on linux...

# Input
HEADERS += plugin/mxwindow.h
SOURCES += plugin/mxwindow.cpp \
            plugin/plugin.cpp
