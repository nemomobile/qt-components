TEMPLATE = subdirs
SUBDIRS = lib module native

module.depends += lib
native.depends += lib

mx {
    SUBDIRS += Mx
    Mx.depends += lib
}

meego {
    SUBDIRS += MeeGo MeeGo/themebridge
    MeeGo.depends += lib
    MeeGo-themebridge.depends += lib
}
