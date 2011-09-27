TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src
demos:SUBDIRS += demos
examples:SUBDIRS += examples
tests:SUBDIRS += tests

isEmpty(Q_COMPONENTS_SOURCE_TREE)|isEmpty(Q_COMPONENTS_BUILD_TREE) {
    error(Please run configure.)
}

features.files += $$Q_COMPONENTS_SOURCE_TREE/features/qtcomponents_1_1.prf
features.files += $$Q_COMPONENTS_BUILD_TREE/features/qtcomponents_1_1_config.prf
features.path = $$[QMAKE_MKSPECS]/features
INSTALLS += features

symbian {
    features.path ~= s/^[A-Za-z]:/ # strip drive letter
    BLD_INF_RULES.prj_exports += "features/qtcomponents_1_1.prf $$features.path/qtcomponents_1_1.prf"
    BLD_INF_RULES.prj_exports += "features/qtcomponents_1_1_config.prf $$features.path/qtcomponents_1_1_config.prf"
}

include(doc/doc.pri)
