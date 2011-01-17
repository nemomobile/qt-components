!symbian:for(qmlfile, QML_FILES) {
    ARGUMENTS = $$qmlfile $$DESTDIR
    !isEmpty(QMAKE_POST_LINK):QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
}

OTHER_FILES += $$QML_FILES

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlimages.files = $$QML_IMAGES
qmlimages.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/images

INSTALLS += target qmlfiles qmlimages

symbian {
    pluginstub.sources = $$symbianRemoveSpecialCharacters($$basename(TARGET)).dll
    pluginstub.path = /resource/qt/imports/$$TARGETPATH

    resources.sources = $$QML_FILES
    resources.path = /resource/qt/imports/$$TARGETPATH

    DEPLOYMENT += pluginstub resources
}

install_native {
    symbian {
        native_pluginstub.sources = $$symbianRemoveSpecialCharacters($$basename(TARGET)).dll
        native_pluginstub.path = /resource/qt/imports/Qt/labs/components/native

        native_resources.sources = $$QML_FILES $$QML_IMAGES
        native_resources.path = /resource/qt/imports/Qt/labs/components/native

        DEPLOYMENT += native_pluginstub native_resources
    } else:unix {
        native_symlink.CONFIG += no_path
        native_symlink.commands = $(SYMLINK) -n $$[QT_INSTALL_IMPORTS]/$$TARGETPATH \
            $(INSTALL_ROOT)$$[QT_INSTALL_IMPORTS]/Qt/labs/components/native
        INSTALLS += native_symlink
    } else {
        native_target.CONFIG += no_check_exist executable
        native_target.files = $$DESTDIR/$(TARGET)
        native_target.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native

        native_qmlfiles.files = $$QML_FILES
        native_qmlfiles.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native

        native_qmlimages.files = $$QML_IMAGES
        native_qmlimages.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native/images

        INSTALLS += native_target native_qmlfiles native_qmlimages
    }
}
