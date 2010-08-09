TEMPLATE = subdirs
SUBDIRS = lib module

mx {
    SUBDIRS += Mx
}

meego {
    SUBDIRS += MeeGo MeeGo/themebridge
}
