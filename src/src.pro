TEMPLATE = subdirs
SUBDIRS = components

meego {
    SUBDIRS += meego meego/themebridge
}

symbian3 {
    SUBDIRS += symbian symbian/extras
}
