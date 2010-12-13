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

class tst_quickcomponentsmultilineedit : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentsmultilineedit();

private slots:
    void font();
    void cursorPosition();
    void horizontalAlignment();
    void verticalAlignment();
    void contentHeight();
    void contentWidth();
    void readOnly();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void textFormat();
    void wrapMode();

private:
    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};

tst_quickcomponentsmultilineedit::tst_quickcomponentsmultilineedit()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    engine = window->engine();
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());

    QFile file("tst_quickcomponentsmultilineedit.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    QVERIFY( (componentObject = component->create()) != 0);
}


void tst_quickcomponentsmultilineedit::font()
{
    QVERIFY( componentObject->setProperty("font.family", "Helvetica") );
    QCOMPARE( componentObject->property("font.family").toString(), QString("Helvetica") );
}

void tst_quickcomponentsmultilineedit::cursorPosition()
{
    QVERIFY( componentObject->setProperty("cursorPosition", 0) );
    QCOMPARE( componentObject->property("cursorPosition").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::horizontalAlignment()
{
    QVERIFY( componentObject->setProperty("horizontalAlignment", 0) );
    QCOMPARE( componentObject->property("horizontalAlignment").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::verticalAlignment()
{
    QVERIFY( componentObject->setProperty("verticalAlignment", 0) );
    QCOMPARE( componentObject->property("verticalAlignment").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::contentHeight()
{
    QVERIFY( componentObject->setProperty("contentHeight", 0) );
    QCOMPARE( componentObject->property("contentHeight").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::contentWidth()
{
    QVERIFY( componentObject->setProperty("contentWidth", 0) );
    QCOMPARE( componentObject->property("contentWidth").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::readOnly()
{
    QVERIFY( componentObject->setProperty("readOnly", true) );
    QVERIFY( componentObject->setProperty("text", "I just changed the text") );
    QVERIFY( componentObject->property("text").toString() != QString("I just changed the text"));
}

void tst_quickcomponentsmultilineedit::selectedText()
{
    QVERIFY( componentObject->setProperty("text", "Good morning") );
    QVERIFY( componentObject->setProperty("selectionStart", 0) );
    QVERIFY( componentObject->setProperty("selectionEnd", 4) );

    QCOMPARE( componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentsmultilineedit::selectionEnd()
{
    QVERIFY( componentObject->setProperty("selectionEnd", 2) );

}

void tst_quickcomponentsmultilineedit::selectionStart()
{
    QVERIFY( componentObject->setProperty("selectionStart", 1) );
}

void tst_quickcomponentsmultilineedit::text()
{
    QVERIFY( componentObject->setProperty("text", "Hello World") );
    QCOMPARE( componentObject->property("text").toString(), QString("Hello World") );
}

void tst_quickcomponentsmultilineedit::textFormat()
{
    QVERIFY( componentObject->setProperty("textFormat", 0) );
    QCOMPARE( componentObject->property("textFormat").toInt(), 0 );
}

void tst_quickcomponentsmultilineedit::wrapMode()
{
    QVERIFY( componentObject->setProperty("wrapMode", 0) );
    QCOMPARE( componentObject->property("wrapMode").toInt(), 0 );
}

QTEST_MAIN(tst_quickcomponentsmultilineedit)

#include "tst_quickcomponentsmultilineedit.moc"
