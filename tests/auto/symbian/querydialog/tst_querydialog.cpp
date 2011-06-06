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

class tst_querydialog : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void titleText();
    void message();
    void acceptButtonText();
    void rejectButtonText();
    void icon();

private:
    QObject *componentObject;
};

void tst_querydialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_querydialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_querydialog::titleText()
{
    QCOMPARE(componentObject->property("titleText").toString(),QString("Testing QueryDialog"));
    componentObject->setProperty("titleText","QueryDialog Tested!");
    QCOMPARE(componentObject->property("titleText").toString(),QString("QueryDialog Tested!"));
}

void tst_querydialog::message()
{
    QVERIFY(componentObject->setProperty("message", "This is a test"));
    QCOMPARE(componentObject->property("message").toString(),QString("This is a test"));
}

void tst_querydialog::acceptButtonText()
{
    QVERIFY(componentObject->setProperty("acceptButtonText", "OK"));
    QCOMPARE(componentObject->property("acceptButtonText").toString(),QString("OK"));
}

void tst_querydialog::rejectButtonText()
{
    QVERIFY(componentObject->setProperty("rejectButtonText", "Cancel"));
    QCOMPARE(componentObject->property("rejectButtonText").toString(),QString("Cancel"));
}

void tst_querydialog::icon()
{
    QCOMPARE(QFileInfo(componentObject->property("icon").toString()).fileName(),QString("logo.png"));
}

QTEST_MAIN(tst_querydialog)

#include "tst_querydialog.moc"
