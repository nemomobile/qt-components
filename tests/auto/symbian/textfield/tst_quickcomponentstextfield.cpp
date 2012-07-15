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
#include <QGraphicsObject>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "tst_quickcomponentstest.h"

#ifdef Q_OS_SYMBIAN
static const QString EDITOR_STYLE_FONT = "Nokia Sans S60,-1,20,5,50,0,0,0,0,0";
#else
static const QString EDITOR_STYLE_FONT = "Nokia Sans,-1,20,5,50,0,0,0,0,0";
#endif

class tst_quickcomponentstextfield : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void defaultPropertyValues();
    void acceptableInput();
    void implicitSize();
    void font();
    void focus();
    void cursorRectangle();
    void placeholderText();
    void placeholderTextAndPresetText();
    void placeholderTextAndReadOnly();
    void enabled();
    void maximumLength();
    void platformInverted();

private:
    QObject* m_componentObject;
    QScopedPointer<QDeclarativeView> m_view;
};

void tst_quickcomponentstextfield::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextfield.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));

    m_view.reset(tst_quickcomponentstest::createDeclarativeView("tst_quickcomponentstextfield.qml"));
    QTest::qWaitForWindowShown(m_view.data());
    QVERIFY(m_view.data());
    QVERIFY(m_view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(m_view.data()));
}

void tst_quickcomponentstextfield::defaultPropertyValues()
{
    QGraphicsObject* textField = m_componentObject->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *placeHolder = m_view->rootObject()->findChild<QGraphicsObject*>("placeholder");
    QObject* platformStyle = m_view->engine()->rootContext()->contextProperty("platformStyle").value<QObject*>();
    QVERIFY(textField);
    QVERIFY(placeHolder);
    QVERIFY(platformStyle);

    QVERIFY(textField->property("placeholderText").isValid());
    QVERIFY(textField->property("placeholderText").toString().isEmpty());
    QVERIFY(placeHolder->property("font").isValid());
    QVERIFY(placeHolder->property("font").toString() == EDITOR_STYLE_FONT);

    QVERIFY(textField->property("inputMethodHints").isValid());
    QCOMPARE(textField->property("inputMethodHints").toInt(), int(Qt::ImhNone));

    QVERIFY(textField->property("font").isValid());
    QCOMPARE(textField->property("font").toString(), EDITOR_STYLE_FONT);

    QVERIFY(textField->property("cursorPosition").isValid());

    QVERIFY(textField->property("readOnly").isValid());
    QVERIFY(!textField->property("readOnly").toBool());

    QVERIFY(textField->property("echoMode").isValid());
    QCOMPARE(textField->property("echoMode").toInt(), 0);

    QVERIFY(textField->property("acceptableInput").isValid());
    QVERIFY(textField->property("acceptableInput").toBool());

    QVERIFY(textField->property("inputMask").isValid());
    QVERIFY(textField->property("inputMask").toString().isEmpty());

    QVERIFY(textField->property("validator").isValid());

    QVERIFY(textField->property("selectedText").isValid());
    QVERIFY(textField->property("selectedText").toString().isEmpty());

    QVERIFY(textField->property("selectionEnd").isValid());
    QVERIFY(textField->property("selectionStart").isValid());
    //No selection so the 2 properties should be equal
    QCOMPARE(textField->property("selectionEnd").toInt(), textField->property("selectionStart").toInt());

    QVERIFY(textField->property("text").isValid());
    QCOMPARE(textField->property("text").toString(), QString("test"));

    QVERIFY(textField->property("platformLeftMargin").isValid());
    QCOMPARE(textField->property("platformLeftMargin"), platformStyle->property("paddingSmall"));

    QVERIFY(textField->property("platformRightMargin").isValid());
    QCOMPARE(textField->property("platformRightMargin"), platformStyle->property("paddingSmall"));
    
    QVERIFY(textField->property("errorHighlight").isValid());
    QVERIFY(!textField->property("errorHighlight").toBool());

    QVERIFY(textField->property("platformInverted").isValid());
    QVERIFY(!textField->property("platformInverted").toBool());
}

