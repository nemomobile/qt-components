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
#include <QDeclarativeView>
#include <QLineEdit>
#include <QTextOption>
#include <QDeclarativeExpression>
#include <QClipboard>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstextarea : public QObject

{
    Q_OBJECT

private slots:
    void initTestCase();
    void font();
    void cursorPosition();
    void horizontalAlignment();
    void verticalAlignment();
    void readOnly();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void textFormat();
    void wrapMode();
    void copypaste();
    void cutpaste();
    void positionAt();
    void positionToRectangle();

private:
    QObject *componentObject;
    QDeclarativeEngine *engine;
};

void tst_quickcomponentstextarea::initTestCase()
{
    QString errors;
    engine = 0;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextarea.qml", &errors, &engine);
    QVERIFY2(componentObject, qPrintable(errors));
}


void tst_quickcomponentstextarea::font()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("font", "Helvetica"));
    QVERIFY(componentObject->property("font").toString().startsWith("Helvetica"));
}

void tst_quickcomponentstextarea::cursorPosition()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("cursorPosition", 0));
    QCOMPARE(componentObject->property("cursorPosition").toInt(), 0);
}

void tst_quickcomponentstextarea::horizontalAlignment()
{
    // test setting/getting the property
    QVERIFY( componentObject->setProperty("horizontalAlignment", 0) );
    QCOMPARE( componentObject->property("horizontalAlignment").toInt(), 0 );
}

void tst_quickcomponentstextarea::verticalAlignment()
{
    // test setting/getting the property
    QVERIFY( componentObject->setProperty("verticalAlignment", 0) );
    QCOMPARE( componentObject->property("verticalAlignment").toInt(), 0 );
}

void tst_quickcomponentstextarea::readOnly()
{
    QVERIFY(componentObject->setProperty("readOnly", true));
    QVERIFY(componentObject->setProperty("text", "I just changed the text"));
    QEXPECT_FAIL("", "Not yet guarded by readOnly property, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-318", Continue);
    QVERIFY(componentObject->property("text").toString() != QString("I just changed the text"));

    // if readOnly == true, we should not be able to cut or paste
    // set the text to something known, select and cut it
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "cut"), "Could not cut the text");

    // check if the text area was cleared
    // ###: Change the string being compared here when the test above is fixed (readOnly guard)
    QCOMPARE(componentObject->property("text").toString(), QString("I just changed the text"));

    // we shouldn't be able to paste stuff on it too
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "copy"), "Could not copy the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // the text should remain the same as readOnly == true
    // ###: Change the string being compared here when the test above is fixed (readOnly guard)
    QCOMPARE(componentObject->property("text").toString(), QString("I just changed the text"));

    // reset readonly so it doesn't affect the next tests
    componentObject->setProperty("readOnly", false);
}

void tst_quickcomponentstextarea::selectedText()
{
    componentObject->setProperty("text", "Good morning");

    // test select(start, end)
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 4)),
             "Could not select from 0-4");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good") );

    // test selectAll()
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good morning") );

    // test selectWord (it will select the word closest to the cursor)
    componentObject->setProperty("cursorPosition", 0);
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectWord"), "Could not select one word");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentstextarea::selectionStart()
{
    componentObject->setProperty("text", "Good morning");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 11)),
             "Could not select from 5-11");
    QCOMPARE(componentObject->property("selectionStart").toInt(), 5);
}

void tst_quickcomponentstextarea::selectionEnd()
{
    componentObject->setProperty("text", "Good morning");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 11)),
             "Could not select from 5-11");
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 11);
}

void tst_quickcomponentstextarea::text()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("text", "Hello World"));
    QCOMPARE(componentObject->property("text").toString(), QString("Hello World"));
}

