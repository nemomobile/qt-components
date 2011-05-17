win32:!win32-g++* {
    unixstyle = false
} else:win32-g++*:isEmpty(QMAKE_SH) {
    unixstyle = false
} else {
    unixstyle = true
}

QDOC = $$[QT_INSTALL_BINS]/qdoc3
QHELPGENERATOR = $$[QT_INSTALL_BINS]/qhelpgenerator

symbian3:QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/src/symbian/qt-components.qdocconf
meego:QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/src/meego/qt-components.qdocconf

!isEmpty(QDOCCONF) {
    $$unixstyle {
        html_docs.commands = (QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS] Q_COMPONENTS_SOURCE_TREE=$${Q_COMPONENTS_SOURCE_TREE} $$QDOC $$QDOCCONF)
    } else {
        QDOC = $$replace(QDOC, /, $$QMAKE_DIR_SEP)
        QHELPGENERATOR = $$replace(QHELPGENERATOR, /, $$QMAKE_DIR_SEP)
        html_docs.commands = (set QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS]&& set Q_COMPONENTS_SOURCE_TREE=$${Q_COMPONENTS_SOURCE_TREE}&& $$QDOC $$QDOCCONF)
    }
    qch_docs.depends = html_docs
    qch_docs.commands = (cd $${Q_COMPONENTS_SOURCE_TREE} && $$QHELPGENERATOR doc/html/qtcomponentssymbian.qhp -o doc/qch/qtcomponentssymbian.qch)
    QMAKE_EXTRA_TARGETS += html_docs qch_docs
}

docs.depends = html_docs qch_docs
QMAKE_EXTRA_TARGETS += docs