void tst_quickcomponentstextfield::acceptableInput()
{
    QGraphicsObject* textField = m_componentObject->findChild<QGraphicsObject*>("textFieldWithValidator");
    QVERIFY(textField);

    // Validator is set to acceptable values between 11 and 31
    QVERIFY(textField->setProperty("text", "5"));
    QVERIFY(!textField->property("acceptableInput").toBool());
    QVERIFY(textField->setProperty("text", "25"));
    QVERIFY(textField->property("acceptableInput").toBool());
}

void tst_quickcomponentstextfield::implicitSize()
{
    // Test that the implicit width and height is set
    QGraphicsObject* textField = m_componentObject->findChild<QGraphicsObject*>("textFieldImpSize");
    QVERIFY(textField);

    int implicitWidth = textField->property("implicitWidth").toInt();
    int implicitHeight = textField->property("implicitHeight").toInt();

    // Test that a change to the text resize the implicit width
    QVERIFY(textField->setProperty("text", "ImplicitSizeTest"));
    QVERIFY(textField->property("implicitWidth").toInt() > implicitWidth);

    // Test that setting a longer placeholderText resize the implicit width
    implicitWidth = textField->property("implicitWidth").toInt();
    QVERIFY(textField->setProperty("placeholderText", "ImplicitSizeTest_ImplicitSizeTest"));
    QVERIFY(textField->property("implicitWidth").toInt() > implicitWidth);

    //Test that a font change resize the the implicit width and height
    implicitWidth = textField->property("implicitWidth").toInt();
    QFont font = textField->property("font").value<QFont>();
    font.setPixelSize(font.pixelSize() + 5);
    textField->setProperty("font", font);
    QVERIFY(textField->property("implicitWidth").toInt() > implicitWidth);
    QVERIFY(textField->property("implicitHeight").toInt() > implicitHeight);
}

void tst_quickcomponentstextfield::font()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *textInput = m_view->rootObject()->findChild<QGraphicsObject*>("textInput");
    QGraphicsObject *placeHolder = m_view->rootObject()->findChild<QGraphicsObject*>("placeholder");
    QVERIFY(textField);
    QVERIFY(textInput);
    QVERIFY(placeHolder);

    QFont font;
    font.setBold(true);
    font.setPixelSize(25);
    font.setFamily("Arial");
    textField->setProperty("font", font);

    QCOMPARE(font, textField->property("font").value<QFont>());
    QCOMPARE(font, textInput->property("font").value<QFont>());
    QCOMPARE(font, placeHolder->property("font").value<QFont>());
}

void tst_quickcomponentstextfield::focus()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *textInput = m_view->rootObject()->findChild<QGraphicsObject*>("textInput");
    QGraphicsObject *textFieldImpSize = m_view->rootObject()->findChild<QGraphicsObject*>("textFieldImpSize");

    QVERIFY(!textField->property("focus").toBool());
    QVERIFY(!textField->property("activeFocus").toBool());
    QVERIFY(textInput->property("focus").toBool());
    QVERIFY(!textInput->property("activeFocus").toBool());
    QVERIFY(!textFieldImpSize->property("focus").toBool());
    QVERIFY(!textFieldImpSize->property("activeFocus").toBool());

    textField->setFocus(Qt::OtherFocusReason);

    QVERIFY(textField->property("focus").toBool());
    QVERIFY(textField->property("activeFocus").toBool());
    QVERIFY(textInput->property("focus").toBool());
    QVERIFY(textInput->property("activeFocus").toBool());
    QVERIFY(!textFieldImpSize->property("focus").toBool());
    QVERIFY(!textFieldImpSize->property("activeFocus").toBool());

    textFieldImpSize->setFocus(Qt::OtherFocusReason);

    QVERIFY(!textField->property("focus").toBool());
    QVERIFY(!textField->property("activeFocus").toBool());
    QVERIFY(textInput->property("focus").toBool());
    QVERIFY(!textInput->property("activeFocus").toBool());
    QVERIFY(textFieldImpSize->property("focus").toBool());
    QVERIFY(textFieldImpSize->property("activeFocus").toBool());

    textField->setFocus(Qt::OtherFocusReason);

    QVERIFY(textField->property("focus").toBool());
    QVERIFY(textField->property("activeFocus").toBool());
    QVERIFY(textInput->property("focus").toBool());
    QVERIFY(textInput->property("activeFocus").toBool());
    QVERIFY(!textFieldImpSize->property("focus").toBool());
    QVERIFY(!textFieldImpSize->property("activeFocus").toBool());
}

