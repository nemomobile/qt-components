TEMPLATE = app
QT += declarative
target.path = $$INSTALL_PREFIX/bin

equals(QT_MAJOR_VERSION, 5) {
    include(../../../src/compat/compat.pri)
    QT += widgets
} else {
    include(../util/util.pri)
}

CONFIG -= app_bundle

# Input
SOURCES += main.cpp

OTHER_FILES += \
    qml/*.qml

desktop.files = qmlcomponentgallery.desktop
desktop.path = $$INSTALL_PREFIX/share/applications

equals(QT_MAJOR_VERSION, 5) {
    copied_qrc_file = $$OUT_PWD/qml/qmlfiles_copy.qrc

    # Prevent qmake from complaining about missing qrc file
    system($$QMAKE_MKDIR $$OUT_PWD/qml && touch $$copied_qrc_file )

    copy_qml_files.commands = $$QMAKE_COPY_DIR $$PWD/qml $$OUT_PWD

    # Prevent qmake from being confused and choosing the qrc file from the source dir
    copy_qml_files.commands += && $$QMAKE_MOVE $$OUT_PWD/qml/qmlfiles.qrc $$copied_qrc_file

    rewrite_qml_files.commands = $$bumpQmlVersion($$OUT_PWD/qml, 2.0)
    rewrite_qml_files.depends = copy_qml_files

    QMAKE_EXTRA_TARGETS += copy_qml_files rewrite_qml_files
    rcc.depends += copy_qml_files rewrite_qml_files

    RESOURCES = $$copied_qrc_file
} else {
    RESOURCES = qml/qmlfiles.qrc
}

# Don't overwrite default gallery example on device
lessThan(QT_MAJOR_VERSION, 5): INSTALLS += target desktop
