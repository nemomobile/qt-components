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
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <qdeclarativewindow.h>
#include <QLineEdit>

class tst_quickcomponentslineedit : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentslineedit();

private slots:
    void placeholderText();
    void inputMethodHint();
    void font();
    void cursorPosition();
    void readOnly();
    void displayText();
    void passwordMode();
    void horizontalAlignment();
    void inputMask();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void acceptableInput();
    void validator();
    void maximumLength();
    void selectionPolicy();

private:
    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};

tst_quickcomponentslineedit::tst_quickcomponentslineedit()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    engine = window->engine();
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());

    QFile file("tst_quickcomponentslineedit.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    QVERIFY( (componentObject = component->create()) != 0);
}

void tst_quickcomponentslineedit::placeholderText()
{
    QVERIFY( componentObject->setProperty("placeholderText", "Some text") );
    QCOMPARE( componentObject->property("placeholderText").toString(), QString("Some text") );
}

void tst_quickcomponentslineedit::inputMethodHint()
{
    QVERIFY( componentObject->setProperty("inputMethodHint", Qt::ImhPreferNumbers) );
    QVERIFY( componentObject->setProperty("text", "1234") );
    QCOMPARE( componentObject->property("placeholderText").toString(), QString("Some text") );
    QVERIFY( componentObject->setProperty("text", "Some more text" ) );
    QCOMPARE( componentObject->property("text").toString(), QString("1234") );
}

void tst_quickcomponentslineedit::font()
{
    QVERIFY( componentObject->setProperty("font.family", "Helvetica") );
    QCOMPARE( componentObject->property("font.family").toString(), QString("Helvetica") );
}

void tst_quickcomponentslineedit::cursorPosition()
{
    QVERIFY( componentObject->setProperty("cursorPosition", 0) );
    QCOMPARE( componentObject->property("cursorPosition").toInt(), 0 );
}

void tst_quickcomponentslineedit::readOnly()
{
    QVERIFY( componentObject->setProperty("readOnly", true) );
    QVERIFY( componentObject->setProperty("text", "I just changed the text") );
    QVERIFY( componentObject->property("text").toString() != QString("I just changed the text"));
}

void tst_quickcomponentslineedit::displayText()
{
    QVERIFY( componentObject->setProperty("displayText", "Display Hint") );
    QCOMPARE( componentObject->property("displayText").toString(), QString("Display Hint") );
}

void tst_quickcomponentslineedit::passwordMode()
{
    QVERIFY( componentObject->setProperty("passwordMode", 2) );
    QCOMPARE( componentObject->property("passwordMode").toInt(), 2 );
}

void tst_quickcomponentslineedit::horizontalAlignment()
{
    QVERIFY( componentObject->setProperty("horizontalAlignment", 2) );

}

void tst_quickcomponentslineedit::inputMask()
{
    QVERIFY( componentObject->setProperty("inputMask", 2) );

}

void tst_quickcomponentslineedit::selectedText()
{
    QVERIFY( componentObject->setProperty("text", "Good morning") );
    QVERIFY( componentObject->setProperty("selectionStart", 0) );
    QVERIFY( componentObject->setProperty("selectionEnd", 4) );

    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentslineedit::selectionEnd()
{
    QVERIFY( componentObject->setProperty("selectionEnd", 2) );

}

void tst_quickcomponentslineedit::selectionStart()
{
    QVERIFY( componentObject->setProperty("selectionStart", 1) );
}

void tst_quickcomponentslineedit::text()
{
    QVERIFY( componentObject->setProperty("text", "Hello World") );
    QCOMPARE( componentObject->property("text").toString(), QString("Hello World") );
}

void tst_quickcomponentslineedit::acceptableInput()
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

void tst_quickcomponentslineedit::validator()
{
    QVERIFY( componentObject->setProperty("validator", "-?\\d{1,3}") );

}

void tst_quickcomponentslineedit::maximumLength()
{
    QVERIFY( componentObject->setProperty("maximumLength", 100) );
    QCOMPARE( componentObject->property("maximumLength").toInt(), 100 );
}

void tst_quickcomponentslineedit::selectionPolicy()
{
}

QTEST_MAIN(tst_quickcomponentslineedit)

#include "tst_quickcomponentslineedit.moc"
