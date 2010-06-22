include (../qt-components.pri)

DEPENDPATH  += $$INCLUDEPATH .
CONFIG += qtestlib
CONFIG -= app_bundle
QT += declarative

test.files =
test.path = .
test.depends = all
!isEmpty(DESTDIR): test.commands = cd ./$(DESTDIR) &&
macx:      test.commands += ./$(QMAKE_TARGET)
else:unix: test.commands += ./$(QMAKE_TARGET)
else:      test.commands += $(QMAKE_TARGET)
embedded:  test.commands += -qws
INSTALLS += test

QMAKE_EXTRA_TARGETS += test
