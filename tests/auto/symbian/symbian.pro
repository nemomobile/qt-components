TEMPLATE = subdirs

SUBDIRS += \
    button \
    checkbox \
    declarativeframe \
    declarativeicon \
    declarativeimplicitsizeitem \
    declarativestyle \
    framepool \
    iconpool \
    imageprovider \
    listheading \
    listitem \
    listitemtext \
    page \
    pagestack \
    progressbar \
    radiobutton \
    scrollbar \
    selectionlistitem \
    slider \
    statusbar \
    switch \
    tabbar \
    tabbarlayout \
    tabbutton \
    tabgroup \
    textarea \
    textfield \
    toolbar \
    toolbutton \
    tooltip #TODO not yet finalized

#TODO fail on mac, to be fixed
!macx:SUBDIRS += \
    applicationwindow \
    declarativescreen \
    window
