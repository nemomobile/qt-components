TEMPLATE = subdirs
SUBDIRS=            \
# This one depends on private Qt headers :-(
#    qdatetimemodel  \
#    qrangemodel

meego {
   !win32:!macx:SUBDIRS += mlocalthemedaemonclient \
                    mremotethemedaemonclient \
                    mwindowstate

    # FIXME: although ostensibly generic API tests, these currently
    # have a hardcoded dependency on meegotouch
    SUBDIRS += \
        api_button \
        api_checkbox \
        api_slider \
        api_busyindicator \
	api_applicationwindow \
        api_radiobutton \
        api_pagestack \
        api_progressbar \
        api_scrolldecorator \
        # api_choicelist \ #TODO choicelist is not common API
        api_buttoncolumn \
        api_buttonrow \
        api_dialog \ #TODO not yet finalized
        api_querydialog \ #TODO not yet finalized
        api_selectiondialog \ #TODO not yet finalized
        api_multiselectiondialog \ #TODO not yet finalized
        api_textfield \ #TODO not yet finalized
        api_tabgroup \
	api_textarea \ #TODO not yet finalized
        api_toolbar \
	api_window \
        api_screen \
        multipleviews
}
