TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src
demos:SUBDIRS += demos
examples:SUBDIRS += examples
tests:SUBDIRS += tests

isEmpty(Q_COMPONENTS_SOURCE_TREE)|isEmpty(Q_COMPONENTS_BUILD_TREE) {
    error(Please run configure.)
}

features.files += $$Q_COMPONENTS_SOURCE_TREE/features/qt-components.prf
features.files += $$Q_COMPONENTS_BUILD_TREE/features/qt-components-config.prf
features.path = $$[QMAKE_MKSPECS]/features
INSTALLS += features

symbian {
    features.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/qt-components.prf $$features.path/qt-components.prf"
    BLD_INF_RULES.prj_exports += "features/qt-components-config.prf $$features.path/qt-components-config.prf"
}

include(doc/doc.pri)
