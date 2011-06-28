/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QTest>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include "tst_quickcomponentstest.h"

static const QByteArray QT_COMP_IMPORT_STRING_SYMBIAN = "import com.nokia.symbian 1.1\n";

static void failIfWarnings(QtMsgType type, const char *msg)
{
    Q_UNUSED(msg);
    switch (type) {
    case QtDebugMsg:
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        QFAIL("NO invoke of any QWARN, QCRITICAL nor QFATAL messages should occur.");
    }
}

class tst_scrollbar : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    //symbian specific tests
    void testValidScrollBar();
    void testValidScrollBar_data();
    void testScrollBarWithInvalidFlickable();
    void testScrollBarLoadingWithNoParams();
    void testPropertiesScrollBar();
    //Common API component tests
    void testValidScrollDecorator_data();
    void testValidScrollDecorator();
    void testScrollDecoratorWithInvalidFlickable();
    void testScrollDecoratorLoadingWithNoParams();
    void testPropertiesScrollDecorator();

private:
    //common function for ScrollDecorator and ScrollBar
    void testValid();

private:
    QDeclarativeEngine engine;
};

void tst_scrollbar::initTestCase()
{
    engine.addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
}

void tst_scrollbar::cleanupTestCase()
{
}

void tst_scrollbar::init()
{
}

void tst_scrollbar::cleanup()
{
}

void tst_scrollbar::testValid()
{
    QFETCH(QByteArray, qml);
    QFETCH(bool, isValid);
    QDeclarativeComponent c(&engine);
    c.setData(qml, QUrl());
    QCOMPARE(c.isReady(), isValid);
}

void tst_scrollbar::testScrollBarWithInvalidFlickable()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"Item {\n"
                     +"    id: root\n"
                     +"    Rectangle { id: rect }\n"
                     +"    ScrollBar { flickableItem: rect }\n"
                     +"}";
    QTest::ignoreMessage(QtWarningMsg, "<Unknown File>:6: Unable to assign QObject* to QDeclarativeFlickable*");
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, 0)->children().value(1);//QDeclarativeComponent.create should invoke expected QWARN
    delete obj;
}

void tst_scrollbar::testScrollBarLoadingWithNoParams()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"ScrollBar {\n"
                     +"}";
    QtMsgHandler orig = qInstallMsgHandler(failIfWarnings);
    QString errors;
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, &errors)->children().value(1);
    QVERIFY2(obj, qPrintable(errors));
    qInstallMsgHandler(orig);
    delete obj;
}

