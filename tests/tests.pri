include (../qt-components.pri)

QT += declarative
CONFIG -= app_bundle

equals(QT_MAJOR_VERSION, 5): QT += quick

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/tests/shared

DEFINES += Q_COMPONENTS_BUILD_TREE=\\\"$${Q_COMPONENTS_BUILD_TREE}\\\"
