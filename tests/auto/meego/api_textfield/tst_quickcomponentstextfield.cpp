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
#include <qdeclarativecontext.h>
#include <qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QDeclarativeExpression>
#include <QRegExpValidator>
#include <QFontDatabase>
#include <QFontMetrics>

#include "tst_quickcomponentstest.h"

QML_DECLARE_TYPE(QValidator)


class tst_quickcomponentstextfield : public QObject

{
    Q_OBJECT

public:
    tst_quickcomponentstextfield();

private slots:
    void initTestCase();
    void placeholderText();
    void inputMethodHints();
    void font();
    void cursorPosition();
    void readOnly();
    void echoMode();
    void text();
    void inputMask();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void acceptableInput();

    // Functions
    void copyAndPaste();
    void cutAndPaste();
    void selectAll();
    void selectWord();
    void positionAt();
    void positionToRectangle();

private:
    QObject *findTextInput(QObject *root);

    // our internal objects
    QObject *componentObject;
    QGraphicsObject *root;
    QGraphicsObject *textInput;

    // font helpers
    QFont mfont;
    QString family;
    int fontSize;
};

tst_quickcomponentstextfield::tst_quickcomponentstextfield()
    : componentObject(NULL)
    , root(NULL)
    , textInput(NULL)
{
}

QObject *tst_quickcomponentstextfield::findTextInput(QObject *root)
{
    QObject *returnValue = NULL;
    if (QString(root->metaObject()->className()).contains(QString("QDeclarativeTextInput")))
        return root;

    QObjectList children = root->children();
    if (!children.isEmpty()) {
        for (int i = 0; i < children.size() && !returnValue; ++i)
            returnValue = findTextInput(children.at(i));
    }
    return returnValue;
}

void tst_quickcomponentstextfield::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextfield.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));

    // select a font from the database
    QFontDatabase database;
    family = database.families().at(0);
    fontSize = database.smoothSizes(family, database.styles(family).at(0)).at(0);

    // create the font based on the one found in the system and set it
    mfont = QFont(family, fontSize);
    componentObject->setProperty("font", mfont);

    // we need to find the child that is a QDeclarativeTextInput in order to properly
    // map the position so this test doesn't fail when there are margins.
    // this is a safe assumption for symbian and meego components as these tests
    // aims to test the features of those two platforms components.
    root = qobject_cast<QGraphicsObject*>(componentObject);
    textInput = qobject_cast<QGraphicsObject*>(findTextInput(componentObject));
    QVERIFY(root);
    QVERIFY(textInput);
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
    QFont temp;
    QVERIFY(componentObject->setProperty("font", mfont));
    temp = componentObject->property("font").value<QFont>();
    QCOMPARE(temp, mfont);
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
    // reset text
    componentObject->setProperty("text", QString(""));

    // acceptableInput should be true by default because no validator was set yet
    QCOMPARE(componentObject->property("acceptableInput").toBool(), true);

    // create and set the validator
    QValidator *validator = new QRegExpValidator(QRegExp("-?\\d{1,3}"), 0);
    componentObject->setProperty("validator", QVariant::fromValue(validator));

    // check if we could setup the validator
    QVariant tmpVariant = componentObject->property("validator");
    QValidator *tmpValidator = tmpVariant.value<QValidator*>();
    QCOMPARE(validator, tmpValidator);

    // acceptableInput should be false just after the setup of the validator (with no text)
    QCOMPARE(componentObject->property("acceptableInput").toBool(), false);

    // depending on validator, acceptable input should be true / false
    // set text that is allowed by the validator
    componentObject->setProperty("text", "-42");
    QCOMPARE(componentObject->property("acceptableInput").toBool(), true);

    componentObject->setProperty("text", "128");
    QCOMPARE(componentObject->property("acceptableInput").toBool(), true);

    // make sure setting the text fails if validator fails
    componentObject->setProperty("text", "qwerasdfzxvc");
    QCOMPARE(componentObject->property("acceptableInput").toBool(), false);

    // reset the validator property to tests after this one are not affected
    QValidator *reset = 0;
    componentObject->setProperty("validator", QVariant::fromValue(reset));
    QCOMPARE(componentObject->property("acceptableInput").toBool(), true);
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
#if QT_VERSION < 0x040702
   QEXPECT_FAIL("", "Selected word should be morning", Continue);
#endif
    QCOMPARE(componentObject->property("selectedText").toString(), QString("morning"));

    QVERIFY(QMetaObject::invokeMethod(componentObject, "cut"));
#if QT_VERSION < 0x040702
   QEXPECT_FAIL("", "'morning' was cut, so cursorPosition should be at 5", Continue);
#endif
    QCOMPARE(componentObject->property("selectionStart").toInt(), 5);
#if QT_VERSION < 0x040702
   QEXPECT_FAIL("", "'morning' was cut, so cursorPosition should be at 5", Continue);
#endif
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 5);
    QVERIFY(QMetaObject::invokeMethod(componentObject, "paste"));
    QCOMPARE(componentObject->property("text").toString(), QString("Good morning"));
}

void tst_quickcomponentstextfield::positionAt()
{
    // set the text to something known
    QVariant retVal;
    const QString text("Hello from Position World");
    componentObject->setProperty("text", text);
    componentObject->setProperty("cursorPosition", 0);

    // values smaller than the position of the first char will always return 0
    const int smaller = textInput->mapToItem(root, 0, 0).x();
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, smaller)),
                                       "Could not call positionAt");
    QCOMPARE(retVal.toInt(), 0);

    // this test considers that the width of the element is really big
    // big enough to fit the text and still have some more space
    // values bigger than the position of the last char will always return the size of the string
    const int size = componentObject->property("width").toInt();
    const int bigger = textInput->mapToItem(root, size, 0).x();
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, bigger)),
                                       "Could not call positionAt");
    QCOMPARE(retVal.toInt(), text.size());

    // the position returned for the pixel that is in the end of the text
    // should be the position of the last char
    QFontMetrics fm(mfont);
    const int width = fm.width(text);
    const int mappedWidth = textInput->mapToItem(root, width, 0).x();

    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, mappedWidth)),
                                       "Could not call positionAt");

    QCOMPARE(text.left(retVal.toInt()), text);
}

void tst_quickcomponentstextfield::positionToRectangle()
{
    // set the text to something known
    QVariant retVal;
    QVariant retValPos;
    const QString text("Hello from Rectangle World");
    componentObject->setProperty("text", text);
    componentObject->setProperty("cursorPosition", 0);

    // the cursor position should not change after a call to positionToRectangle()
    const int oldPosition = componentObject->property("cursorPosition").toInt();

    // now we take the rectangle and check if at the given position we have the right
    // char. because of this, positionToRectangle() should be tested *after* positionAt()
    // because we rely that it's working properly to check the right position
    const int size = text.size() / 2;
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionToRectangle",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, size)),
                                       "Could not call positionToRectangle");

    const QRectF rect = retVal.toRectF();
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retValPos), Q_ARG(QVariant, rect.x())),
                                       "Could not call positionAt");

    // it should be the same as used in 'value' (in this case size)
    QCOMPARE(retValPos.toInt(), size);

    // the 'x' of the rectangle should be the same as the width of the text
    // until the position returned
    QFontMetrics fm(mfont);
    QCOMPARE((int)root->mapRectToItem(textInput, retVal.toRectF()).x(), fm.width(text.left(retValPos.toInt())));

    // the position shouldn't have changed
    QCOMPARE(oldPosition, componentObject->property("cursorPosition").toInt());
}


QTEST_MAIN(tst_quickcomponentstextfield)

#include "tst_quickcomponentstextfield.moc"
