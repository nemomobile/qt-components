/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QGraphicsObject>
#include <QDeclarativeItem>

class tst_tabbutton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultPropertyValues();
    void testClicked();
    void testChecked();

private:
    QScopedPointer<QObject> componentObject;
};

void tst_tabbutton::initTestCase()
{
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_tabbutton.qml", &errors));
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_tabbutton::defaultPropertyValues()
{
    QGraphicsObject *testButton = componentObject->findChild<QGraphicsObject*>("testButton");
    QVERIFY(testButton);
    QVERIFY(testButton->property("tab").isValid());
    QCOMPARE(testButton->property("tab").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
    QVERIFY(testButton->property("width").value<qreal>() > 0);
    QVERIFY(testButton->property("height").value<qreal>() > 0);
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
    QMetaObject::invokeMethod(testButton1, "onClicked");
    QCOMPARE(tabGroup->property("currentTab").value<QGraphicsObject*>(), testButton1->property("tab").value<QGraphicsObject*>());

    // press testButton2 to activate tab2
    QMetaObject::invokeMethod(testButton2, "onClicked");
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


QTEST_MAIN(tst_tabbutton)
#include "tst_tabbutton.moc"
