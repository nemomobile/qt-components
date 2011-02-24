TEMPLATE = subdirs
SUBDIRS = components

mx {
    SUBDIRS += mx
}

meego {
    SUBDIRS += meego meego/themebridge
}

symbian3 {
    SUBDIRS += symbian
}
