TEMPLATE = subdirs

meego {
    SUBDIRS += \
        busyindicator \
        button \
        #buttoncolumn \ #TODO not yet finalized
        #buttonrow \ #TODO not yet finalized
        checkbox \
        #dialog \ #TODO not yet finalized
        #menu \ #TODO not yet finalized
        #pagestack \ #TODO fix PageStack
        progressbar \
        #radiobutton \ #TODO missing component
        #scrolldecorator \ #TODO fix flickableItem
        #selectiondialog \ #TODO missing component
        slider \
        #switch \ #TODO not yet finalized
        #textarea \ #TODO not yet finalized
        #textfield \ #TODO not yet finalized
        #toolbar #TODO not yet finalized
}

symbian3 {
    SUBDIRS += \
        busyindicator \
        button \
        buttoncolumn \
        buttonrow \
        checkbox \
        dialog \
        menu \
        pagestack \
        progressbar \
        radiobutton \
        scrolldecorator \
        selectiondialog \
        slider \
        switch \
        textarea \
        textfield \
        toolbar
}
