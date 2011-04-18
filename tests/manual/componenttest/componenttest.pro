include (../../tests.pri)

TARGET = componenttest
TEMPLATE = app
QT += declarative svg
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD
!symbian: include (../shared/settingswindow.pri)
include (../shared/utils.pri)

SOURCES += componenttest.cpp
RESOURCES += componenttest.qrc

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x200346E0
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = ReadDeviceData SwEvent WriteDeviceData
    componenttest_qmls1.sources = *.qml
    componenttest_qmls2.sources = tests/*.qml
    componenttest_qmls2.path = tests
    componenttest_qmls3.sources = tests/utils/*.qml
    componenttest_qmls3.path = tests/utils
    componenttest_qmls4.sources = tests/content/*.qml
    componenttest_qmls4.path = tests/content
    componenttest_js.sources = *.js
    DEPLOYMENT += componenttest_qmls1 componenttest_qmls2 componenttest_qmls3 componenttest_qmls4 componenttest_js
    BLD_INF_RULES.prj_exports += "componenttest.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(componenttest.iby)"
}

OTHER_FILES += \
    main.qml \
    components/FontSelectionDialog.qml \
    tests/BusyIndicator.qml \
    tests/Button.qml \
    tests/ButtonRow.qml \
    tests/ButtonColumn.qml \
    tests/CheckBox.qml \
    tests/ChoiceList.qml \
    tests/ContextMenu.qml \
    tests/Dialog.qml \
    tests/KeyNavigation.qml \
    tests/ListItem.qml \
    tests/ListView.qml \
    tests/Metrics.qml \
    tests/Menu.qml \
    tests/Page.qml \
    tests/PageStack.qml \
    tests/ProgressBar.qml \
    tests/QueryDialog.qml \
    tests/RadioButton.qml \
    tests/RatingIndicator.qml \
    tests/ScrollBar.qml \
    tests/ScrollDecorator.qml \
    tests/SectionScroller.qml \
    tests/Slider.qml \
    tests/StyleColors.qml \
    tests/StyleFonts.qml \
    tests/StyleLayouts.qml \
    tests/StatusBar.qml \
    tests/Switch.qml \
    tests/Tabs.qml \
    tests/TextArea.qml \
    tests/TextField.qml \
    tests/TextFieldFont.qml \
    tests/TextFieldMaxLength.qml \
    tests/TextFieldOther.qml \
    tests/TextFieldSelection.qml \
    tests/TextFocus.qml \
    tests/ToolBar.qml \
    tests/ToolButton.qml \
    tests/ToolTip.qml \
    tests/Tumbler.qml \
    tests/TumblerDialog.qml \
    tests/content/InputMethodHints.qml
