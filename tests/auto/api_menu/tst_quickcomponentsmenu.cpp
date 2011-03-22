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

class tst_quickcomponentsmenu : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void content();
    void visualParent();
    void status();

private:
    QObject *componentObject;
};

void tst_quickcomponentsmenu::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsmenu.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsmenu::content()
{
    QGraphicsObject *menu = componentObject->findChild<QGraphicsObject*>("menu");
    QGraphicsObject *contextmenu = componentObject->findChild<QGraphicsObject*>("contextmenu");

    qRegisterMetaType<QDeclarativeListProperty<QGraphicsObject> > ("QDeclarativeListProperty<QGraphicsObject>");
    
    QVERIFY(menu->property("content").isValid());
    QVERIFY(contextmenu->property("content").isValid());
    
    QDeclarativeListProperty<QGraphicsObject> list;
    list = qvariant_cast<QDeclarativeListProperty<QGraphicsObject> >(menu->property("content"));
    QCOMPARE(static_cast<QList<QGraphicsObject*> *>(list.data)->count(), 6);

    list = qvariant_cast<QDeclarativeListProperty<QGraphicsObject> >(contextmenu->property("content"));
    QCOMPARE(static_cast<QList<QGraphicsObject*> *>(list.data)->count(), 6);
 }

void tst_quickcomponentsmenu::visualParent()
{
    QGraphicsObject *menu = componentObject->findChild<QGraphicsObject*>("menu");
    QGraphicsObject *contextmenu = componentObject->findChild<QGraphicsObject*>("contextmenu");

    QVERIFY(menu->property("visualParent").isValid());
    QCOMPARE(menu->property("visualParent").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);

    QVERIFY(contextmenu->property("visualParent").isValid());
    QCOMPARE(contextmenu->property("visualParent").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsmenu::status()
{
    QGraphicsObject *menu = componentObject->findChild<QGraphicsObject*>("menu");
    QGraphicsObject *contextmenu = componentObject->findChild<QGraphicsObject*>("contextmenu");

    QVERIFY(menu->property("status").isValid());
    QCOMPARE(menu->property("status").toInt(), 3 /*Closed*/);

    QVERIFY(QMetaObject::invokeMethod(menu, "open"));
    QCOMPARE(menu->property("status").toInt(), 0 /*Opening*/);

    QVERIFY(QMetaObject::invokeMethod(menu, "close"));
    QCOMPARE(menu->property("status").toInt(), 2 /*Closing*/);

    QVERIFY(contextmenu->property("status").isValid());
    QCOMPARE(contextmenu->property("status").toInt(), 3 /*Closed*/);

    QVERIFY(QMetaObject::invokeMethod(contextmenu, "open"));
    QCOMPARE(contextmenu->property("status").toInt(), 0 /*Opening*/);

    QVERIFY(QMetaObject::invokeMethod(contextmenu, "close"));
    QCOMPARE(contextmenu->property("status").toInt(), 2 /*Closing*/);
}

QTEST_MAIN(tst_quickcomponentsmenu)

#include "tst_quickcomponentsmenu.moc"
