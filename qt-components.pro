TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src
examples:SUBDIRS += examples
tests:SUBDIRS += tests

isEmpty(Q_COMPONENTS_SOURCE_TREE)|isEmpty(Q_COMPONENTS_BUILD_TREE) {
    error(Please run configure.)
}

features.files += $$Q_COMPONENTS_SOURCE_TREE/features/qt-components.prf
features.files += $$Q_COMPONENTS_BUILD_TREE/features/qt-components-config.prf
features.path = $$[QT_HOST_DATA]/mkspecs/features
INSTALLS += features

include(doc/doc.pri)
