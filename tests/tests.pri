include (../qt-components.pri)

QT += declarative
CONFIG -= app_bundle

INCLUDEPATH += $$Q_COMPONENTS_SOURCE_TREE/tests/shared

#Symbian Qt versions 4.7 and older use double quotes. Qt version 4.8 onwards use single quote. Desktop use single quotes.
#see Qt 4.8 sha:5f0de35a576cae9333c41dbdd43c8489b20e878a.
symbian{
    equals(QT_MAJOR_VERSION, "4") {
        lessThan(QT_MINOR_VERSION, "8") { DEFINES += Q_COMPONENTS_BUILD_TREE=\"\\\"$${Q_COMPONENTS_BUILD_TREE}\\\"\" }
        else { DEFINES += Q_COMPONENTS_BUILD_TREE=\\\"$${Q_COMPONENTS_BUILD_TREE}\\\" }
    } else { DEFINES += Q_COMPONENTS_BUILD_TREE=\\\"$${Q_COMPONENTS_BUILD_TREE}\\\" }
} else { DEFINES += Q_COMPONENTS_BUILD_TREE=\\\"$${Q_COMPONENTS_BUILD_TREE}\\\" }
