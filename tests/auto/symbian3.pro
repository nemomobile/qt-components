TEMPLATE = subdirs

SUBDIRS += \
    symbian_button \
    symbian_checkbox \
    symbian_declarativeframe \
    symbian_declarativeicon \
    symbian_declarativeimplicitsizeitem \
    symbian_declarativestyle \
    symbian_framepool \
    symbian_iconpool \
    symbian_imageprovider \
    symbian_listheading \
    symbian_listitem \
    symbian_listitemtext \
    symbian_page \
    symbian_pagestack \
    symbian_progressbar \
    symbian_radiobutton \
    symbian_scrollbar \
    symbian_slider \
    symbian_tabbar \
    symbian_tabbarlayout \
    symbian_tabbutton \
    symbian_tabgroup \
    symbian_textarea \
    symbian_textfield \
    symbian_toolbutton \
    symbian_tooltip #TODO not yet finalized

#TODO fail on mac, to be fixed
!macx:SUBDIRS += \
    symbian_applicationwindow \
    symbian_declarativescreen \
    symbian_window
