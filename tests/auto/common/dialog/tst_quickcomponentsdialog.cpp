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

class tst_quickcomponentsdialog : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void title();
    void buttons();
    void content();
    void visualParent();
    void status();
    void accepted();
    void rejected();
    void clickedOutside();

private:
    QObject *componentObject;
};

void tst_quickcomponentsdialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsdialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsdialog::title()
{
    QVERIFY(componentObject->property("title").isValid());
    QCOMPARE(componentObject->property("title").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsdialog::content()
{
    QVERIFY(componentObject->property("content").isValid());
    QCOMPARE(componentObject->property("content").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsdialog::buttons()
{
    QVERIFY(componentObject->property("buttons").isValid());
    QCOMPARE(componentObject->property("buttons").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsdialog::visualParent()
{
    QVERIFY(componentObject->property("visualParent").isValid());
    QCOMPARE(componentObject->property("visualParent").value<QDeclarativeItem*>(), (QDeclarativeItem*)0);
}

void tst_quickcomponentsdialog::status()
{
    QVERIFY(componentObject->property("status").isValid());
    QCOMPARE(componentObject->property("status").toInt(), 3 /*DialogStatus.Closed*/);

    QVERIFY(QMetaObject::invokeMethod(componentObject, "open"));
    QCOMPARE(componentObject->property("status").toInt(), 0 /*DialogStatus.Opening*/);

    QVERIFY(QMetaObject::invokeMethod(componentObject, "close"));
    QCOMPARE(componentObject->property("status").toInt(), 2 /*DialogStatus.Closing*/);
}

void tst_quickcomponentsdialog::accepted()
{
    QVERIFY(QMetaObject::invokeMethod(componentObject, "open"));
    QTRY_COMPARE(componentObject->property("status").toInt(), 1);

    QSignalSpy acceptedSpy(componentObject, SIGNAL(accepted()));
    QVERIFY(acceptedSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(componentObject, "accept"));
    QCOMPARE(acceptedSpy.count(), 1);
}

void tst_quickcomponentsdialog::rejected()
{
    QVERIFY(QMetaObject::invokeMethod(componentObject, "open"));
    QTRY_COMPARE(componentObject->property("status").toInt(), 1);

    QSignalSpy rejectedSpy(componentObject, SIGNAL(rejected()));
    QVERIFY(rejectedSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(componentObject, "reject"));
    QCOMPARE(rejectedSpy.count(), 1);
}

void tst_quickcomponentsdialog::clickedOutside()
{
    QSignalSpy clickedOutsideSpy(componentObject, SIGNAL(clickedOutside()));
    QVERIFY(clickedOutsideSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(componentObject, "clickedOutside"));
    QCOMPARE(clickedOutsideSpy.count(), 1);
}

QTEST_MAIN(tst_quickcomponentsdialog)

#include "tst_quickcomponentsdialog.moc"
