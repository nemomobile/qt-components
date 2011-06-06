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
