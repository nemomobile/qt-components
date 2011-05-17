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
#include <QDeclarativeComponent>

class tst_tabgroup : public QObject
{
    Q_OBJECT

public:
    tst_tabgroup() : componentObject(0), view(0) {}

private slots:
    void init();
    void cleanup();
    void defaultPropertyValues();
    void testCurrentTabInitializationStatic();
    void testCurrentTabInitializationStatic2();
    void testCurrentTabInitializationDynamic();
    void testReparentingStatic();
    void testReparentingDynamic();
    void testAddRemoveTabs();
    void testPageSignaling();

private:
    QScopedPointer<QObject> componentObject;
    QDeclarativeView *view;
};

void tst_tabgroup::init()
{
    QString errors;
    componentObject.reset(tst_quickcomponentstest::createComponentFromFile("tst_tabgroup.qml", &errors, &view));
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_tabgroup::cleanup()
{
    // some of the test cases modify the object tree, so we need to delete (and re-create) it
    // between every test case.
    componentObject.reset();
}

void tst_tabgroup::defaultPropertyValues()
{
    QGraphicsObject *testGroup = componentObject->findChild<QGraphicsObject*>("testGroup");
    QVERIFY(testGroup);
    QVERIFY(testGroup->property("currentTab").isValid());
    QCOMPARE(testGroup->property("currentTab").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_tabgroup::testCurrentTabInitializationStatic()
{
    // check that first child item is set as the current tab by default

    QGraphicsObject *tabGroup1 = componentObject->findChild<QGraphicsObject*>("tabGroup1");
    QVERIFY(tabGroup1);

    QGraphicsObject *group1tab1 = componentObject->findChild<QGraphicsObject*>("group1tab1");
    QVERIFY(group1tab1);

    QVERIFY(tabGroup1->property("currentTab").value<QDeclarativeItem*>() == group1tab1);
}

void tst_tabgroup::testCurrentTabInitializationStatic2()
{
    // check that user can override the initial current tab

    QGraphicsObject *tabGroup3 = componentObject->findChild<QGraphicsObject*>("tabGroup3");
    QVERIFY(tabGroup3);

    QGraphicsObject *group3tab2 = componentObject->findChild<QGraphicsObject*>("group3tab2");
    QVERIFY(group3tab2);

    QVERIFY(tabGroup3->property("currentTab").value<QDeclarativeItem*>() == group3tab2);
}

QObject *findQmlChild(QObject *object, const QString &objectName) {
    if (!object)
        return 0;

    if (object->objectName() == objectName)
        return object;

    // find object tree
    QObject *result = object->findChild<QObject*>(objectName);

    if (!result) {
        // not found from the object tree. try the graphics item tree
        QGraphicsObject *asGraphicsObject = qobject_cast<QGraphicsObject*>(object);
        if (asGraphicsObject) {
            for (int i = 0; i < asGraphicsObject->childItems().count() && !result; i++) {
                result = findQmlChild(asGraphicsObject->childItems().at(i)->toGraphicsObject(), objectName);
            }
        }
    }
    return result;
}

void tst_tabgroup::testCurrentTabInitializationDynamic()
{
    // check that user can override the initial current tab

    QDeclarativeItem *tabGroup2 = componentObject->findChild<QDeclarativeItem*>("tabGroup2");
    QVERIFY(tabGroup2);

    QGraphicsObject *group2tab2 = qobject_cast<QGraphicsObject*>(findQmlChild(tabGroup2,"group2tab2"));
    QVERIFY(group2tab2);

    QVERIFY(tabGroup2->property("currentTab").value<QDeclarativeItem*>() == group2tab2);
}

void tst_tabgroup::testReparentingStatic()
{
    // test initial reparenting works for static children

    QDeclarativeItem *tabGroup1 = componentObject->findChild<QDeclarativeItem*>("tabGroup1");
    QVERIFY(tabGroup1);

    QDeclarativeItem *group1tab1 = componentObject->findChild<QDeclarativeItem*>("group1tab1");
    QVERIFY(group1tab1);

    QDeclarativeItem *group1tab2 = componentObject->findChild<QDeclarativeItem*>("group1tab2");
    QVERIFY(group1tab2);

    QVERIFY(group1tab1->parent());
    QCOMPARE(group1tab1->parentItem()->parentItem()->parentItem(), tabGroup1);
    QVERIFY(group1tab2->parent());
    QCOMPARE(group1tab2->parentItem()->parentItem()->parentItem(), tabGroup1);
}

void tst_tabgroup::testReparentingDynamic()
{
    // test initial reparenting works for dunamic children

    QDeclarativeItem *tabGroup2 = componentObject->findChild<QDeclarativeItem*>("tabGroup2");
    QVERIFY(tabGroup2);

    QDeclarativeItem *group2tab1 = qobject_cast<QDeclarativeItem*>(findQmlChild(tabGroup2,"group2tab1"));
    QVERIFY(group2tab1);

    QDeclarativeItem *group2tab2 = qobject_cast<QDeclarativeItem*>(findQmlChild(tabGroup2,"group2tab2"));
    QVERIFY(group2tab2);

    QVERIFY(group2tab1->parentItem());
    QCOMPARE(group2tab1->parentItem()->parentItem()->parentItem(), tabGroup2);
    QVERIFY(group2tab2->parentItem());
    QCOMPARE(group2tab2->parentItem()->parentItem()->parentItem(), tabGroup2);
}

void tst_tabgroup::testAddRemoveTabs()
{
    const QString componentData(
        "import QtQuick 1.0\n"
        "import com.nokia.symbian 1.0\n"
        "Page {\n"
        "}\n");

    // test reparenting works for dynamic addition/removal

    QDeclarativeItem *tabGroup2 = componentObject->findChild<QDeclarativeItem*>("tabGroup2");
    QDeclarativeItem *containerHost = tabGroup2->findChild<QDeclarativeItem*>("containerHost");
    QVERIFY(containerHost);

    QDeclarativeItem *group2tab1 = qobject_cast<QDeclarativeItem*>(findQmlChild(containerHost,"group2tab1"));
    QVERIFY(group2tab1);

    QDeclarativeItem *group2tab2 = qobject_cast<QDeclarativeItem*>(findQmlChild(containerHost,"group2tab2"));
    QVERIFY(group2tab2);

    QCOMPARE(containerHost->childItems().count(), 2);

    // create new tab
    QString errors;
    QDeclarativeItem *newTabContent = qobject_cast<QDeclarativeItem*>(tst_quickcomponentstest::createComponentFromString(componentData, &errors));
    QVERIFY2(newTabContent, qPrintable(errors));

    // stick it into the tab group
    newTabContent->setParentItem(tabGroup2);

    QVERIFY(newTabContent->parentItem());
    QCOMPARE(newTabContent->parentItem()->parentItem(), containerHost);
    QCOMPARE(containerHost->childItems().count(), 3);

    // delete first tab
    delete group2tab1;

    // qml_object.destroy() is called which deletes the istance via event loop
    // let's complete the deletion.
    QCoreApplication::processEvents(QEventLoop::DeferredDeletion);
    QCOMPARE(containerHost->childItems().count(), 2);

    // delete new first (original second) tab
    delete group2tab2;
    QCoreApplication::processEvents(QEventLoop::DeferredDeletion);
    QCOMPARE(containerHost->childItems().count(), 1);
}

void tst_tabgroup::testPageSignaling()
{
    // test page content gets proper signals

    QDeclarativeItem *tabGroup2 = componentObject->findChild<QDeclarativeItem*>("tabGroup2");
    QVERIFY(tabGroup2);

    // tabs are Page instances
    QDeclarativeItem *group2tab1 = qobject_cast<QDeclarativeItem*>(findQmlChild(tabGroup2,"group2tab1"));
    QVERIFY(group2tab1);

    QDeclarativeItem *group2tab2 = qobject_cast<QDeclarativeItem*>(findQmlChild(tabGroup2,"group2tab2"));
    QVERIFY(group2tab2);

    // set group2tab1 as the current tab
    tabGroup2->setProperty("currentTab", qVariantFromValue(group2tab1));
    QTRY_COMPARE(group2tab1->property("status").toInt(), 2 /*PageStatus.Active*/);
    QTRY_COMPARE(group2tab2->property("status").toInt(), 0 /*PageStatus.Inactive*/);

    // set group2tab2 as the current tab
    tabGroup2->setProperty("currentTab", qVariantFromValue(group2tab2));

    QCOMPARE(group2tab2->property("status").toInt(), 1 /*PageStatus.Activating*/);
    QCOMPARE(group2tab1->property("status").toInt(), 3 /*PageStatus.Deactivating*/);

    // wait for the effec to finish and update the statuses
    QTRY_COMPARE(group2tab2->property("status").toInt(), 2 /*PageStatus.Active*/);
    QTRY_COMPARE(group2tab1->property("status").toInt(), 0 /*PageStatus.Inactive*/);
}


QTEST_MAIN(tst_tabgroup)
#include "tst_tabgroup.moc"
