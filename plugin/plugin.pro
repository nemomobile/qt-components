include (../qt-components.pri)

TEMPLATE = lib
TARGET = qtcomponentsplugin
CONFIG += qt plugin
QT += declarative

SOURCES += plugin.cpp

TARGETPATH = Qt/labs/components

target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

QML_FILES = \
	Mx/qmldir \
	Mx/Button.qml    \
	Mx/Common.qml    \
	Mx/Expander.qml  \
	Mx/PathBar.js    \  
	Mx/ProgressBar.qml \  
	Mx/ScrollBar.qml   \
	Mx/ScrollView.qml  \
	Mx/TitlebarCombobox.qml  \
	Mx/TooltipLoader.qml  \
	Mx/TopLevelItem.qml \
	Mx/Combobox.qml  \
	Mx/Entry.qml \   
	Mx/Label.qml \
        Mx/PathBar.qml  \
	Mx/ScrollGrid.qml \  
	Mx/Slider.qml  \ 
	Mx/Toggle.qml  \
	Mx/Tooltip.qml  \
	Mx/Window.qml

QML_IMAGES += \
    Mx/images/button-active.png \
    Mx/images/button-hover.png \
    Mx/images/button.png \
    Mx/images/close-hover.png \
    Mx/images/close.png \
    Mx/images/combobox-marker.png \
    Mx/images/combobox.png \
    Mx/images/combobox-toolbar-marker.png \
    Mx/images/combobox-toolbar.png \
    Mx/images/edit-clear.png \
    Mx/images/edit-find.png \
    Mx/images/entry-active.png \
    Mx/images/entry.png \
    Mx/images/expander-arrow-down-hover.png \
    Mx/images/expander-arrow-down.png \
    Mx/images/expander-arrow-up-hover.png \
    Mx/images/expander-arrow-up.png \
    Mx/images/expander-closed.png \
    Mx/images/expander-hover.png \
    Mx/images/expander-open.png \
    Mx/images/menu-item.png \
    Mx/images/menu.png \
    Mx/images/menu-toolbar.png \
    Mx/images/pathbar-button-active.png \
    Mx/images/pathbar-button-focus.png \
    Mx/images/pathbar-button-hover.png \
    Mx/images/pathbar-button-last-active.png \
    Mx/images/pathbar-button-last-focus.png \
    Mx/images/pathbar-button-last-hover.png \
    Mx/images/pathbar-button-last.png \
    Mx/images/pathbar-button.png \
    Mx/images/plus-button-checked-hover.png \
    Mx/images/plus-button-checked.png \
    Mx/images/plus-button-hover.png \
    Mx/images/plus-button.png \
    Mx/images/progress-bar-background.png \
    Mx/images/progress-bar-bar.png \
    Mx/images/resize-grip.png \
    Mx/images/scroll-button-down-hover.png \
    Mx/images/scroll-button-down.png \
    Mx/images/scroll-button-left-hover.png \
    Mx/images/scroll-button-left.png \
    Mx/images/scroll-button-right-hover.png \
    Mx/images/scroll-button-right.png \
    Mx/images/scroll-button-up-hover.png \
    Mx/images/scroll-button-up.png \
    Mx/images/scroll-hbackground-disabled.png \
    Mx/images/scroll-hbackground.png \
    Mx/images/scroll-hhandle-active.png \
    Mx/images/scroll-hhandle-disabled.png \
    Mx/images/scroll-hhandle-hover.png \
    Mx/images/scroll-hhandle.png \
    Mx/images/scroll-vbackground-disabled.png \
    Mx/images/scroll-vbackground.png \
    Mx/images/scroll-vhandle-active.png \
    Mx/images/scroll-vhandle-disabled.png \
    Mx/images/scroll-vhandle-hover.png \
    Mx/images/scroll-vhandle.png \
    Mx/images/slider-background-active.png \
    Mx/images/slider-background-disabled.png \
    Mx/images/slider-background-focus.png \
    Mx/images/slider-background-hover.png \
    Mx/images/slider-background.png \
    Mx/images/slider-handle-active.png \
    Mx/images/slider-handle-disabled.png \
    Mx/images/slider-handle-focus.png \
    Mx/images/slider-handle-hover.png \
    Mx/images/slider-handle.png \
    Mx/images/toggle-background-active.png \
    Mx/images/toggle-background.png \
    Mx/images/toggle-handle.png \
    Mx/images/toolbar-button-hover.png \
    Mx/images/tooltip-background.png 



qmlfiles.files = $$QML_FILES
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

qmlimages.files = $$QML_IMAGES
qmlimages.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH/images

INSTALLS += target qmlfiles qmlimages
