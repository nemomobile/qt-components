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

#include <QTest>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDebug>
#include <QSignalSpy>
#include "tst_quickcomponentstest.h"

class tst_menuitem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void defaultPropertyValues();
    void properties();

private:
    QObject *componentObject;
};

void tst_menuitem::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_menuitem.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_menuitem::cleanupTestCase()
{
}

void tst_menuitem::init()
{
}

void tst_menuitem::cleanup()
{
}

void tst_menuitem::defaultPropertyValues()
{
    QVERIFY(componentObject->property("platformInverted").isValid());
    QCOMPARE(componentObject->property("platformInverted").toBool(), false);
}


void tst_menuitem::properties()
{
    QVERIFY(componentObject->setProperty("platformInverted", true));
    QCOMPARE(componentObject->property("platformInverted").toBool(), true);

    QVERIFY(componentObject->property("platformLeftMargin").isValid());
    QVERIFY(componentObject->setProperty("platformLeftMargin", 10));
    QCOMPARE(componentObject->property("platformLeftMargin").toInt(), 10);
}

QTEST_MAIN(tst_menuitem)
#include "tst_menuitem.moc"
