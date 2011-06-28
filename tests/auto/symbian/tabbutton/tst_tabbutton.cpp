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
#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QGraphicsObject>
#include <QDeclarativeItem>
#include <QDeclarativeContext>

class tst_tabbutton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultPropertyValues();
    void testPlatformInverted();
    void testClicked();
    void testChecked();
    void testGraphicsVisibility();

private:
    QScopedPointer<QObject> componentObject;
    QScopedPointer<QDeclarativeView> view;
};

void tst_tabbutton::initTestCase()
{
    QDeclarativeView *initView = 0;
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_tabbutton.qml", &errors, &initView));
    QVERIFY2(componentObject, qPrintable(errors));
    initView->show();
    QApplication::setActiveWindow(initView);
    QTest::qWaitForWindowShown(initView);
    view.reset(initView);
}

void tst_tabbutton::defaultPropertyValues()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("testButton");
    QVERIFY(testButton);
    QVERIFY(testButton->property("tab").isValid());
    QCOMPARE(testButton->property("tab").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
    QVERIFY(testButton->property("width").value<qreal>() > 0);
    QVERIFY(testButton->property("height").value<qreal>() > 0);

    QVERIFY(testButton->property("checked").isValid());
    QVERIFY(testButton->property("pressed").isValid());
    QVERIFY(testButton->property("text").isValid());
    QVERIFY(testButton->property("iconSource").isValid());

    QVERIFY(testButton->property("platformInverted").isValid());
    QCOMPARE(testButton->property("platformInverted").toBool(), false);
}

void tst_tabbutton::testPlatformInverted()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("testButton");
    QVERIFY(testButton);

    testButton->setProperty("platformInverted", QVariant(true));
    QCOMPARE(testButton->property("platformInverted").toBool(), true);
}

void tst_tabbutton::testClicked()
{
    // clicking a tab button sets the group.currentTab point to the corresponding tab content

    QGraphicsObject *tabGroup = componentObject->findChild<QGraphicsObject*>("tabGroup");
    QVERIFY(tabGroup);

    QGraphicsObject *testButton1 = componentObject->findChild<QGraphicsObject*>("tabButton1");
    QVERIFY(testButton1);

    QGraphicsObject *testButton2 = componentObject->findChild<QGraphicsObject*>("tabButton2");
    QVERIFY(testButton2);

    // press testButton1 to activate tab1
    QMetaObject::invokeMethod(testButton1, "clicked");
    QCOMPARE(tabGroup->property("currentTab").value<QGraphicsObject*>(), testButton1->property("tab").value<QGraphicsObject*>());

    // press testButton2 to activate tab2
    QMetaObject::invokeMethod(testButton2, "clicked");
    QCOMPARE(tabGroup->property("currentTab").value<QGraphicsObject*>(), testButton2->property("tab").value<QGraphicsObject*>());
}

void tst_tabbutton::testChecked()
{
    // checked is switched to true if group.currentTab points to the tab button's content item

    QGraphicsObject *tabGroup = componentObject->findChild<QGraphicsObject*>("tabGroup");
    QVERIFY(tabGroup);

    QGraphicsObject *testButton1 = componentObject->findChild<QGraphicsObject*>("tabButton1");
    QVERIFY(testButton1);

    QGraphicsObject *testButton2 = componentObject->findChild<QGraphicsObject*>("tabButton2");
    QVERIFY(testButton2);

    // set the tab1 content as the currentTab
    tabGroup->setProperty("currentTab", testButton1->property("tab") );
    QVERIFY(testButton1->property("checked").value<bool>());
    QVERIFY(!testButton2->property("checked").value<bool>());

    // set the tab2 content as the currentTab
    tabGroup->setProperty("currentTab", testButton2->property("tab") );
    QVERIFY(testButton2->property("checked").value<bool>());
    QVERIFY(!testButton1->property("checked").value<bool>());
}

void tst_tabbutton::testGraphicsVisibility()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("testButton");
    QVERIFY(testButton);

    QGraphicsObject *image = testButton->findChild<QGraphicsObject*>("image");
    QVERIFY(!image);

    QGraphicsObject *icon = testButton->findChild<QGraphicsObject*>("icon");
    QVERIFY(!icon);

    QGraphicsObject *label = testButton->findChild<QGraphicsObject*>("label");
    QVERIFY(label);

    // set text
    testButton->setProperty("text", QVariant("tab button text"));

    // set portrait orientation
    QDeclarativeContext *context = QDeclarativeEngine::contextForObject(testButton);
    QVERIFY(context);
    QObject *screen = context->contextProperty("screen").value<QObject*>();
    QVERIFY(screen);
    screen->setProperty("allowedOrientations", QVariant(1)); // 1 = SDeclarativeScreen::Portrait
    QTRY_COMPARE(screen->property("currentOrientation"), QVariant(1));

    // Set graphic source to jpg
    testButton->setProperty("iconSource", QVariant("qrc:/non_existing_image.jpg"));

    image = testButton->findChild<QGraphicsObject*>("image");
    QVERIFY(image);
    icon = testButton->findChild<QGraphicsObject*>("icon");
    QVERIFY(!icon);
    QGraphicsObject *imageContainer = qobject_cast<QGraphicsObject *>(image->parent());
    QVERIFY(imageContainer);
    QVERIFY(imageContainer->isVisible());
    QTRY_VERIFY(label->isVisible());

    // Set graphic source to logical icon #1
    testButton->setProperty("iconSource", QVariant("qtg_graf_drill_down_indicator"));
    icon = testButton->findChild<QGraphicsObject*>("icon");
    QVERIFY(icon);

    // Set graphic source to logical icon #2
    testButton->setProperty("iconSource", QVariant("image://theme/qtg_graf_drill_down_indicator.svg"));
    icon = testButton->findChild<QGraphicsObject*>("icon");
    QVERIFY(icon);

    // set landscape orientation -> this should hide the label
    screen->setProperty("allowedOrientations", QVariant(2)); // 2 = SDeclarativeScreen::Landscape
    QTRY_COMPARE(screen->property("currentOrientation"), QVariant(2));
    QVERIFY(imageContainer->isVisible());
    QTRY_VERIFY(!label->isVisible());

    // remove the image -> text should be visible again
    testButton->setProperty("iconSource", QVariant(""));
    QTRY_VERIFY(label->isVisible());
}


QTEST_MAIN(tst_tabbutton)
#include "tst_tabbutton.moc"
