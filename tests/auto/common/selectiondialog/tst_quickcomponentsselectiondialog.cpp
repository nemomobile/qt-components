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

#include <QtTest>
#include <QSignalSpy>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsselectiondialog : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
// These cases are testing API inherited from Dialog component
    void title();
    void buttons();
    void content();
    void visualParent();
    void status();
    void accepted();
    void rejected();
// Test cases for SelectionDialog API
    void selectedIndex();
    void delegate();
    void titleText();

private:
    QObject *componentObject;
};

void tst_quickcomponentsselectiondialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsselectiondialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsselectiondialog::title()
{
    QVERIFY(componentObject->property("title").isValid());
    QCOMPARE(componentObject->property("title").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsselectiondialog::content()
{
    QVERIFY(componentObject->property("content").isValid());
    QCOMPARE(componentObject->property("content").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsselectiondialog::buttons()
{
    QVERIFY(componentObject->property("buttons").isValid());
    QCOMPARE(componentObject->property("buttons").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsselectiondialog::visualParent()
{
    QVERIFY(componentObject->property("visualParent").isValid());
    QCOMPARE(componentObject->property("visualParent").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsselectiondialog::status()
{
    QVERIFY(componentObject->property("status").isValid());
    QCOMPARE(componentObject->property("status").toInt(), 3 /*DialogStatus.Closed*/);

    QVERIFY(QMetaObject::invokeMethod(componentObject, "open"));
    QCOMPARE(componentObject->property("status").toInt(), 0 /*DialogStatus.Opening*/);

    QVERIFY(QMetaObject::invokeMethod(componentObject, "close"));
    QCOMPARE(componentObject->property("status").toInt(), 2 /*DialogStatus.Closing*/);
}

void tst_quickcomponentsselectiondialog::accepted()
{
    QSignalSpy acceptedSpy(componentObject, SIGNAL(accepted()));
    QVERIFY(acceptedSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(componentObject, "accept"));
    QCOMPARE(acceptedSpy.count(), 1);
}

void tst_quickcomponentsselectiondialog::rejected()
{
    QSignalSpy rejectedSpy(componentObject, SIGNAL(rejected()));
    QVERIFY(rejectedSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(componentObject, "reject"));
    QCOMPARE(rejectedSpy.count(), 1);
}

void tst_quickcomponentsselectiondialog::selectedIndex()
{
    QVERIFY(componentObject->property("selectedIndex").isValid());
    QCOMPARE(componentObject->property("selectedIndex").toInt(), -1);
}

void tst_quickcomponentsselectiondialog::delegate()
{
    QVERIFY(componentObject->property("delegate").isValid());
}

void tst_quickcomponentsselectiondialog::titleText()
{
    QVERIFY(componentObject->property("titleText").isValid());
    QCOMPARE(componentObject->property("titleText").toString(),QString("Testing"));
    componentObject->setProperty("titleText","Tested!");
    QCOMPARE(componentObject->property("titleText").toString(),QString("Tested!"));
}

QTEST_MAIN(tst_quickcomponentsselectiondialog)

#include "tst_quickcomponentsselectiondialog.moc"
