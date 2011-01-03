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
#include <QDeclarativeExpression>
#include <QRegExpValidator>

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

    // ### missing function tests

private:
    QObject *componentObject;
    QDeclarativeEngine *engine;
};

void tst_quickcomponentstextfield::initTestCase()
{
    QString errors;
    engine = 0;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextfield.qml", &errors, &engine);
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

    QEXPECT_FAIL("", "Not yet guarded by input hint, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-317", Continue);
    QCOMPARE( componentObject->property("text").toString(), QString("1234") );
}

void tst_quickcomponentstextfield::font()
{
    QVERIFY( componentObject->setProperty("font", "Helvetica") );
    QCOMPARE( componentObject->property("font").toString(), QString("Helvetica") );
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
    QEXPECT_FAIL("", "Not yet guarded by readOnly property, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-318", Continue);
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
    componentObject->setProperty("text", "Good morning");
    QDeclarativeExpression *expr = new QDeclarativeExpression(engine->rootContext(), componentObject, "select(0,4);");
    expr->evaluate();
    if (expr->hasError())
        qDebug() << expr->error();
    QVERIFY( !expr->hasError() );
    QEXPECT_FAIL("", "Not yet able to verify text selection, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-322", Continue);
    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentstextfield::selectionStart()
{
    componentObject->setProperty("text", "Good morning");
    QDeclarativeExpression *expr = new QDeclarativeExpression(engine->rootContext(), componentObject, "select(5,11);");
    expr->evaluate();
    if (expr->hasError())
        qDebug() << expr->error();
    QVERIFY( !expr->hasError() );
    QEXPECT_FAIL("", "Not yet able to verify text selection, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-322", Continue);
    QCOMPARE( componentObject->property("selectionStart").toInt(), 5 );
}

void tst_quickcomponentstextfield::selectionEnd()
{
    componentObject->setProperty("text", "Good morning");
    QDeclarativeExpression *expr = new QDeclarativeExpression(engine->rootContext(), componentObject, "select(5,11);");
    expr->evaluate();
    if (expr->hasError())
        qDebug() << expr->error();
    QVERIFY( !expr->hasError() );
    QEXPECT_FAIL("", "Not yet able to verify text selection, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-322", Continue);
    QCOMPARE( componentObject->property("selectionEnd").toInt(), 11 );
}

void tst_quickcomponentstextfield::text()
{
    QVERIFY( componentObject->setProperty("text", "Hello World") );
    QCOMPARE( componentObject->property("text").toString(), QString("Hello World") );
}

void tst_quickcomponentstextfield::acceptableInput()
{
    QRegExpValidator* validator = new QRegExpValidator(QRegExp("-?\\d{1,3}"),this);
    componentObject->setProperty("validator",QVariant::fromValue(validator));

    // depending on validator, acceptable input should be true / false
    // set text that is allowed by the validator
    QVERIFY( componentObject->setProperty("text", "-42") );
    QCOMPARE( componentObject->property("acceptableInput").toBool(), true);

    QVERIFY( componentObject->setProperty("text", "128") );
    QCOMPARE( componentObject->property("acceptableInput").toBool(), true);

    // make sure setting the text fails if validator fails
    QVERIFY( componentObject->setProperty("text", "qwerasdfzxvc") );
    QEXPECT_FAIL("", "Not yet able to verify validator failure, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-323", Continue);
    QCOMPARE( componentObject->property("acceptableInput").toBool(), false);
}

QTEST_MAIN(tst_quickcomponentstextfield)

QML_DECLARE_TYPE(QRegExpValidator)

#include "tst_quickcomponentstextfield.moc"
