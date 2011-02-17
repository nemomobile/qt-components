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
    TARGET.CAPABILITY = CAP_APPLICATION AllFiles
    componenttest_qmls1.sources = *.qml
    componenttest_qmls2.sources = tests/*.qml
    componenttest_qmls2.path = tests
    componenttest_qmls3.sources = tests/utils/*.qml
    componenttest_qmls3.path = tests/utils
    componenttest_js.sources = *.js
    DEPLOYMENT += componenttest_qmls1 componenttest_qmls2 componenttest_qmls3 componenttest_js
    BLD_INF_RULES.prj_exports += "componenttest.iby $$CORE_APP_LAYER_IBY_EXPORT_PATH(componenttest.iby)"
}

OTHER_FILES += \
    main.qml \
    tests/Button.qml \
    tests/CheckBox.qml \
    tests/ChoiceList.qml \
    tests/Dialog.qml \
    tests/ListItem.qml \
    tests/ListView.qml \
    tests/Metrics.qml \
    tests/ObjectMenu.qml \
    tests/ProgressBar.qml \
    tests/RadioButton.qml \
    tests/ScrollBar.qml \
    tests/ScrollDecorator.qml \
    tests/Slider.qml \
    tests/ViewMenu.qml

