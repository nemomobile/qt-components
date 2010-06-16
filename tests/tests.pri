INCLUDEPATH += $$PWD/../src $$PWD/../src/models
DEPENDPATH  += $$INCLUDEPATH .
LIBS += -L../../lib -lqtcomponents
QMAKE_RPATHDIR = $$OUT_PWD/../../lib
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
