include (../../qt-components.pri)

TEMPLATE = lib
TARGET = $$qtLibraryTarget(mxplugin)
DESTDIR = $$Q_COMPONENTS_BUILD_TREE/lib
win32:DLLDESTDIR = $$Q_COMPONENTS_BUILD_TREE/bin
CONFIG += qt plugin
QT += declarative

SOURCES += \
    plugin.cpp \
    qmxbuttongroup.cpp \
    qmxtoplevelitem.cpp \
    qmxwindow.cpp

HEADERS += \
    qmxbuttongroup.h \
    qmxtoplevelitem.h \
    qmxtoplevelitem_p.h \
    qmxwindow.h

INCLUDEPATH += $$PWD

QML_FILES = \
        qmldir \
        Button.qml \
        Common.qml \
        Expander.qml \
        PathBar.js \
        ProgressBar.qml \
        ScrollBar.qml \
        ScrollView.qml \
        TitlebarCombobox.qml \
        TooltipLoader.qml \
        TopLevelItem.qml \
        Combobox.qml \
        Entry.qml \
        Label.qml \
        PathBar.qml \
        ScrollGrid.qml \
        Slider.qml \
        Toggle.qml \
        Tooltip.qml \
        Window.qml

QML_IMAGES += \
    images/button-active.png \
    images/button-hover.png \
    images/button.png \
    images/close-hover.png \
    images/close.png \
    images/combobox-marker.png \
    images/combobox.png \
    images/combobox-toolbar-marker.png \
    images/combobox-toolbar.png \
    images/edit-clear.png \
    images/edit-find.png \
    images/entry-active.png \
    images/entry.png \
    images/expander-arrow-down-hover.png \
    images/expander-arrow-down.png \
    images/expander-arrow-up-hover.png \
    images/expander-arrow-up.png \
    images/expander-closed.png \
    images/expander-hover.png \
    images/expander-open.png \
    images/menu-item.png \
    images/menu.png \
    images/menu-toolbar.png \
    images/pathbar-button-active.png \
    images/pathbar-button-focus.png \
    images/pathbar-button-hover.png \
    images/pathbar-button-last-active.png \
    images/pathbar-button-last-focus.png \
    images/pathbar-button-last-hover.png \
    images/pathbar-button-last.png \
    images/pathbar-button.png \
    images/plus-button-checked-hover.png \
    images/plus-button-checked.png \
    images/plus-button-hover.png \
    images/plus-button.png \
    images/progress-bar-background.png \
    images/progress-bar-bar.png \
    images/resize-grip.png \
    images/scroll-button-down-hover.png \
    images/scroll-button-down.png \
    images/scroll-button-left-hover.png \
    images/scroll-button-left.png \
    images/scroll-button-right-hover.png \
    images/scroll-button-right.png \
    images/scroll-button-up-hover.png \
    images/scroll-button-up.png \
    images/scroll-hbackground-disabled.png \
    images/scroll-hbackground.png \
    images/scroll-hhandle-active.png \
    images/scroll-hhandle-disabled.png \
    images/scroll-hhandle-hover.png \
    images/scroll-hhandle.png \
    images/scroll-vbackground-disabled.png \
    images/scroll-vbackground.png \
    images/scroll-vhandle-active.png \
    images/scroll-vhandle-disabled.png \
    images/scroll-vhandle-hover.png \
    images/scroll-vhandle.png \
    images/slider-background-active.png \
    images/slider-background-disabled.png \
    images/slider-background-focus.png \
    images/slider-background-hover.png \
    images/slider-background.png \
    images/slider-handle-active.png \
    images/slider-handle-disabled.png \
    images/slider-handle-focus.png \
    images/slider-handle-hover.png \
    images/slider-handle.png \
    images/toggle-background-active.png \
    images/toggle-background.png \
    images/toggle-handle.png \
    images/toolbar-button-hover.png \
    images/tooltip-background.png

TARGETPATH = Qt/labs/Mx
include(../../qml.pri)
