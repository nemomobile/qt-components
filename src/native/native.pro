TEMPLATE=subdirs
unix:!symbian:meego {
    symlinkit.CONFIG += no_path
    symlinkit.commands=$(SYMLINK) $$[QT_INSTALL_IMPORTS]/com/meego $(INSTALL_ROOT)$$[QT_INSTALL_IMPORTS]/Qt/labs/components/native
    INSTALLS += symlinkit
}
