TEMPLATE=subdirs
SUBDIRS=            \
# This one depends on private Qt headers :-(
#    qdatetimemodel  \
    qrangemodel

meego {
   !win32:!macx:SUBDIRS += mlocalthemedaemonclient \
                    mremotethemedaemonclient

    # FIXME: although ostensibly generic API tests, these currently
    # have a hardcoded dependency on meegotouch
    SUBDIRS += \
        api_button \
        api_checkbox \
        api_slider \
        api_pagestack \
        api_progressbar \
        api_scrolldecorator \
        api_textfield \ #TODO not yet finalized
        api_textarea \ #TODO not yet finalized
        api_toolbar
}

symbian3 {
    SUBDIRS += \
        symbian_textfield
}
