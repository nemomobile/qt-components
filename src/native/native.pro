TEMPLATE=subdirs
unix {
    symlinkit.path=.
    symlinkit.commands=ln -s $$[QT_INSTALL_IMPORTS]/com/meego $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native
    INSTALLS+=symlinkit
}
