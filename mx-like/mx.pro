TEMPLATE = lib
CONFIG += qt plugin
QT += declarative
MOC_DIR = ./.moc
OBJECTS_DIR = ./.obj

qmldir.files += Mx
qmldir.path += $$[QT_INSTALL_IMPORTS]
INSTALLS += qmldir
include(plugin/plugin.pri)
