TEMPLATE = subdirs
!symbian:SUBDIRS = \
# This one depends on private Qt headers :-(
#    qdatetimemodel \
    qrangemodel

meego {
   !win32:!macx:SUBDIRS += mlocalthemedaemonclient \
                    mremotethemedaemonclient

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
    # The components that are commented are not yet available in Symbian
    # Once a component becomes available the test should be enabled
    SUBDIRS += \
        api_button \
        api_checkbox \
        api_slider \
        #api_pagestack \
        api_progressbar \
        #api_scrolldecorator \ #TODO flickable property not aligned
        #api_textfield \ #TODO not yet finalized
        #api_textarea \ #TODO not yet finalized
        #api_toolbar
}

symbian3 {
    SUBDIRS += \
        symbian_checkbox \
        symbian_declarativeframe \
        symbian_declarativeicon \
        symbian_declarativeimplicitsizeitem \
        symbian_declarativescreen \
        symbian_framepool \
        symbian_iconpool \
        symbian_imageprovider \
        symbian_slider \
        symbian_textarea \
        symbian_textfield \
        symbian_tooltip #TODO not yet finalized
}
