TEMPLATE = subdirs
SUBDIRS = components native

mx {
    SUBDIRS += mx
}

meego {
    SUBDIRS += meego meego/themebridge
}

symbian3 {
    SUBDIRS += symbian symbian/themebridge
}
