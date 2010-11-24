TEMPLATE = subdirs
SUBDIRS = lib module native

mx {
    SUBDIRS += Mx
}

meego {
    SUBDIRS += MeeGo MeeGo/themebridge
}
