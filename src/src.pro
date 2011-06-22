TEMPLATE = subdirs
SUBDIRS = components

meego {
    SUBDIRS += meego
    extras:SUBDIRS += meego/extras
}

symbian3 {
    SUBDIRS += symbian
    extras:SUBDIRS += symbian/extras
}
