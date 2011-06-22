CONFIG += qtestlib testcase

autotests {
    INSTALLS += target qmlfiles
    qmlfiles.files = $$OTHER_FILES
    qmlfiles.path = $$INSTALL_PREFIX/lib/qt-components-tests/auto/meego/$$TARGET
    target.path = $$INSTALL_PREFIX/lib/qt-components-tests/auto/meego/$$TARGET
}

symbian {
    imports.sources = $$OTHER_FILES
    imports.path = .
    DEPLOYMENT += imports
}
