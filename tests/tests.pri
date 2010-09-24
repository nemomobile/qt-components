include (../qt-components.pri)

DEPENDPATH  += $$INCLUDEPATH .
CONFIG += qtestlib
CONFIG -= app_bundle
QT += declarative
TEMPLATE = lib ## this is a hack to make qtLibraryTarget return the correct value
LIBS += -L../../src/module -l$$qtLibraryTarget(qtcomponentsplugin)

test.files =
test.path = .
test.depends = all

