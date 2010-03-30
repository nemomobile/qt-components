TARGET = Mx/mxlikeplugin
qmldir.files += Mx/libmxlikeplugin.so #TODO: This is only the lib name on linux...

# Input
HEADERS += plugin/mxlikewindow.h \
            plugin/mxliketextinput.h
SOURCES += plugin/mxlikewindow.cpp \
            plugin/mxliketextinput.cpp \
            plugin/plugin.cpp
