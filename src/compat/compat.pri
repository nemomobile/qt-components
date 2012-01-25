# -------------------------------------------------------------------
# Compatability hacks to build against Qt5 without changing the
# sources too much. Tries to mask over the differences between
# QtQuick1 and QtQuick2.
# -------------------------------------------------------------------

SOURCES += \
    $$PWD/qgraphicsitem.cpp

HEADERS += \
    $$PWD/qgraphicsitem.h \
    $$PWD/qgraphicsobject.h \
    $$PWD/qdeclarativeitem.h \
    $$PWD/qdeclarativeview.h \
    $$PWD/qgraphicsscene.h

INCLUDEPATH += $$PWD

QT += quick

# Prevent symbol clashes in case QtWidget is loaded
DEFINES += \
    QGraphicsItem=CompatQGraphicsItem \
    QGraphicsObject=CompatQGraphicsObject \
    QGraphicsScene=CompatQGraphicsScene \
    QDeclarativeItem=CompatQDeclarativeItem \
    QDeclarativeView=CompatQDeclarativeView


defineReplace(replaceRecursivly) {
    root = $$1
    files = $$2
    pattern = $$3

    SED = sed -i
    macx: SED += \"\"

    return(find $${root} -name \"$${files}\" -type f -print0 | xargs -r -0 $${SED} -e \'$${pattern}\')
}

defineReplace(bumpQmlVersion) {
    root = $$1
    version = $$2

    # QML files
    pattern = \\(^import\\s.*\\)1\\.[0|1]\\(\\s\\|$${LITERAL_DOLLAR}$${LITERAL_DOLLAR}\\)
    command = $$replaceRecursivly($$root, *.qml, s/$${pattern}/\\1$${version}\\2/)

    # qmldir
    pattern = \\(\\s\\)1\\.[0|1]\\(\\s\\)
    command += && $$replaceRecursivly($$root, qmldir, s/$${pattern}/\\1$${version}\\2/)

    return($$command)
}
