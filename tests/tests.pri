include (../qt-components.pri)

DEPENDPATH  += $$INCLUDEPATH .
CONFIG += qtestlib
CONFIG -= app_bundle
QT += declarative
LIBS += -L../../src/module -lqtcomponentsplugin

test.files =
test.path = .
test.depends = all

