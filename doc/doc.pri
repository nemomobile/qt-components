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
    meego:{
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt-components-meego.qdocconf
        QDOCCONF_QCH = $$QDOCCONF
        QHELPFILE = doc/html/qtcomponentsmeego.qhp
        QCHFILE = doc/qch/qtcomponentsmeego.qch
    }

} else {
    QDOC_ONLINE = " "

    meego: {
        QDOCCONF = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-meego-online.qdocconf
        QDOCCONF_QCH = $${Q_COMPONENTS_SOURCE_TREE}/doc/config/qt474/qt-components-meego.qdocconf
        QHELPFILE = doc/html/qtcomponentsmeego.qhp
        QCHFILE = doc/qch/qtcomponentsmeego.qch
    }
}


QMAKE_EXTRA_TARGETS += qch_docs html_docs

docs.depends = qch_docs html_docs

html_docs.commands = $$HTML_DOCUMENTATION
qch_docs.commands = $$QCH_DOCUMENTATION

QMAKE_EXTRA_TARGETS += docs

