/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

    // Functions
    void copyAndPaste();
    void cutAndPaste();
    void selectAll();
    void selectWord();

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
    QFont font;
    font.setFamily("Helvetica");
    font.setPixelSize(12);

    QFont temp;
    QVERIFY( componentObject->setProperty("font", font) );
    temp = componentObject->property("font").value<QFont>();
    QCOMPARE( temp, font );
}

void tst_quickcomponentstextfield::cursorPosition()
{
    QVERIFY(componentObject->setProperty("text", "Good morning"));
    QVERIFY( componentObject->setProperty("cursorPosition", 0) );
    QCOMPARE( componentObject->property("cursorPosition").toInt(), 0 );

    // test other values, different from 0
    QVERIFY( componentObject->setProperty("cursorPosition", 4) );
    QCOMPARE( componentObject->property("cursorPosition").toInt(), 4 );
}

void tst_quickcomponentstextfield::readOnly()
{
    QVERIFY( componentObject->setProperty("readOnly", true) );
    QVERIFY( componentObject->setProperty("text", "I just changed the text") );
    QEXPECT_FAIL("", "Not yet guarded by readOnly property, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-318", Continue);
    QVERIFY( componentObject->property("text").toString() != QString("I just changed the text"));
    // reset readOnly, in order to not mess around with the other tests
    QVERIFY( componentObject->setProperty("readOnly", false) );
}

void tst_quickcomponentstextfield::echoMode()
{
    // ### set correct enum type
    QVERIFY( componentObject->setProperty("echoMode", 2) );
    QCOMPARE( componentObject->property("echoMode").toInt(), 2 );
    // reset echoMode, in order to not mess around with the other tests
    QVERIFY( componentObject->setProperty("echoMode", 0) );
}

void tst_quickcomponentstextfield::inputMask()
{
    QVERIFY(componentObject->setProperty("inputMask", QString("000.000;_")));
    QVERIFY(componentObject->setProperty("text", "A1.2B"));
    QCOMPARE(componentObject->property("text").toString(), QString("1.2"));

    QVERIFY(componentObject->setProperty("text", "12345"));
    QCOMPARE(componentObject->property("text").toString(), QString("123.45"));

    QVERIFY(componentObject->setProperty("text", "123456"));
    QCOMPARE(componentObject->property("text").toString(), QString("123.456"));

    QVERIFY(componentObject->setProperty("text", "1234567"));
    QCOMPARE(componentObject->property("text").toString(), QString("123.456"));

    QVERIFY(componentObject->setProperty("text", "....."));
    QCOMPARE(componentObject->property("text").toString(), QString("."));

    QVERIFY(componentObject->setProperty("inputMask", QString("D0.AA.XX.AA.00;_")));
    QVERIFY(componentObject->setProperty("text", QString("0!P3")));
    QCOMPARE(componentObject->property("text").toString(), QString("0..!P..3"));

    QVERIFY(componentObject->setProperty("text", "Sometext"));
    QCOMPARE(componentObject->property("text").toString(), QString(".So.me.te."));

    QVERIFY(componentObject->setProperty("inputMask", ">AAAA"));
    QVERIFY(componentObject->setProperty("text", QString("abcd")));
    QCOMPARE(componentObject->property("text").toString(), QString("ABCD"));

    QVERIFY(componentObject->setProperty("text", QString("12Ab")));
    QCOMPARE(componentObject->property("text").toString(), QString("AB"));

    QVERIFY(componentObject->setProperty("inputMask", QString("#9999")));
    QVERIFY(componentObject->setProperty("text", "Sometext"));
    QCOMPARE(componentObject->property("text").toString(), QString(""));

    QVERIFY(componentObject->setProperty("text", "4564"));
    QCOMPARE(componentObject->property("text").toString(), QString("4564"));

    QVERIFY(componentObject->setProperty("text", "-4564"));
    QCOMPARE(componentObject->property("text").toString(), QString("-4564"));

    // reset mask, in order to not mess around with the other tests
    QVERIFY(componentObject->setProperty("inputMask", ""));
}

void tst_quickcomponentstextfield::selectedText()
{
    componentObject->setProperty("text", "Good morning");

    int methodIndex = componentObject->metaObject()->indexOfMethod("select(QVariant,QVariant)");
    QMetaMethod method = componentObject->metaObject()->method(methodIndex);
    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 0), Q_ARG(QVariant, 6)));
    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good m") );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 0), Q_ARG(QVariant, 4)));
    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good") );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 0), Q_ARG(QVariant, 12)));
    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good morning") );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 5), Q_ARG(QVariant, 12)));
    QCOMPARE( componentObject->property("selectedText").toString(), QString("morning") );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 5), Q_ARG(QVariant, 10)));
    QCOMPARE( componentObject->property("selectedText").toString(), QString("morni") );
}