void tst_scrollbar::testValidScrollBar_data()
{
    QTest::addColumn<QByteArray>("qml");
    QTest::addColumn<bool>("isValid");
    QByteArray oneDefaultScrollbarWithListView = "import QtQuick 1.1\n"
                                                 +QT_COMP_IMPORT_STRING_SYMBIAN
                                                 +"Item {\n"
                                                 +"    id: root\n"
                                                 +"    ListView { id: flickableArea }\n"
                                                 +"    ScrollBar { flickableItem: flickableArea }\n"
                                                 +"}";
    QByteArray oneDefaultScrollbarWithGridView= "import QtQuick 1.1\n"
                                                +QT_COMP_IMPORT_STRING_SYMBIAN
                                                +"Item {\n"
                                                +"    id: root\n"
                                                +"    GridView { id: flickableArea }\n"
                                                +"    ScrollBar { flickableItem: flickableArea }\n"
                                                +"}";
    QByteArray oneDefaultScrollbarWithFlickable = "import QtQuick 1.1\n"
                                                  +QT_COMP_IMPORT_STRING_SYMBIAN
                                                  +"Item {\n"
                                                  +"    id: root\n"
                                                  +"    Flickable { id: flickableArea }\n"
                                                  +"    ScrollBar { flickableItem: flickableArea }\n"
                                                  +"}";
    QByteArray nonInteractiveScrollbar = "import QtQuick 1.1\n"
                                         +QT_COMP_IMPORT_STRING_SYMBIAN
                                         +"Item {\n"
                                         +"    id: root\n"
                                         +"    Flickable { id: flickableArea }\n"
                                         +"    ScrollBar { interactive: false; flickableItem: flickableArea }\n"
                                         +"}";
    QByteArray interactiveScrollbar = "import QtQuick 1.1\n"
                                      +QT_COMP_IMPORT_STRING_SYMBIAN
                                      +"Item {\n"
                                      +"    id: root\n"
                                      +"    Flickable { id: flickableArea }\n"
                                      +"    ScrollBar { interactive: true; flickableItem: flickableArea }\n"
                                      +"}";
    QByteArray showWhenScrollingScrollbar = "import QtQuick 1.1\n"
                                            +QT_COMP_IMPORT_STRING_SYMBIAN
                                            +"Item {\n"
                                            +"    id: root\n"
                                            +"    Flickable { id: flickableArea }\n"
                                            +"    ScrollBar { policy: Symbian.ScrollBarWhenScrolling; flickableItem: flickableArea }\n"
                                            +"}";
    QByteArray showWhenNeededScrollbar = "import QtQuick 1.1\n"
                                         +QT_COMP_IMPORT_STRING_SYMBIAN
                                         +"Item {\n"
                                         +"    id: root\n"
                                         +"    Flickable { id: flickableArea }\n"
                                         +"    ScrollBar { policy: Symbian.ScrollBarWhenNeeded; flickableItem: flickableArea }\n"
                                         +"}";
    QByteArray horizontalScrollbar = "import QtQuick 1.1\n"
                                     +QT_COMP_IMPORT_STRING_SYMBIAN
                                     +"Item {\n"
                                     +"    id: root\n"
                                     +"    Flickable { id: flickableArea }\n"
                                     +"    ScrollBar { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                                     +"}";
    QByteArray verticalScrollbar = "import QtQuick 1.1\n"
                                   +QT_COMP_IMPORT_STRING_SYMBIAN
                                   +"Item {\n"
                                   +"    id: root\n"
                                   +"    Flickable { id: flickableArea }\n"
                                   +"    ScrollBar { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                                   +"}";
    QByteArray twoScrollbars = "import QtQuick 1.1\n"
                               +QT_COMP_IMPORT_STRING_SYMBIAN
                               +"Item {\n"
                               +"    id: root\n"
                               +"    Flickable { id: flickableArea }\n"
                               +"    ScrollBar { flickableItem: flickableArea }\n"
                               +"    ScrollBar { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                               +"}";
    QTest::newRow("ScrollBar with ListView") << oneDefaultScrollbarWithListView << true;
    QTest::newRow("ScrollBar with GridView") << oneDefaultScrollbarWithGridView << true;
    QTest::newRow("ScrollBar with Flickable") << oneDefaultScrollbarWithFlickable << true;
    QTest::newRow("ScrollBar interactive property set to true") << nonInteractiveScrollbar << true;
    QTest::newRow("ScrollBar interactive property set to false") << interactiveScrollbar << true;
    QTest::newRow("ScrollBar policy set to Symbian.ScrollBarWhenScrolling") << showWhenScrollingScrollbar << true;
    QTest::newRow("ScrollBar policy set to Symbian.ScrollBarWhenNeeded") << showWhenNeededScrollbar << true;
    QTest::newRow("ScrollBar orientation set to Qt.Horizontal") << horizontalScrollbar << true;
    QTest::newRow("ScrollBar orientation set to Qt.Vertical") << verticalScrollbar << true;
    QTest::newRow("Two Scrollbars, one default and one Horizontal") << twoScrollbars << true;
}

void tst_scrollbar::testValidScrollBar()
{
    testValid();
}

void tst_scrollbar::testPropertiesScrollBar()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"Item {\n"
                     +"    id: root\n"
                     +"    Flickable { id: flickableArea }\n"
                     +"    ScrollBar { flickableItem: flickableArea }\n"
                     +"}";
    QString errors;
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, &errors)->children().value(1);
    QVERIFY2(obj, qPrintable(errors));

    QVERIFY(obj->property("interactive").isValid());
    QCOMPARE(obj->property("interactive").toBool(), true);
    obj->setProperty("interactive", false);
    QCOMPARE(obj->property("interactive").toBool(), false);

    QVERIFY(obj->property("flickableItem").isValid());

    QVERIFY(obj->property("orientation").isValid());
    QCOMPARE(obj->property("orientation").toInt(), int(Qt::Vertical));
    obj->setProperty("orientation", Qt::Horizontal);
    QCOMPARE(obj->property("orientation").toInt(), int(Qt::Horizontal));

    QVERIFY(obj->property("policy").isValid());
    QCOMPARE(obj->property("policy").toInt(), 1);
    obj->setProperty("policy", 0);
    QCOMPARE(obj->property("policy").toInt(), 0);

    QVERIFY(obj->property("privateSectionScroller").isValid());
    QCOMPARE(obj->property("privateSectionScroller").toBool(), false);
    obj->setProperty("privateSectionScroller", true);
    QCOMPARE(obj->property("privateSectionScroller").toBool(), true);

    QVERIFY(obj->property("platformInverted").isValid());
    QCOMPARE(obj->property("platformInverted").toBool(), false);
    obj->setProperty("platformInverted", true);
    QCOMPARE(obj->property("platformInverted").toBool(), true);

    delete obj;
}

