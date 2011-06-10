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
