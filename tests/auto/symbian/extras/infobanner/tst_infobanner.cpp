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
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>

#include "tst_quickcomponentstest.h"

class tst_infobanner : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void iconSource();
    void text();    
    void timeout();
    void interactive();
    void clicked();

private:
    QObject *componentObject;
};

void tst_infobanner::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_infobanner.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_infobanner::iconSource()
{
    QCOMPARE(QFileInfo(componentObject->property("iconSource").toString()).fileName(),QString("logo.png"));
}

void tst_infobanner::text()
{
    QCOMPARE(componentObject->property("text").toString(),QString("Testing info banner"));
    componentObject->setProperty("text","Info banner Tested!");
    QCOMPARE(componentObject->property("text").toString(),QString("Info banner Tested!"));
}

void tst_infobanner::timeout()
{
    QVERIFY(componentObject->setProperty("timeout", 3000));
    QCOMPARE(componentObject->property("timeout").toReal(), 3000.0);
}

void tst_infobanner::interactive()
{
    QVERIFY(componentObject->setProperty("interactive", true));
    QCOMPARE(componentObject->property("interactive").toBool(), true);
}

void tst_infobanner::clicked()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QMetaObject::invokeMethod(componentObject,"clicked",Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

QTEST_MAIN(tst_infobanner)

#include "tst_infobanner.moc"
