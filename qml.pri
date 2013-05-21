isEmpty(TARGETPATH):error($$basename(_PRO_FILE_) must define TARGETPATH)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/imports/$$member(TARGETPATH, 0)

NATIVE_FILES = $$QML_FILES native/qmldir
NATIVE_FILES -= qmldir

!debug_and_release|CONFIG(release, debug|release): CONFIG += copy_qml_files

copy_qml_files {
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

        for(qmlfile, NATIVE_FILES) {
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

    equals(QT_MAJOR_VERSION, 5) {
        bump_qml_version.commands = $$bumpQmlVersion($$Q_COMPONENTS_BUILD_TREE/imports, 2.0)
        for(post_targetdep, POST_TARGETDEPS): bump_qml_version.depends += $${post_targetdep}
        QMAKE_EXTRA_TARGETS += bump_qml_version
        POST_TARGETDEPS += bump_qml_version
    }
}

OTHER_FILES += $$QML_FILES

target.path = $$[QT_INSTALL_QML]/$$member(TARGETPATH, 0)
INSTALLS += target

for(targetpath, $$list($$unique(TARGETPATH))) {
    installpath = $$[QT_INSTALL_QML]/$$targetpath
    installpath = $$replace(installpath, \\\\, /)
    !isEqual(targetpath, $$member(TARGETPATH, 0)) {
        qmltarget = qmltarget_$$replace(targetpath, /, _)
        eval($${qmltarget}.CONFIG += no_check_exist executable)
        eval($${qmltarget}.files = $$DESTDIR/$(TARGET))
        eval($${qmltarget}.files += stfu)
        eval($${qmltarget}.path = $$installpath)
        INSTALLS += $${qmltarget}
    }

    !debug_and_release|CONFIG(release, debug|release) {
        # Only install these files once if debug_and_release

        qmlfiles = qmlfiles_$$replace(targetpath, /, _)
        eval($${qmlfiles}.files = $$QML_FILES)
        eval($${qmlfiles}.path = $$installpath)

        qmlimages = qmlimages_$$replace(targetpath, /, _)
        eval($${qmlimages}.files = $$QML_IMAGES)
        eval($${qmlimages}.path = $$installpath/images)

        INSTALLS += $${qmlfiles} $${qmlimages}
    }
}

install_native {
    native_target.CONFIG += no_check_exist executable
    native_target.files = $$DESTDIR/$(TARGET)
    native_target.path = $$[QT_INSTALL_QML]/Qt/labs/components/native

    INSTALLS += native_target

    !debug_and_release|CONFIG(release, debug|release) {
        # Only install these files once if debug_and_release
        native_qmlfiles.files = $$NATIVE_FILES
        native_qmlfiles.path = $$[QT_INSTALL_QML]/Qt/labs/components/native

        native_qmlimages.files = $$QML_IMAGES
        native_qmlimages.path = $$[QT_INSTALL_QML]/Qt/labs/components/native/images

        INSTALLS += native_qmlfiles native_qmlimages
    }
}

equals(QT_MAJOR_VERSION, 5):if(!debug_and_release|CONFIG(release, debug|release)) {
    bump_qml_version_installed.CONFIG += no_path
    bump_qml_version_installed.commands = test -d $$[QT_INSTALL_IMPORTS]

    for(install, INSTALLS) {
        isEmpty($${install}.files): next()

        bump_qml_version_installed.depends += install_$${install}
        bump_qml_version_installed.commands += && $$bumpQmlVersion(\$\(INSTALL_ROOT\)$$eval($${install}.path), 2.0)
    }

    INSTALLS += bump_qml_version_installed
}
