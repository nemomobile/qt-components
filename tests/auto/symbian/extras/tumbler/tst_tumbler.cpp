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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/private/qdeclarativelistmodel_p.h>
#include <QDeclarativeView>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_tumbler : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void basic();
    void selected();
    void property_enabled();
    void property_items();
    void signal_changed();

private:
    QObject *componentObject;
};

void tst_tumbler::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_tumbler.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_tumbler::basic()
{
    QObject *child;
    const QObjectList children = componentObject->children();
    // verify there is 4 children
    QCOMPARE(children.size(), 4);
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("tb")) {
            // verify aColumn's selectedIndex has default value of 0
            QVERIFY2(child->property("privateDelayInit").isValid(), "Error, TumblerColumn's child must have selectedIndex property");
            QVERIFY2(child->property("columns").isValid(), "Error, TumblerColumn's child must have selectedIndex property");
        }
    }
}

void tst_tumbler::selected()
{
    QObject *child;
    const QObjectList children = componentObject->children();
    // verify there is 4 children
    QCOMPARE(children.size(), 4);
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("aColumn")) {
            // verify aColumn's selectedIndex has default value of 0
            QVERIFY2(child->property("selectedIndex").isValid(), "Error, TumblerColumn's child must have selectedIndex property");
            QCOMPARE(child->property("selectedIndex").toInt(), 0);
            child->setProperty("selectedIndex", 2);
            QCOMPARE(child->property("selectedIndex").toInt(), 2);
        }
        if (child->objectName().contains("bColumn")) {
            // verify bColumn's selectedIndex has default value of 0
            QVERIFY2(child->property("selectedIndex").isValid(), "Error, TumblerColumn's child must have selectedIndex property");
            QCOMPARE(child->property("selectedIndex").toInt(), 0);
            child->setProperty("selectedIndex", 4);
            QCOMPARE(child->property("selectedIndex").toInt(), 4);
        }
    }
}

void tst_tumbler::property_enabled()
{
    QObject *child;
    const QObjectList children = componentObject->children();
    // verify there is 4 children
    QCOMPARE(children.size(), 4);
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("aColumn")) {
            QVERIFY2(child->property("enabled").isValid(), "Error, TumblerColumn's child must have enabled property");
            // verify aColumn's enabled has default value true
            QCOMPARE(child->property("enabled").toBool(), true);
            child->setProperty("enabled", false);
            QCOMPARE(child->property("enabled").toBool(), false);
        }
        if (child->objectName().contains("bColumn")) {
            // verify bColumn's enabled has default value false
            QVERIFY2(child->property("enabled").isValid(), "Error, TumblerColumn's child must have enabled property");
            QCOMPARE(child->property("enabled").toBool(), false);
            child->setProperty("enabled", true);
            QCOMPARE(child->property("enabled").toBool(), true);
        }
    }
}

void tst_tumbler::property_items()
{
    QObject *child;
    const QObjectList children = componentObject->children();
    // verify there is 4 children
    QCOMPARE(children.size(), 4);
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        // verify ListModel
        if (child->objectName().contains("model")) {
            // verify there is 5 elements
            QCOMPARE(child->property("count").toInt(), 5);
        }
        // TODO: Add test cases for TumblerColumns
    }
}

void tst_tumbler::signal_changed()
{
    QObject *child;
    const QObjectList children = componentObject->children();
    // verify there is 4 children
    QCOMPARE(children.size(), 4);
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("tb")) {
            QSignalSpy spy(child, SIGNAL(changed(int)));
            QCOMPARE(spy.count(),0);
            QMetaObject::invokeMethod(child, "changed", Qt::DirectConnection, Q_ARG(int,2));
            QCOMPARE(spy.count(),1);
            QVERIFY(spy.isValid());
        }
    }
}

QTEST_MAIN(tst_tumbler)

#include "tst_tumbler.moc"
