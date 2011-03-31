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
#include <QGraphicsObject>
#include "tst_quickcomponentstest.h"

static const QString EDITOR_STYLE_FONT = "Nokia Sans,-1,20,5,50,0,0,0,0,0";

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
    QVERIFY(textField);
    QVERIFY(placeHolder);

    QVERIFY(textField->property("placeholderText").isValid());
    QVERIFY(textField->property("placeholderText").toString().isEmpty());
    QVERIFY(placeHolder->property("font").isValid());
    QCOMPARE(placeHolder->property("font").toString(), EDITOR_STYLE_FONT);

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
    QEXPECT_FAIL("", "inputMethodQuery(Qt::ImMicroFocus) returns quite wide cursor rectangle for TextInput, http://bugreports.qt.nokia.com/browse/QTBUG-18343", Continue);
    QCOMPARE(m_view->scene()->inputMethodQuery(Qt::ImMicroFocus), cursorRect);
    QVERIFY(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus).isValid());
    QEXPECT_FAIL("", "inputMethodQuery(Qt::ImMicroFocus) returns quite wide cursor rectangle for TextInput, http://bugreports.qt.nokia.com/browse/QTBUG-18343", Continue);
    QCOMPARE(QApplication::focusWidget()->inputMethodQuery(Qt::ImMicroFocus), cursorRect);
}

QTEST_MAIN(tst_quickcomponentstextfield)

#include "tst_quickcomponentstextfield.moc"
