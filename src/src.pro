TEMPLATE = subdirs
SUBDIRS = lib components native

components.depends += lib
native.depends += lib

mx {
    SUBDIRS += mx
    mx.depends += lib
}

meego {
    SUBDIRS += meego meego/themebridge
    meego.depends += lib
    meego-themebridge.depends += lib
}
