TEMPLATE=subdirs
SUBDIRS=            \
# This one depends on private Qt headers :-(
#    qdatetimemodel  \
    qrangemodel \
    api_button \
    api_checkbox \
    api_slider \
    api_progressbar \
    api_scrolldecorator \
    #api_textfield \ #TODO not yet finalized
    #api_textarea #TODO not yet finalized

meego {
   !macx:SUBDIRS += mlocalthemedaemonclient \
                    mremotethemedaemonclient
}
