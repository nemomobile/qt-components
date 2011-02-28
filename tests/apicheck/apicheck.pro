include (../../qt-components.pri)
include (../tests.pri)

TEMPLATE = app
TARGET = tst_apicheck
CONFIG += qtestlib testcase
INCLUDEPATH += base

HEADERS += base/apicheckbase.h \
           button/apicheck_button.h \
           checkbox/apicheck_checkbox.h \
           choicelist/apicheck_choicelist.h \
           slider/apicheck_slider.h \
           textfield/apicheck_textfield.h \
           progressbar/apicheck_progressbar.h \
           radiobutton/apicheck_radiobutton.h \
           textarea/apicheck_textarea.h \
           scrolldecorator/apicheck_scrolldecorator.h \
           page/apicheck_page.h \
           pagestack/apicheck_pagestack.h

!symbian3:HEADERS += \
           busyindicator/apicheck_busyindicator.h \
           buttoncolumn/apicheck_buttoncolumn.h \
           buttonrow/apicheck_buttonrow.h \
           toolbar/apicheck_toolbar.h

SOURCES += tst_apicheck.cpp \
           base/apicheckbase.cpp \
           button/apicheck_button.cpp \
           checkbox/apicheck_checkbox.cpp \
           choicelist/apicheck_choicelist.cpp \
           slider/apicheck_slider.cpp \
           textfield/apicheck_textfield.cpp \
           radiobutton/apicheck_radiobutton.cpp \
           textarea/apicheck_textarea.cpp \
           page/apicheck_page.cpp \
           pagestack/apicheck_pagestack.cpp

!symbian3:SOURCES += \
           busyindicator/apicheck_busyindicator.cpp \
           buttoncolumn/apicheck_buttoncolumn.cpp \
           buttonrow/apicheck_buttonrow.cpp \
           toolbar/apicheck_toolbar.cpp