void tst_scrollbar::testValidScrollDecorator_data()
{
    QTest::addColumn<QByteArray>("qml");
    QTest::addColumn<bool>("isValid");
    QByteArray oneDefaultScrollDecoratorWithListView = "import QtQuick 1.1\n"
                                                       +QT_COMP_IMPORT_STRING_SYMBIAN
                                                       +"Item {\n"
                                                       +"    id: root\n"
                                                       +"    ListView { id: flickableArea }\n"
                                                       +"    ScrollDecorator { flickableItem: flickableArea }\n"
                                                       +"}";
    QByteArray oneDefaultScrollDecoratorWithGridView = "import QtQuick 1.1\n"
                                                       +QT_COMP_IMPORT_STRING_SYMBIAN
                                                       +"Item {\n"
                                                       +"    id: root\n"
                                                       +"    GridView { id: flickableArea }\n"
                                                       +"    ScrollDecorator { flickableItem: flickableArea }\n"
                                                       +"}";
    QByteArray oneDefaultScrollDecoratorWithFlickable = "import QtQuick 1.1\n"
                                                        +QT_COMP_IMPORT_STRING_SYMBIAN
                                                        +"Item {\n"
                                                        +"    id: root\n"
                                                        +"    Flickable { id: flickableArea }\n"
                                                        +"    ScrollDecorator { flickableItem: flickableArea }\n"
                                                        +"}";
    QByteArray nonInteractiveScrollDecorator = "import QtQuick 1.1\n"
                                               +QT_COMP_IMPORT_STRING_SYMBIAN
                                               +"Item {\n"
                                               +"    id: root\n"
                                               +"    Flickable { id: flickableArea }\n"
                                               +"    ScrollDecorator { interactive: false; flickableItem: flickableArea }\n"
                                               +"}";
    QByteArray interactiveScrollDecorator = "import QtQuick 1.1\n"
                                            +QT_COMP_IMPORT_STRING_SYMBIAN
                                            +"Item {\n"
                                            +"    id: root\n"
                                            +"    Flickable { id: flickableArea }\n"
                                            +"    ScrollDecorator { interactive: true; flickableItem: flickableArea }\n"
                                            +"}";
    QByteArray showWhenScrollingScrollDecorator = "import QtQuick 1.1\n"
                                                  +QT_COMP_IMPORT_STRING_SYMBIAN
                                                  +"Item {\n"
                                                  +"    id: root\n"
                                                  +"    Flickable { id: flickableArea }\n"
                                                  +"    ScrollDecorator { policy: Symbian.ScrollBarWhenScrolling; flickableItem: flickableArea }\n"
                                                  +"}";
    QByteArray showWhenNeededScrollDecorator = "import QtQuick 1.1\n"
                                               +QT_COMP_IMPORT_STRING_SYMBIAN
                                               +"Item {\n"
                                               +"    id: root\n"
                                               +"    Flickable { id: flickableArea }\n"
                                               +"    ScrollDecorator { policy: Symbian.ScrollBarWhenNeeded; flickableItem: flickableArea }\n"
                                               +"}";
    QByteArray horizontalScrollDecorator = "import QtQuick 1.1\n"
                                           +QT_COMP_IMPORT_STRING_SYMBIAN
                                           +"Item {\n"
                                           +"    id: root\n"
                                           +"    Flickable { id: flickableArea }\n"
                                           +"    ScrollDecorator { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                                           +"}";
    QByteArray verticalScrollDecorator = "import QtQuick 1.1\n"
                                         +QT_COMP_IMPORT_STRING_SYMBIAN
                                         +"Item {\n"
                                         +"    id: root\n"
                                         +"    Flickable { id: flickableArea }\n"
                                         +"    ScrollDecorator { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                                         +"}";
    QByteArray onPositionChangedScrollDecorator = "import QtQuick 1.1\n"
                                                  +QT_COMP_IMPORT_STRING_SYMBIAN
                                                  +"Item {\n"
                                                  +"    id: root\n"
                                                  +"    Flickable { id: flickableArea }\n"
                                                  +"    ScrollDecorator { onPositionChanged: {} flickableItem: flickableArea; }\n"
                                                  +"}";
    QByteArray onPageSizeChangedScrollDecorator = "import QtQuick 1.1\n"
                                                  +QT_COMP_IMPORT_STRING_SYMBIAN
                                                  +"Item {\n"
                                                  +"    id: root\n"
                                                  +"    Flickable { id: flickableArea }\n"
                                                  +"    ScrollDecorator { onPageSizeChanged: {} flickableItem: flickableArea }\n"
                                                  +"}";
    QByteArray twoScrollDecorators = "import QtQuick 1.1\n"
                                     +QT_COMP_IMPORT_STRING_SYMBIAN
                                     +"Item {\n"
                                     +"    id: root\n"
                                     +"    Flickable { id: flickableArea }\n"
                                     +"    ScrollDecorator { flickableItem: flickableArea }\n"
                                     +"    ScrollDecorator { orientation: Qt.Horizontal; flickableItem: flickableArea }\n"
                                     +"}";
    QTest::newRow("ScrollDecorator with ListView") << oneDefaultScrollDecoratorWithListView << true;
    QTest::newRow("ScrollDecorator with GridView") << oneDefaultScrollDecoratorWithGridView << true;
    QTest::newRow("ScrollDecorator with Flickable") << oneDefaultScrollDecoratorWithFlickable << true;
    QTest::newRow("ScrollDecorator interactive property set to true") << nonInteractiveScrollDecorator << false;
    QTest::newRow("ScrollDecorator interactive property set to false") << interactiveScrollDecorator << false;
    QTest::newRow("ScrollDecorator policy set to Symbian.ScrollBarWhenScrolling") << showWhenScrollingScrollDecorator << false;
    QTest::newRow("ScrollDecorator policy set to Symbian.ScrollBarWhenNeeded") << showWhenNeededScrollDecorator << false;
    QTest::newRow("ScrollDecorator orientation set to Qt.Horizontal") << horizontalScrollDecorator << false;
    QTest::newRow("ScrollDecorator orientation set to Qt.Vertical") << verticalScrollDecorator << false;
    QTest::newRow("ScrollDecorator with (position property) onPositionChanged") << onPositionChangedScrollDecorator << false;
    QTest::newRow("ScrollDecorator with (pageSize property) onPageSizeChanged") << onPageSizeChangedScrollDecorator << false;
    QTest::newRow("Two ScrollDecorators, one default and one Horizontal") << twoScrollDecorators << false;
}

