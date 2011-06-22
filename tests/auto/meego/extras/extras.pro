TEMPLATE=app
TARGET=tst_extras
CONFIG += qmltestcase warn_on quicktest
SOURCES += main.cpp

# this reads the QML files from the same directory as this pro file
DEFINES += QUICK_TEST_SOURCE_DIR=\"\\\"$$PWD\\\"\"

OTHER_FILES += *.qml
