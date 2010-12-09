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
    api_lineedit \
    api_multilineedit

testsxml.files = tests.xml

unix {
  testsxml.path = /usr/share/qt-components-tests
}

INSTALLS += testsxml
