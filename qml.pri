!symbian {
    for(qmlfile, QML_FILES) {
        ARGUMENTS = $$_PRO_FILE_PWD_/$$qmlfile $$DESTDIR
        target = copy_$$lower($$basename(qmlfile))
        target = $$replace(target, \\., _)
        commands = $${target}.commands
        $$commands += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
        QMAKE_EXTRA_TARGETS += $$target
        POST_TARGETDEPS += $$target
    }

    copy_native {
        NATIVE_DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/Qt/labs/components/native

        unix {
            QMAKE_POST_LINK += $(SYMLINK) -n $$DESTDIR $$NATIVE_DESTDIR
        } else {
            !isEmpty(NATIVESUBPATH):NATIVE_DESTDIR = $$NATIVE_DESTDIR/$$NATIVESUBPATH
            NATIVE_DESTDIR = $$replace(NATIVE_DESTDIR, /, $$QMAKE_DIR_SEP)

            win32:!win32-g++* {
                unixstyle = false
            } else :win32-g++*:isEmpty(QMAKE_SH) {
                unixstyle = false
            } else {
                unixstyle = true
            }

            $$unixstyle {
                CHK_DIR_EXISTS_MKDIR = $$QMAKE_CHK_DIR_EXISTS $$NATIVE_DESTDIR || $$QMAKE_MKDIR $$NATIVE_DESTDIR
            } else {
                CHK_DIR_EXISTS_MKDIR = $$QMAKE_CHK_DIR_EXISTS $$NATIVE_DESTDIR $$QMAKE_MKDIR $$NATIVE_DESTDIR
            }

            mkdir_native.commands += $$CHK_DIR_EXISTS_MKDIR
            QMAKE_EXTRA_TARGETS += mkdir_native

            ARGUMENTS = $$DESTDIR/$(TARGET) $$NATIVE_DESTDIR
            QMAKE_POST_LINK += ($$CHK_DIR_EXISTS_MKDIR) && $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)

            for(qmlfile, QML_FILES) {
                ARGUMENTS = $$_PRO_FILE_PWD_/$$qmlfile $$NATIVE_DESTDIR
                target = copy_native_$$lower($$basename(qmlfile))
                target = $$replace(target, \\., _)
                commands = $${target}.commands
                $$commands += $$QMAKE_COPY $$replace(ARGUMENTS, /, $$QMAKE_DIR_SEP)
                depends = $${target}.depends
                $$depends += mkdir_native
                QMAKE_EXTRA_TARGETS += $$target
                POST_TARGETDEPS += $$target
            }
        }
    }
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
        native_pluginstub.sources = $$_PRO_FILE_PWD_/qmakepluginstubs/$$symbianRemoveSpecialCharacters($$basename(TARGET)).qtplugin
        native_pluginstub.path = /resource/qt/imports/Qt/labs/components/native/$$NATIVESUBPATH

        native_qmlfiles.sources = $$QML_FILES
        native_qmlfiles.path = /resource/qt/imports/Qt/labs/components/native/$$NATIVESUBPATH

        native_qmlimages.sources = $$QML_IMAGES
        native_qmlimages.path = /resource/qt/imports/Qt/labs/components/native/$$NATIVESUBPATH/images

        DEPLOYMENT += native_pluginstub native_qmlfiles native_qmlimages
    } else:unix {
        native_symlink.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH
        native_symlink.commands = $(SYMLINK) -n $$[QT_INSTALL_IMPORTS]/$$TARGETPATH \
            $(INSTALL_ROOT)$$[QT_INSTALL_IMPORTS]/Qt/labs/components/native
        native_symlink.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$$[QT_INSTALL_IMPORTS]/Qt/labs/components/native
        INSTALLS += native_symlink
    } else {
        native_target.CONFIG += no_check_exist executable
        native_target.files = $$DESTDIR/$(TARGET)
        native_target.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native/$$NATIVESUBPATH

        native_qmlfiles.files = $$QML_FILES
        native_qmlfiles.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native/$$NATIVESUBPATH

        native_qmlimages.files = $$QML_IMAGES
        native_qmlimages.path = $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native/$$NATIVESUBPATH/images

        INSTALLS += native_target native_qmlfiles native_qmlimages
    }
}
