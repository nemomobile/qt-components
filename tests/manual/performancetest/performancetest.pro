include (../../tests.pri)

TARGET = performancetest
TEMPLATE = app
QT += declarative svg
CONFIG -= app_bundle
win32:DESTDIR = $$OUT_PWD
!symbian: include (../../shared/settingswindow.pri)

SOURCES += performancetest.cpp
RESOURCES += performancetest.qrc

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x200346E6
    TARGET.EPOCHEAPSIZE = 0x1000 0xF00000
    TARGET.CAPABILITY = CAP_APPLICATION AllFiles
    performancetest_qmls1.sources = *.qml
    performancetest_qmls2.sources = tests/*.qml
    performancetest_qmls2.path = tests
    performancetest_qmls3.sources = tests/utils/*.qml
    performancetest_qmls3.path = tests/utils
    performancetest_js.sources = *.js
    DEPLOYMENT += performancetest_qmls1 performancetest_qmls2 performancetest_qmls3 performancetest_js
    BLD_INF_RULES.prj_exports += "performancetest.iby /epoc32/rom/include/core/app/performancetest.iby"
}

OTHER_FILES += \
    main.qml

