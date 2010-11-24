TEMPLATE=subdirs
!exists("$$[QT_INSTALL_IMPORTS]/Qt/labs/components/native") {
    !symbian {
    	system("ln -s $$[QT_INSTALL_IMPORTS]/com/meego $$[QT_INSTALL_IMPORTS]/Qt/labs/components/native")
    }
}