void tst_quickcomponentstextfield::cursorRectangle()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *button = m_view->rootObject()->findChild<QGraphicsObject*>("testButton");
    QVariant cursorRect;

    button->setFocus(Qt::OtherFocusReason);
    QVERIFY(!m_view->scene()->inputMethodQuery(Qt::ImMicroFocus).isValid());
    QVERIFY(!QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).isValid());

    textField->setFocus(Qt::OtherFocusReason);
    QVERIFY2(QMetaObject::invokeMethod(textField,
                                       "positionToRectangle",
                                       Q_RETURN_ARG(QVariant, cursorRect),
                                       Q_ARG(QVariant, textField->property("cursorPosition").toInt())),
                                       "Could not call positionToRectangle");

    QVERIFY(m_view->scene()->inputMethodQuery(Qt::ImMicroFocus).isValid());
    // Compare x, width and height properties as y property is not set in positionToRectangle()
    QCOMPARE(m_view->scene()->inputMethodQuery(Qt::ImMicroFocus).toRectF().x(), cursorRect.toRectF().x());
    QCOMPARE(m_view->scene()->inputMethodQuery(Qt::ImMicroFocus).toRectF().width(), cursorRect.toRectF().width());
    QCOMPARE(m_view->scene()->inputMethodQuery(Qt::ImMicroFocus).toRectF().height(), cursorRect.toRectF().height());

    QVERIFY(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).isValid());
    QCOMPARE(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).toRectF().x(), cursorRect.toRectF().x());
    QCOMPARE(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).toRectF().width(), cursorRect.toRectF().width());
    QCOMPARE(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).toRectF().height(), cursorRect.toRectF().height());
}

void tst_quickcomponentstextfield::placeholderText()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);

    QGraphicsObject *button = m_view->rootObject()->findChild<QGraphicsObject*>("testButton");
    QGraphicsObject *textInput = textField->findChild<QGraphicsObject*>("textInput");
    QGraphicsObject *placeHolder = textField->findChild<QGraphicsObject*>("placeholder");

    QVERIFY(button);
    QVERIFY(textInput);
    QVERIFY(placeHolder);

    //move focus
    button->setFocus(Qt::OtherFocusReason);

    textField->setProperty("text", QString(""));
    textField->setProperty("placeholderText", QString("placeholderText"));

    // Place holder should be visible
    QCOMPARE(placeHolder->property("visible").toBool(), true);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));

    // Focus textField
    textField->setFocus(Qt::OtherFocusReason);
    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));

    //Type something
    textField->setProperty("text", QString("Test"));
    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString("Test"));

    // Focus another component
    button->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString("Test"));

    // Focus back, empty text and focus out again
#ifdef Q_OS_SYMBIAN
    // To avoid virtual keyboard on symbian
    textField->setProperty("readOnly", true);
#endif
    textField->setFocus(Qt::OtherFocusReason);
    textField->setProperty("text", QString(""));
    button->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), true);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));
#ifdef Q_OS_SYMBIAN
    // To avoid virtual keyboard on symbian
    textField->setProperty("readOnly", false);
#endif

    // Empty prompt text
    textField->setProperty("placeholderText", QString(""));
    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString(""));
    QCOMPARE(textField->property("text").toString(), QString(""));
}

void tst_quickcomponentstextfield::placeholderTextAndPresetText()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);

    QGraphicsObject *button = m_view->rootObject()->findChild<QGraphicsObject*>("testButton");
    QGraphicsObject *textInput = textField->findChild<QGraphicsObject*>("textInput");
    QGraphicsObject *placeHolder = textField->findChild<QGraphicsObject*>("placeholder");

    QVERIFY(button);
    QVERIFY(textInput);
    QVERIFY(placeHolder);

    textField->setProperty("text", QString("Preset text here."));
    textField->setProperty("placeholderText", QString("placeholderText"));

    // Preset text should displayed
    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString("Preset text here."));

    // Focus textField
    textField->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString("Preset text here."));

    // Focus out
    button->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString("Preset text here."));

    // Focus back and empty text and focus out again