void tst_quickcomponentstextarea::textFormat()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("textFormat", Qt::AutoText));
    QCOMPARE(componentObject->property("textFormat").toInt(), (int)Qt::AutoText);

    componentObject->setProperty("textFormat", 4);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            <= (int)Qt::LogText);

    // try to set a lower value
    componentObject->setProperty("textFormat", -1);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            >= (int)Qt::PlainText);

    // try to set random value
    componentObject->setProperty("textFormat", qrand());
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            <= (int)Qt::PlainText
            && componentObject->property("textFormat").toInt()
            >= (int)Qt::LogText);
}

void tst_quickcomponentstextarea::wrapMode()
{
    QVERIFY( componentObject->setProperty("wrapMode", 0) );
    QCOMPARE( componentObject->property("wrapMode").toInt(), 0 );
}

void tst_quickcomponentstextarea::copypaste()
{
    // set the text to something known, select and copy it
    componentObject->setProperty("text", "Hello Copy World");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "copy"), "Could not copy the text");

    // check if clipboard's content is right
    QCOMPARE(QApplication::clipboard()->text(), QString("Hello Copy World"));

    // clear text property
    componentObject->setProperty("text", "");
    QCOMPARE(componentObject->property("text").toString(), QString(""));

    // paste
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // check value of text
    QCOMPARE(componentObject->property("text").toString(), QString("Hello Copy World"));
}

void tst_quickcomponentstextarea::cutpaste()
{
    // set the text to something known, select and cut it
    componentObject->setProperty("text", "Hello Cut World");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "cut"), "Could not cut the text");

    // check if clipboard's content is right
    QCOMPARE(QApplication::clipboard()->text(), QString("Hello Cut World"));

    // check if the text area was cleared
    QCOMPARE(componentObject->property("text").toString(), QString(""));

    // check if the cursor position is '0' (no text after all)
    QCOMPARE(componentObject->property("cursorPosition").toInt(), 0);

    // check if selection start and end are '0' too
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);

    // paste
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // check value of text
    QCOMPARE(componentObject->property("text").toString(), QString("Hello Cut World"));
}

void tst_quickcomponentstextarea::positionAt()
{
    // set the text to something known
    QVariant retVal;
    const QString text("Hello from Position World");
    componentObject->setProperty("text", text);

    // values smaller than the position of the first char will always return 0
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, 0), Q_ARG(QVariant, 0)),
             "Could not call positionAt");
    QCOMPARE(retVal.toInt(), 0);

    // this test considers that the width of the element is really big
    // big enough to fit the text and still have some more space
    // values bigger than the position of the last char will always return the size of the string
    const int size = componentObject->property("width").toInt();
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, size), Q_ARG(QVariant, 0)),
             "Could not call positionAt)");
    QCOMPARE(retVal.toInt(), text.size());

    QFontMetrics m(componentObject->property("font").toString());
    const int width = m.width(text);

    // random position considering the string above. it will return the char
    // position at x = width/2 pixels (should be the 17th char (o))
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, width/2), Q_ARG(QVariant, 0)),
             "Could not call positionAt)");
    QCOMPARE(retVal.toInt(), 17);
    QCOMPARE(text.at(retVal.toInt()), QString("o").at(0));
}

void tst_quickcomponentstextarea::positionToRectangle()
{
    // set the text to something known
    QVariant retVal;
    const QString text("Hello from Rectangle World");
    componentObject->setProperty("text", text);

    // the cursor position should not change after a call to positionToRectangle()
    const int oldPosition = componentObject->property("cursorPosition").toInt();

    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionToRectangle",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, 17)),
             "Could not call positionToRectangle");

    // for char number 17 with the specified font (Helvetica) the cursor
    // rect whould be at = topleft(99, 0) and =size(1, 16)
    QCOMPARE(retVal.toRectF(), QRectF(98, 0, 1, 16));

    // the position shouldn't have changed
    QCOMPARE(oldPosition, componentObject->property("cursorPosition").toInt());

}

QTEST_MAIN(tst_quickcomponentstextarea)

#include "tst_quickcomponentstextarea.moc"
