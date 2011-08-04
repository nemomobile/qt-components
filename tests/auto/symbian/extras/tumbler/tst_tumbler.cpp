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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QStringList>

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
    QDeclarativeItem *tumblerColumn = componentObject->findChild<QDeclarativeItem*>("aColumn");
    QVERIFY(tumblerColumn);
    QCOMPARE(tumblerColumn->property("itemCount").toInt(), 5);

    tumblerColumn = componentObject->findChild<QDeclarativeItem*>("bColumn");
    QVERIFY(tumblerColumn);
    QCOMPARE(tumblerColumn->property("itemCount").toInt(), 5);

    // try setting c++ model in "items"
    QStringList dataList;
    dataList.append("Item 1");
    dataList.append("Item 2");
    dataList.append("Item 3");
    tumblerColumn->setProperty("items", QVariant::fromValue(dataList));
    QCOMPARE(tumblerColumn->property("itemCount").toInt(), 3);
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
