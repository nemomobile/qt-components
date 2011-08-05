win32:!win32-g++* {
    unixstyle = false
} else:win32-g++*:isEmpty(QMAKE_SH) {
    unixstyle = false
} else {
    unixstyle = true
}

QDOC = $$[QT_INSTALL_BINS]/qdoc3

SAMPLEQDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/macros.qdocconf
OUTPUT = $$system($$QDOC $$SAMPLEQDOCCONF 2>&1)
contains(OUTPUT, "-creator") {

   QDOC_ONLINE = -online
    symbian3:{
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt-components-symbian.qdocconf
        QDOCCONF_QCH = $$QDOCCONF
        QHELPFILE = doc/html/qtcomponentssymbian.qhp
        QCHFILE = doc/qch/qtcomponentssymbian.qch
    }
    meego:{
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt-components-meego.qdocconf
        QDOCCONF_QCH = $$QDOCCONF
        QHELPFILE = doc/html/qtcomponentsmeego.qhp
        QCHFILE = doc/qch/qtcomponentsmeego.qch
    }

} else {
    QDOC_ONLINE = " "

    symbian3: {
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-symbian-online.qdocconf
        QDOCCONF_QCH = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-symbian.qdocconf
        QHELPFILE = doc/html/qtcomponentssymbian.qhp
        QCHFILE = doc/qch/qtcomponentssymbian.qch
    }
    meego: {
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-meego-online.qdocconf
        QDOCCONF_QCH = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-meego.qdocconf
        QHELPFILE = doc/html/qtcomponentsmeego.qhp
        QCHFILE = doc/qch/qtcomponentsmeego.qch
    }
}


!isEmpty(QDOCCONF) {
    QHELPGENERATOR = $$[QT_INSTALL_BINS]/qhelpgenerator
    $$unixstyle {
        SET_ENV = QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS] Q_COMPONENTS_SOURCE_TREE=$${Q_COMPONENTS_SOURCE_TREE}
        QDOC = $$replace(QDOC, \\\\, $$QMAKE_DIR_SEP)
        QHELPGENERATOR = $$replace(QHELPGENERATOR, \\\\, $$QMAKE_DIR_SEP)

        HTML_DOCUMENTATION =    $$SET_ENV $$QDOC $$QDOC_ONLINE $$QDOCCONF
        QCH_DOCUMENTATION =     ($$SET_ENV $$QDOC $$QDOCCONF_QCH) && \
                                (cd $${Q_COMPONENTS_SOURCE_TREE}) && \
                                ($$QHELPGENERATOR $$QHELPFILE -o doc/qch/qtcomponentssymbian.qch)

    } else {
        SET_ENV = set QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS]&& set Q_COMPONENTS_SOURCE_TREE=$${Q_COMPONENTS_SOURCE_TREE}


        HTML_DOCUMENTATION =    ($$SET_ENV&& $$QDOC $$QDOC_ONLINE $$QDOCCONF)
        HTML_DOCUMENTATION = $$replace(HTML_DOCUMENTATION, /, $$QMAKE_DIR_SEP)

        QCH_DOCUMENTATION =     ($$SET_ENV&& $$QDOC $$QDOCCONF_QCH && \
                        cd $${Q_COMPONENTS_SOURCE_TREE}&& \
                        $$QHELPGENERATOR $$QHELPFILE -o doc/qch/qtcomponentssymbian.qch)
        QCH_DOCUMENTATION = $$replace(QCH_DOCUMENTATION, /, $$QMAKE_DIR_SEP)

    }

}

QMAKE_EXTRA_TARGETS += qch_docs html_docs

docs.depends = qch_docs html_docs

html_docs.commands = $$HTML_DOCUMENTATION
qch_docs.commands = $$QCH_DOCUMENTATION

QMAKE_EXTRA_TARGETS += docs

