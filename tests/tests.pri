include (../qt-components.pri)

QT += declarative
CONFIG -= app_bundle

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/tests/shared
DEFINES += Q_COMPONENTS_BUILD_TREE=\"\\\"$${Q_COMPONENTS_BUILD_TREE}\\\"\"
