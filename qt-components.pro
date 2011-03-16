TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src
tests:SUBDIRS += tests
examples:SUBDIRS += examples

isEmpty(Q_COMPONENTS_SOURCE_TREE)|isEmpty(Q_COMPONENTS_BUILD_TREE) {
    error(Please run configure.)
}

features.files += $$Q_COMPONENTS_SOURCE_TREE/features/qt-components.prf
features.files += $$Q_COMPONENTS_BUILD_TREE/features/qt-components-config.prf
features.path = $$[QMAKE_MKSPECS]/features
INSTALLS += features

symbian {
    BLD_INF_RULES.prj_exports += "features/qt-components.prf $$[QMAKE_MKSPECS]/features/qt-components.prf"
    BLD_INF_RULES.prj_exports += "features/qt-components-config.prf $$[QMAKE_MKSPECS]/features/qt-components-config.prf"
}
