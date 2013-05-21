TEMPLATE = subdirs
SUBDIRS = components

meego {
    SUBDIRS += meego
    extras:SUBDIRS += meego/extras
}