void tst_scrollbar::testValidScrollDecorator()
{
    testValid();
}

void tst_scrollbar::testScrollDecoratorWithInvalidFlickable()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"Item {\n"
                     +"    id: root\n"
                     +"    Rectangle { id: rect }\n"
                     +"    ScrollDecorator { flickableItem: rect }\n"
                     +"}";
    QTest::ignoreMessage(QtWarningMsg, "<Unknown File>:6: Unable to assign QObject* to QDeclarativeFlickable*");
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, 0)->children().value(1);//QDeclarativeComponent.create should invoke expected QWARN
    delete obj;
}
void tst_scrollbar::testScrollDecoratorLoadingWithNoParams()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"ScrollDecorator {\n"
                     +"}";
    QtMsgHandler orig = qInstallMsgHandler(failIfWarnings);
    QString errors;
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, &errors)->children().value(1);
    QVERIFY2(obj, qPrintable(errors));
    qInstallMsgHandler(orig);
    delete obj;
}

void tst_scrollbar::testPropertiesScrollDecorator()
{
    QByteArray qml = "import QtQuick 1.1\n"
                     +QT_COMP_IMPORT_STRING_SYMBIAN
                     +"Item {\n"
                     +"    id: root\n"
                     +"    Flickable { id: flickableArea }\n"
                     +"    ScrollDecorator { flickableItem: flickableArea }\n"
                     +"}";
    QString errors;
    QObject *obj = tst_quickcomponentstest::createComponentFromString(qml, &errors)->children().value(1);
    QVERIFY2(obj, qPrintable(errors));

    QVERIFY(obj->property("flickableItem").isValid());

    QVERIFY(obj->property("platformInverted").isValid());
    QCOMPARE(obj->property("platformInverted").toBool(), false);
    obj->setProperty("platformInverted", true);
    QCOMPARE(obj->property("platformInverted").toBool(), true);

    delete obj;
}

QTEST_MAIN(tst_scrollbar)
#include "tst_scrollbar.moc"
