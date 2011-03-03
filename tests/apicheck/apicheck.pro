include (../tests.pri)

TEMPLATE = app
TARGET = tst_apicheck
CONFIG += qtestlib testcase
INCLUDEPATH += base

HEADERS += base/apicheckbase.h \
           button/apicheck_button.h \
           checkbox/apicheck_checkbox.h \
           choicelist/apicheck_choicelist.h \
           page/apicheck_page.h \
           pagestack/apicheck_pagestack.h \
           progressbar/apicheck_progressbar.h \
           radiobutton/apicheck_radiobutton.h \
           scrolldecorator/apicheck_scrolldecorator.h \
           slider/apicheck_slider.h \
           tabbutton/apicheck_tabbutton.h \
           tabgroup/apicheck_tabgroup.h \
           textarea/apicheck_textarea.h \
           textfield/apicheck_textfield.h \
           toolbar/apicheck_toolbar.h \
           window/apicheck_window.h

!symbian3:HEADERS += \
           busyindicator/apicheck_busyindicator.h \
           buttoncolumn/apicheck_buttoncolumn.h \
           buttonrow/apicheck_buttonrow.h

SOURCES += tst_apicheck.cpp \
           base/apicheckbase.cpp \
           button/apicheck_button.cpp \
           checkbox/apicheck_checkbox.cpp \
           choicelist/apicheck_choicelist.cpp \
           page/apicheck_page.cpp \
           pagestack/apicheck_pagestack.cpp \
           radiobutton/apicheck_radiobutton.cpp \
           slider/apicheck_slider.cpp \
           tabbutton/apicheck_tabbutton.cpp \
           tabgroup/apicheck_tabgroup.cpp \
           textarea/apicheck_textarea.cpp \
           textfield/apicheck_textfield.cpp \
           toolbar/apicheck_toolbar.cpp \
           window/apicheck_window.cpp

!symbian3:SOURCES += \
           busyindicator/apicheck_busyindicator.cpp \
           buttoncolumn/apicheck_buttoncolumn.cpp \
           buttonrow/apicheck_buttonrow.cpp