void tst_quickcomponentstextfield::selectionStart()
{
    componentObject->setProperty("text", "Good morning");

    int methodIndex = componentObject->metaObject()->indexOfMethod("select(QVariant,QVariant)");
    QMetaMethod method = componentObject->metaObject()->method(methodIndex);
    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 0), Q_ARG(QVariant, 6)));
    QCOMPARE( componentObject->property("selectionStart").toInt(), 0 );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 3), Q_ARG(QVariant, 6)));
    QCOMPARE( componentObject->property("selectionStart").toInt(), 3 );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 5), Q_ARG(QVariant, 6)));
    QCOMPARE( componentObject->property("selectionStart").toInt(), 5 );
}

void tst_quickcomponentstextfield::selectionEnd()
{
    componentObject->setProperty("text", "Good morning");

    int methodIndex = componentObject->metaObject()->indexOfMethod("select(QVariant,QVariant)");
    QMetaMethod method = componentObject->metaObject()->method(methodIndex);
    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 0), Q_ARG(QVariant, 6)));
    QCOMPARE( componentObject->property("selectionEnd").toInt(), 6 );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 3), Q_ARG(QVariant, 4)));
    QCOMPARE( componentObject->property("selectionEnd").toInt(), 4 );

    // when selection range is higher than limit, it stays unchanged
    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 3), Q_ARG(QVariant, 100)));
    QCOMPARE( componentObject->property("selectionEnd").toInt(), 4 );

    QVERIFY(method.invoke(componentObject, Qt::DirectConnection, Q_ARG(QVariant, 5), Q_ARG(QVariant, 12)));
    QCOMPARE( componentObject->property("selectionEnd").toInt(), 12 );
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

void tst_quickcomponentstextfield::copyAndPaste()
{
    QVERIFY(componentObject->setProperty("text", "Good morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 6)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "copy"));
    QVERIFY(componentObject->setProperty("cursorPosition", 0));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good mGood morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 4)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "copy"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 18)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good"));

    componentObject->setProperty("text", "Good morning");

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 12)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "copy"));
    QVERIFY(componentObject->setProperty("cursorPosition", 12));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morningmorning"));
}

void tst_quickcomponentstextfield::cutAndPaste()
{
    componentObject->setProperty("text", "Good morning");

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 6)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);
    QCOMPARE(componentObject->property("text").toString(), QString("orning"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 12)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 5);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 5);
    QVERIFY(componentObject->setProperty("cursorPosition", 0));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("morningGood "));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 7)));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);
    QVERIFY(componentObject->setProperty("cursorPosition", 5));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));
}

void tst_quickcomponentstextfield::selectAll()
{
    componentObject->setProperty("text", "Good morning");

    QVERIFY(QMetaObject::invokeMethod(componentObject, "selectAll"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);
    QCOMPARE(componentObject->property("text").toString(), QString(""));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "selectAll"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "copy"));
    QVERIFY(componentObject->setProperty("cursorPosition", 12));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morningGood morning"));
}

void tst_quickcomponentstextfield::selectWord()
{
    componentObject->setProperty("text", "Good morning");

    QVERIFY(componentObject->setProperty("cursorPosition", 0));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "selectWord"));
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));

    QVERIFY(componentObject->setProperty("cursorPosition", 2));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "selectWord"));
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good"));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));


    QVERIFY(componentObject->setProperty("cursorPosition", 5));
    QVERIFY(QMetaObject::invokeMethod(componentObject, "selectWord"));

    QEXPECT_FAIL("", "Selected word should be morning", Continue);
    QCOMPARE(componentObject->property("selectedText").toString(), QString("morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
    QEXPECT_FAIL("", "'morning' was cut, so cursorPosition should be at 5", Continue);
    QCOMPARE(componentObject->property("selectionStart").toInt(), 5);
    QEXPECT_FAIL("", "'morning' was cut, so cursorPosition should be at 5", Continue);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 5);
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));
}

QTEST_MAIN(tst_quickcomponentstextfield)

QML_DECLARE_TYPE(QRegExpValidator)

#include "tst_quickcomponentstextfield.moc"
