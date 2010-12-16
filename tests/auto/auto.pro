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
    #api_lineedit \ #TODO not yet finalized
    #api_multilineedit #TODO not yet finalized

meego {
   SUBDIRS += mremotethemedaemonclient
}