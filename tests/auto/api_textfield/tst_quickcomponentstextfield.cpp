/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <QLineEdit>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstextfield : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void placeholderText();
    void inputMethodHints();
    void font();
    void cursorPosition();
    void readOnly();
    void echoMode();
    void inputMask();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void acceptableInput();
    void validator();

    // ### missing function tests

private:
    QObject *componentObject;
};

void tst_quickcomponentstextfield::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextfield.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentstextfield::placeholderText()
{
    QVERIFY( componentObject->setProperty("placeholderText", "Some text") );
    QCOMPARE( componentObject->property("placeholderText").toString(), QString("Some text") );
}

void tst_quickcomponentstextfield::inputMethodHints()
{
    QVERIFY( componentObject->setProperty("inputMethodHints", Qt::ImhPreferNumbers) );
    QVERIFY( componentObject->setProperty("text", "1234") );
    QCOMPARE( componentObject->property("placeholderText").toString(), QString("Some text") );
    QVERIFY( componentObject->setProperty("text", "Some more text" ) );
    QCOMPARE( componentObject->property("text").toString(), QString("1234") );
}

void tst_quickcomponentstextfield::font()
{
    QVERIFY( componentObject->setProperty("font.family", "Helvetica") );
    QCOMPARE( componentObject->property("font.family").toString(), QString("Helvetica") );
}

void tst_quickcomponentstextfield::cursorPosition()
{
    QVERIFY( componentObject->setProperty("cursorPosition", 0) );
    QCOMPARE( componentObject->property("cursorPosition").toInt(), 0 );
}

void tst_quickcomponentstextfield::readOnly()
{
    QVERIFY( componentObject->setProperty("readOnly", true) );
    QVERIFY( componentObject->setProperty("text", "I just changed the text") );
    QVERIFY( componentObject->property("text").toString() != QString("I just changed the text"));
}

void tst_quickcomponentstextfield::echoMode()
{
    // ### set correct enum type
    QVERIFY( componentObject->setProperty("echoMode", 2) );
    QCOMPARE( componentObject->property("echoMode").toInt(), 2 );
}

void tst_quickcomponentstextfield::inputMask()
{
    QVERIFY( componentObject->setProperty("inputMask", 2) );

}

void tst_quickcomponentstextfield::selectedText()
{
    QVERIFY( componentObject->setProperty("text", "Good morning") );
    QVERIFY( componentObject->setProperty("selectionStart", 0) );
    QVERIFY( componentObject->setProperty("selectionEnd", 4) );

    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentstextfield::selectionEnd()
{
    // ### needs to set selection
    QVERIFY( componentObject->setProperty("selectionEnd", 2) );

}

void tst_quickcomponentstextfield::selectionStart()
{
    // ### needs to set selection
    QVERIFY( componentObject->setProperty("selectionStart", 1) );
}

void tst_quickcomponentstextfield::text()
{
    QVERIFY( componentObject->setProperty("text", "Hello World") );
    QCOMPARE( componentObject->property("text").toString(), QString("Hello World") );
}

void tst_quickcomponentstextfield::acceptableInput()
{
    // depending on validator, acceptable input should be true / false
    QVERIFY( componentObject->setProperty("validator", "-?\\d{1,3}") );

    // set text that is allowed by the validator
    QVERIFY( componentObject->setProperty("text", "42") );
    QCOMPARE( componentObject->property("text").toInt(), 42);
    QCOMPARE( componentObject->property("acceptableInput").toBool(), true);

    // make sure setting the text fails if validator fails
    QVERIFY( componentObject->setProperty("text", "2800}") );
    QCOMPARE( componentObject->property("acceptableInput").toBool(), false);
}

void tst_quickcomponentstextfield::validator()
{
    QVERIFY( componentObject->setProperty("validator", "-?\\d{1,3}") );

}

QTEST_MAIN(tst_quickcomponentstextfield)

#include "tst_quickcomponentstextfield.moc"