#ifdef Q_OS_SYMBIAN
    // To avoid virtual keyboard on symbian
    textField->setProperty("readOnly", true);
#endif
    textField->setFocus(Qt::OtherFocusReason);
    textField->setProperty("text", QString(""));
    button->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), true);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));
#ifdef Q_OS_SYMBIAN
    // To avoid virtual keyboard on symbian
    textField->setProperty("readOnly", false);
#endif

    // Empty prompt text
    textField->setProperty("placeholderText", QString(""));
    QCOMPARE(placeHolder->property("visible").toBool(), false);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString(""));
    QCOMPARE(textField->property("text").toString(), QString(""));
}

void tst_quickcomponentstextfield::placeholderTextAndReadOnly()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);

    QGraphicsObject *button = m_view->rootObject()->findChild<QGraphicsObject*>("testButton");
    QGraphicsObject *textInput = textField->findChild<QGraphicsObject*>("textInput");
    QGraphicsObject *placeHolder = textField->findChild<QGraphicsObject*>("placeholder");

    QVERIFY(button);
    QVERIFY(textInput);
    QVERIFY(placeHolder);

    textField->setProperty("text", QString(""));
    textField->setProperty("placeholderText", QString("placeholderText"));
    textField->setProperty("readOnly", true);

    // Prompt should be visible
    QCOMPARE(placeHolder->property("visible").toBool(), true);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));

    // Focus textField
    textField->setFocus(Qt::OtherFocusReason);

    QCOMPARE(placeHolder->property("visible").toBool(), true);
    QCOMPARE(textInput->property("visible").toBool(), true);
    QCOMPARE(textField->property("placeholderText").toString(), QString("placeholderText"));
    QCOMPARE(textField->property("text").toString(), QString(""));

    // Focus out
    button->setFocus(Qt::OtherFocusReason);
}

void tst_quickcomponentstextfield::enabled()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *textInput = m_view->rootObject()->findChild<QGraphicsObject*>("textInput");
    QVERIFY(textField);
    QVERIFY(textInput);
    QVERIFY(textField->property("enabled").toBool());
    QVERIFY(textInput->property("enabled").toBool());

    textField->setProperty("text", QString("Some text"));
    // Focus textArea
    textField->setFocus(Qt::OtherFocusReason);
    QVERIFY(textField->property("activeFocus").toBool());
    QVERIFY(textInput->property("cursorVisible").toBool());
    // Make selection
    QVERIFY(QMetaObject::invokeMethod(textField, "selectAll"));
    QVERIFY(textField->property("selectionEnd").toInt() >
            textField->property("selectionStart").toInt());

    // Make sure that setting to disabled behaves as expected
    textField->setProperty("enabled", false);
    QVERIFY(!textField->property("enabled").toBool());
    QVERIFY(!textField->property("activeFocus").toBool());
    QVERIFY(!textInput->property("cursorVisible").toBool());
    QCOMPARE(textField->property("selectionEnd").toInt(),
             textField->property("selectionStart").toInt()); // no selection
}

void tst_quickcomponentstextfield::maximumLength()
{
    QGraphicsObject* textField = m_componentObject->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);

    // Set a value to the maximum length
    QVERIFY(textField->setProperty("maximumLength", 20));
    QCOMPARE(textField->property("maximumLength").toInt(), 20);

    // Set a text that is as long as the set value for the max length
    QString testText("12345678901234567890");
    QVERIFY(textField->setProperty("text", testText));
    QCOMPARE(textField->property("text").toString(), testText);

    // Set a longer text than the set value for max length is
    QVERIFY(textField->setProperty("text", QString("12345678901234567890asdfghjkl")));
    // The text should be truncated to the length of max length
    QCOMPARE(textField->property("text").toString(), testText);
}

void tst_quickcomponentstextfield::platformInverted()
{
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);
    textField->setProperty("platformInverted", true);
    QCOMPARE(textField->property("platformInverted").toBool(), true);
}

QTEST_MAIN(tst_quickcomponentstextfield)

#include "tst_quickcomponentstextfield.moc"
