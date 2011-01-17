include (../../../qt-components.pri)
include (../../tests.pri)
include (../base/apicheckbase.pri)

TARGET = apicheck_textfield
TEMPLATE = app
QT += declarative

SOURCES += apicheck_textfield.cpp
OTHER_FILES += ApiCheckTextField.qml
